#include <unity.h>

#include "Led/TestLed.h"

void test_initial_state_off() {
    TestLed led;
    TEST_ASSERT_FALSE(led.is_on());
}

void test_turn_on_sets_state() {
    TestLed led;
    bool state = led.turn_on();
    TEST_ASSERT_TRUE(state);
    TEST_ASSERT_TRUE(led.is_on());
}

void test_turn_off_sets_state() {
    TestLed led;
    led.turn_on();
    bool state = led.turn_off();
    TEST_ASSERT_FALSE(state);
    TEST_ASSERT_FALSE(led.is_on());
}

void test_toggle_flips_state() {
    TestLed led;
    TEST_ASSERT_FALSE(led.is_on());
    bool s1 = led.toggle();
    TEST_ASSERT_TRUE(s1);
    TEST_ASSERT_TRUE(led.is_on());
    bool s2 = led.toggle();
    TEST_ASSERT_FALSE(s2);
    TEST_ASSERT_FALSE(led.is_on());
}

void test_set_state_helper() {
    TestLed led;
    led.set_state(true);
    TEST_ASSERT_TRUE(led.is_on());
    led.set_state(false);
    TEST_ASSERT_FALSE(led.is_on());
}

#ifdef ARDUINO
#include <Arduino.h>
void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_off);
    RUN_TEST(test_turn_on_sets_state);
    RUN_TEST(test_turn_off_sets_state);
    RUN_TEST(test_toggle_flips_state);
    RUN_TEST(test_set_state_helper);
    UNITY_END();
}
void loop() {}
#else
int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_off);
    RUN_TEST(test_turn_on_sets_state);
    RUN_TEST(test_turn_off_sets_state);
    RUN_TEST(test_toggle_flips_state);
    RUN_TEST(test_set_state_helper);
    return UNITY_END();
}
#endif


