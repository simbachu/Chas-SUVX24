#include <Arduino.h>

static const uint8_t kSensorPin = 4;

//! @brief Flag to indicate if the sensor has been tripped
volatile bool tripped {false};

//! @brief Checks if the sensor is triggered
//! @code{.cpp}
//! attachInterrupt(digitalPinToInterrupt(kSensorPin), sensor_triggered, HIGH);
//! // This will set the tripped flag to true when the sensor is tripped
//! @endcode
void sensor_triggered();

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }
    pinMode(kSensorPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(kSensorPin), sensor_triggered, HIGH);
}

void loop() {
    if (tripped) {
        Serial.println("Sensor triggered");
        // Reset the flag to false
        tripped = false;
    }
}

void sensor_triggered() {
    tripped = true;
}
