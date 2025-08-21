//! Minimal HTTP LED microservice for Arduino UNO R4 WiFi
#include <Arduino.h>
#include <stdint.h>
#include <WiFiS3.h>
#include "arduino_secrets.h"
#include "Http/Api.h"
#include "Http/Response.h"
#include "Led/LedHardware.h"
#include "Temp/TempHardware.h"
#include "Temp/TempMonitor.h"
#include <string_view>
#include <float.h>

//! Networking
static WiFiServer server(80);

//! Hardware
static const uint8_t LED_PIN = LED_BUILTIN;
static LedHardware led(LED_PIN);
static uint32_t last_ip_print_ms = 0;

//! TMP36 sensor on A0: 0.5s inner sampling, 10s outer aggregation
static TempHardware temp_hw(A0);
static TempMonitor temp_sensor(temp_hw);
static bool parse_threshold_from_body(WiFiClient &client, float &out_value) {
  // Read any remaining bytes (body) into a small buffer
  char buf[64];
  size_t n = 0;
  unsigned long start = millis();
  while (client.available() && n < sizeof(buf) - 1 && (millis() - start) < 50) {
    buf[n++] = (char)client.read();
  }
  buf[n] = '\0';
  // Expect JSON like {"value": 23.5}
  StaticJsonDocument<96> doc;
  DeserializationError err = deserializeJson(doc, buf);
  if (err) return false;
  if (!doc.containsKey("value")) return false;
  out_value = doc["value"].as<float>();
  return true;
}

//! Utility: return current LED state as string
static std::string_view led_state_string() {
  return led.is_on() ? std::string_view("on") : std::string_view("off");
}

//! Utility: write a JSON HTTP response
static void send_json(WiFiClient &client, int status_code, const String &json_body) {
  String status_text = status_code == 200 ? "OK" : (status_code == 404 ? "Not Found" : "Error");
  client.print("HTTP/1.1 ");
  client.print(status_code);
  client.print(" ");
  client.println(status_text);
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(json_body.length());
  client.println("Connection: close");
  client.println();
  client.print(json_body);
}

//! Consume and discard incoming HTTP headers
static void discard_http_headers(WiFiClient &client) {
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line.length() == 0 || line == "\r\n" || line == "\n" || line == "\r") {
      break;
    }
  }
}

//! Route handlers and dispatcher
static void route_led_on(WiFiClient &client) {
  led.turn_on();
  char body[64];
  auto s = led_state_string();
  make_led_status_json(s, body, sizeof(body));
  send_json(client, 200, String(body));
}

static void route_led_off(WiFiClient &client) {
  led.turn_off();
  char body[64];
  auto s = led_state_string();
  make_led_status_json(s, body, sizeof(body));
  send_json(client, 200, String(body));
}

static void route_led_toggle(WiFiClient &client) {
  led.toggle();
  char body[64];
  auto s = led_state_string();
  make_led_status_json(s, body, sizeof(body));
  send_json(client, 200, String(body));
}

static void route_led_status(WiFiClient &client) {
  char body[64];
  make_led_status_json(led_state_string(), body, sizeof(body));
  send_json(client, 200, String(body));
}

static void route_temp_avg(WiFiClient &client) {
  // 1-sigma clipped average to dampen spikes
  float avg = temp_sensor.average_last_window_sigma_clipped(1.0f);
  StaticJsonDocument<96> doc;
  doc["status"] = "ok";
  float min_alert, max_alert;
  if (temp_sensor.get_alerts(min_alert, max_alert)) {
    doc["alert_min"] = min_alert;
    doc["alert_max"] = max_alert;
    doc["alert_violated"] = temp_sensor.was_alert_violated();
  }
  if (isnan(avg)) {
    doc["message"] = "no data";
  } else {
    doc["celsius_avg_60s"] = avg;
  }
  char body[128];
  serializeJson(doc, body, sizeof(body));
  send_json(client, 200, String(body));
}

static void route_temp_min(WiFiClient &client) {
  StaticJsonDocument<96> doc;
  doc["status"] = "ok";
  float min_val;
  if (!temp_sensor.get_min_historic(min_val)) {
    doc["message"] = "no data";
  } else {
    doc["celsius_min"] = min_val;
  }
  char body[128];
  serializeJson(doc, body, sizeof(body));
  send_json(client, 200, String(body));
}

static void route_temp_max(WiFiClient &client) {
  StaticJsonDocument<96> doc;
  doc["status"] = "ok";
  float max_val;
  if (!temp_sensor.get_max_historic(max_val)) {
    doc["message"] = "no data";
  } else {
    doc["celsius_max"] = max_val;
  }
  char body[128];
  serializeJson(doc, body, sizeof(body));
  send_json(client, 200, String(body));
}

static void route_temp_raw(WiFiClient &client) {
  int raw = analogRead(A0);
  constexpr float fs_counts = 4095.0f; // 12-bit
  constexpr float fs_volts = 3.3f;
  float volts = (static_cast<float>(raw) * fs_volts) / fs_counts;
  float c = temp_hw.read_celsius();
  StaticJsonDocument<160> doc;
  doc["status"] = "ok";
  doc["raw_counts"] = raw;
  doc["volts"] = volts;
  doc["celsius"] = c;
  char body[192];
  serializeJson(doc, body, sizeof(body));
  send_json(client, 200, String(body));
}

static void route_root(WiFiClient &client) {
  StaticJsonDocument<256> doc;
  doc["status"] = "ok";
  JsonArray routes = doc.createNestedArray("routes");
  routes.add(HttpRoutes::LED_ON);
  routes.add(HttpRoutes::LED_OFF);
  routes.add(HttpRoutes::LED_TOGGLE);
  routes.add(HttpRoutes::LED_STATUS);
  routes.add(HttpRoutes::TEMP);
  routes.add(HttpRoutes::TEMP_MIN);
  routes.add(HttpRoutes::TEMP_MAX);
  routes.add(HttpRoutes::TEMP_RAW);
  routes.add(HttpRoutes::TEMP_ALERT_MIN);
  routes.add(HttpRoutes::TEMP_ALERT_MAX);
  routes.add(HttpRoutes::TEMP_ALERT_CLEAR);
  char body[256];
  serializeJson(doc, body, sizeof(body));
  send_json(client, 200, String(body));
}

struct RouteEntry { const char* path; const char* method; void (*handler)(WiFiClient &); };

static const RouteEntry ROUTES[] = {
  { HttpRoutes::LED_ON,    "POST", route_led_on },
  { HttpRoutes::LED_OFF,   "POST", route_led_off },
  { HttpRoutes::LED_TOGGLE,"POST", route_led_toggle },
  { HttpRoutes::LED_STATUS,"GET",  route_led_status },
  { HttpRoutes::TEMP,      "GET",  route_temp_avg },
  { HttpRoutes::TEMP_MIN,  "GET",  route_temp_min },
  { HttpRoutes::TEMP_MAX,  "GET",  route_temp_max },
  { HttpRoutes::TEMP_RAW,  "GET",  route_temp_raw },
  { HttpRoutes::TEMP_ALERT_MIN, "POST", nullptr },
  { HttpRoutes::TEMP_ALERT_MAX, "POST", nullptr },
  { HttpRoutes::TEMP_ALERT_CLEAR, "POST", nullptr },
  { "/",                  "GET",  route_root }
};

static void handle_request(WiFiClient &client, const String &method, const String &path) {
  bool path_found = false;
  for (size_t i = 0; i < sizeof(ROUTES)/sizeof(ROUTES[0]); ++i) {
    if (path.equals(ROUTES[i].path)) {
      path_found = true;
      if (method == ROUTES[i].method) {
        if (ROUTES[i].handler != nullptr) {
          ROUTES[i].handler(client);
        } else {
          // Special-case POST alert endpoints
          if (path == HttpRoutes::TEMP_ALERT_MIN) {
            float v;
            if (!parse_threshold_from_body(client, v)) {
              char body[96];
              make_error_json("bad json: {\"value\":<number>} expected", body, sizeof(body));
              send_json(client, 400, String(body));
              return;
            }
            if (v < TempMonitor::kAllowedMinC || v > TempMonitor::kAllowedMaxC) {
              char body[96];
              make_error_json("value outside [-50,100] C", body, sizeof(body));
              send_json(client, 400, String(body));
              return;
            }
            temp_sensor.set_alert_min(v);
            StaticJsonDocument<96> doc;
            doc["status"] = "ok";
            doc["alert_min"] = v;
            char body[128];
            serializeJson(doc, body, sizeof(body));
            send_json(client, 200, String(body));
          } else if (path == HttpRoutes::TEMP_ALERT_MAX) {
            float v;
            if (!parse_threshold_from_body(client, v)) {
              char body[96];
              make_error_json("bad json: {\"value\":<number>} expected", body, sizeof(body));
              send_json(client, 400, String(body));
              return;
            }
            if (v < TempMonitor::kAllowedMinC || v > TempMonitor::kAllowedMaxC) {
              char body[96];
              make_error_json("value outside [-50,100] C", body, sizeof(body));
              send_json(client, 400, String(body));
              return;
            }
            temp_sensor.set_alert_max(v);
            StaticJsonDocument<96> doc;
            doc["status"] = "ok";
            doc["alert_max"] = v;
            char body[128];
            serializeJson(doc, body, sizeof(body));
            send_json(client, 200, String(body));
          } else if (path == HttpRoutes::TEMP_ALERT_CLEAR) {
            temp_sensor.clear_alert_violation();
            StaticJsonDocument<64> doc;
            doc["status"] = "ok";
            char body[96];
            serializeJson(doc, body, sizeof(body));
            send_json(client, 200, String(body));
          }
        }
        return;
      }
    }
  }
  if (path_found) {
    char body[96];
    make_error_json("method not allowed", body, sizeof(body));
    send_json(client, 405, String(body));
  } else {
    char body[96];
    make_error_json("unknown route", body, sizeof(body));
    send_json(client, 404, String(body));
  }
}

static void connect_wifi() {
  Serial.print("Connecting to WiFi SSID: ");
  Serial.println(SECRET_SSID);

  WiFi.begin(SECRET_SSID, SECRET_PASS);

  uint32_t start = (uint32_t)millis();
  const uint32_t timeout_ms = 20000; // 20s timeout
  while (WiFi.status() != WL_CONNECTED && ((uint32_t)millis() - start) < timeout_ms) {
    delay(250);
    Serial.print('.');
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    IPAddress ip = WiFi.localIP();
    Serial.print("WiFi connected. IP: ");
    Serial.println(ip);
  } else {
    Serial.println("WiFi connection failed. Check credentials in include/arduino_secrets.h");
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  temp_sensor.begin();

  connect_wifi();
  server.begin();
  Serial.println("HTTP LED microservice started on port 80");
}

void loop() {
  // Periodically print IP every 30 seconds when connected
  uint32_t now = (uint32_t)millis();
  if (WiFi.status() == WL_CONNECTED && (uint32_t)(now - last_ip_print_ms) >= 30000U) {
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    if (temp_sensor.is_window_full()) {
      float avg = temp_sensor.average_last_window();
      Serial.print("Temp avg 60s: ");
      Serial.print(avg);
      Serial.println(" C");
    }
    last_ip_print_ms = now;
  }

  WiFiClient client = server.available();
  if (!client) {
    // Sample temperature even when idle
    temp_sensor.sample_if_due(now);
    delay(10);
    return;
  }

  // Wait for data
  uint32_t start = (uint32_t)millis();
  while (client.connected() && !client.available() && ((uint32_t)millis() - start) < 1000) {
    delay(1);
  }

  if (!client.available()) {
    client.stop();
    return;
  }

  // Read request line: METHOD PATH HTTP/1.1
  String request_line = client.readStringUntil('\r');
  client.read(); // Consume '\n'

  // Discard headers to the blank line
  discard_http_headers(client);

  int sp1 = request_line.indexOf(' ');
  int sp2 = request_line.indexOf(' ', sp1 + 1);
  if (sp1 == -1 || sp2 == -1) {
    send_json(client, 400, "{\"status\":\"error\",\"message\":\"bad request line\"}");
    client.stop();
    return;
  }

  String method = request_line.substring(0, sp1);
  String path = request_line.substring(sp1 + 1, sp2);

  handle_request(client, method, path);
  client.stop();

  // Also sample after handling a request
  temp_sensor.sample_if_due((uint32_t)millis());
}
