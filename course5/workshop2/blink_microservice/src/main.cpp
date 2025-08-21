// Minimal HTTP LED microservice for Arduino UNO R4 WiFi
#include <Arduino.h>
#include <stdint.h>
#include <WiFiS3.h>
#include "arduino_secrets.h"
#include "Http/Api.h"
#include "Http/Response.h"
#include "Led/LedHardware.h"
#include <string_view>

// Networking
static WiFiServer server(80);

// Hardware
static const uint8_t LED_PIN = LED_BUILTIN;
static LedHardware led(LED_PIN);
static uint32_t last_ip_print_ms = 0;

// Utility: return current LED state as string
static std::string_view led_state_string() {
  return led.is_on() ? std::string_view("on") : std::string_view("off");
}

// Utility: write a JSON HTTP response
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

// Consume and discard incoming HTTP headers
static void discard_http_headers(WiFiClient &client) {
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line.length() == 0 || line == "\r\n" || line == "\n" || line == "\r") {
      break;
    }
  }
}

// Route handling
static void handle_request(WiFiClient &client, const String &method, const String &path) {
  if (path == HttpRoutes::LED_ON) {
    if (method != "POST") {
      char body[96];
      make_error_json("method not allowed", body, sizeof(body));
      send_json(client, 405, String(body));
      return;
    }
    led.turn_on();
    char body[64];
    auto s = led_state_string();
    make_led_status_json(s, body, sizeof(body));
    send_json(client, 200, String(body));
    return;
  }
  if (path == HttpRoutes::LED_OFF) {
    if (method != "POST") {
      char body[96];
      make_error_json("method not allowed", body, sizeof(body));
      send_json(client, 405, String(body));
      return;
    }
    led.turn_off();
    char body[64];
    auto s = led_state_string();
    make_led_status_json(s, body, sizeof(body));
    send_json(client, 200, String(body));
    return;
  }
  if (path == HttpRoutes::LED_TOGGLE) {
    if (method != "POST") {
      char body[96];
      make_error_json("method not allowed", body, sizeof(body));
      send_json(client, 405, String(body));
      return;
    }
    led.toggle();
    char body[64];
    auto s = led_state_string();
    make_led_status_json(s, body, sizeof(body));
    send_json(client, 200, String(body));
    return;
  }
  if (path == HttpRoutes::LED_STATUS) {
    if (method != "GET") {
      char body[96];
      make_error_json("method not allowed", body, sizeof(body));
      send_json(client, 405, String(body));
      return;
    }
    char body[64];
    make_led_status_json(led_state_string(), body, sizeof(body));
    send_json(client, 200, String(body));
    return;
  }

  // Optional: root endpoint shows available routes
  if (path == "/") {
    if (method != "GET") {
      char body[96];
      make_error_json("method not allowed", body, sizeof(body));
      send_json(client, 405, String(body));
      return;
    }
    char body[128];
    make_routes_json(HttpRoutes::LED_ON, HttpRoutes::LED_OFF, HttpRoutes::LED_TOGGLE, HttpRoutes::LED_STATUS, body, sizeof(body));
    send_json(client, 200, String(body));
    return;
  }

  char body[96];
  make_error_json("unknown route", body, sizeof(body));
  send_json(client, 404, String(body));
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
    last_ip_print_ms = now;
  }

  WiFiClient client = server.available();
  if (!client) {
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
}
