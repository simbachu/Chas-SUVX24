// We include the Arduino library to use the built-in functions. In Arduino IDE,
// this is done automatically, but in other IDEs you have to include it manually.
#include <Arduino.h>

// Variable for keeping track of time.
// We use this to keep track of the last time the callback function was called.
uint32_t last_time = 0;

// Forward declarations
// We have to tell the compiler that these functions exist, so it can compile the code.
void blink_led();
void timer();

// Setup function
// This is an overload of a pre-existing function in the Arduino library,
// which is why it doesn't need a forward declaration.
void setup() {
    // We set the LED_BUILTIN pin as an output. It is the built-in LED on the board.
    pinMode(LED_BUILTIN, OUTPUT);
    // We call the timer function to start the timer.
    timer();
}

// Loop function
// This is an overload of a pre-existing function in the Arduino library,
// which is why it doesn't need a forward declaration.
void loop() {
    // We call the timer function every loop to advance the time
    timer();
    // We can busy wait a little bit to save power.
    delay(10);
}

// Timer function that procs every second
void timer() {
    // If the time has passed a second, we call the function that toggles the LED
    if (millis() - last_time > 1000) {
        // We update the last time we toggled the LED using the timer
        last_time = millis();
        // We call the function that toggles the LED
        blink_led();
    }
}

// Function that toggles the built-in LED
// As a separate function, we can reuse it, if we want to toggle on some other condition.
void blink_led() {
    // We toggle the LED by toggling the digitalRead of the LED_BUILTIN pin
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
