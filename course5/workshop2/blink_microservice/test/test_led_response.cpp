#include <unity.h>

#include "Http/Response.h"

void test_make_led_status_json_on(void) {
    char buffer[64];
    make_led_status_json("on", buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL_STRING("{\"status\":\"ok\",\"led\":\"on\"}", buffer);
}

void test_make_led_status_json_off(void) {
    char buffer[64];
    make_led_status_json("off", buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL_STRING("{\"status\":\"ok\",\"led\":\"off\"}", buffer);
}

void test_make_error_json_msg(void) {
    char buffer[64];
    make_error_json("unknown route", buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL_STRING("{\"status\":\"error\",\"message\":\"unknown route\"}", buffer);
}

void test_make_routes_json(void) {
    char buffer[128];
    make_routes_json("/a","/b","/c","/d", buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL_STRING("{\"status\":\"ok\",\"routes\":[\"/a\",\"/b\",\"/c\",\"/d\"]}", buffer);
}

#ifdef ARDUINO
#include <Arduino.h>
void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_make_led_status_json_on);
    RUN_TEST(test_make_led_status_json_off);
    RUN_TEST(test_make_error_json_msg);
    RUN_TEST(test_make_routes_json);
    UNITY_END();
}
void loop() {}
#else
int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_make_led_status_json_on);
    RUN_TEST(test_make_led_status_json_off);
    RUN_TEST(test_make_error_json_msg);
    RUN_TEST(test_make_routes_json);
    return UNITY_END();
}
#endif


