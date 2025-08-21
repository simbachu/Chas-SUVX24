#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <string_view>
#include <ArduinoJson.h>

// Builds: {"status":"ok","led":"<state>"}
inline size_t append_sv(char* out, size_t cap, size_t pos, std::string_view s) {
    if (out == nullptr || cap == 0 || pos >= cap) return pos;
    if (s.data() == nullptr) s = std::string_view("");
    size_t writable = cap - 1 - pos; // reserve space for NUL
    size_t to_copy = s.size() < writable ? s.size() : writable;
    if (to_copy > 0) {
        memcpy(out + pos, s.data(), to_copy);
        pos += to_copy;
    }
    return pos;
}

inline size_t append_lit(char* out, size_t cap, size_t pos, const char* lit) {
    if (out == nullptr || cap == 0 || pos >= cap) return pos;
    size_t len = strlen(lit);
    size_t writable = cap - 1 - pos;
    size_t to_copy = len < writable ? len : writable;
    if (to_copy > 0) {
        memcpy(out + pos, lit, to_copy);
        pos += to_copy;
    }
    return pos;
}

inline void terminate_json(char* out, size_t cap, size_t pos) {
    if (out == nullptr || cap == 0) return;
    if (pos >= cap) pos = cap - 1;
    out[pos] = '\0';
}

inline void make_led_status_json(std::string_view state, char* out_buffer, size_t out_buffer_size) {
    if (out_buffer == nullptr || out_buffer_size == 0) return;
    if (state.data() == nullptr) state = "unknown";
    StaticJsonDocument<96> doc;
    doc["status"] = "ok";
    doc["led"] = state;
    auto n = serializeJson(doc, out_buffer, out_buffer_size);
    if (n == 0 && out_buffer_size > 0) out_buffer[0] = '\0';
}

// Builds: {"status":"error","message":"<message>"}
inline void make_error_json(std::string_view message, char* out_buffer, size_t out_buffer_size) {
    if (out_buffer == nullptr || out_buffer_size == 0) return;
    if (message.data() == nullptr) message = "error";
    StaticJsonDocument<128> doc;
    doc["status"] = "error";
    doc["message"] = message;
    auto n = serializeJson(doc, out_buffer, out_buffer_size);
    if (n == 0 && out_buffer_size > 0) out_buffer[0] = '\0';
}

// Builds: {"status":"ok","routes":["r1","r2","r3","r4"]}
inline void make_routes_json(std::string_view r1, std::string_view r2, std::string_view r3, std::string_view r4,
                             char* out_buffer, size_t out_buffer_size) {
    if (out_buffer == nullptr || out_buffer_size == 0) return;
    auto sv_or_empty = [](std::string_view s) { return s.data() ? s : std::string_view(""); };
    r1 = sv_or_empty(r1); r2 = sv_or_empty(r2); r3 = sv_or_empty(r3); r4 = sv_or_empty(r4);
    StaticJsonDocument<192> doc;
    doc["status"] = "ok";
    JsonArray routes = doc.createNestedArray("routes");
    routes.add(r1);
    routes.add(r2);
    routes.add(r3);
    routes.add(r4);
    auto n = serializeJson(doc, out_buffer, out_buffer_size);
    if (n == 0 && out_buffer_size > 0) out_buffer[0] = '\0';
}


