#pragma once

#include <stdint.h>
#include "TempInterface.h"

#ifdef ARDUINO
#include <Arduino.h>

//! Arduino-backed TMP36 hardware reader using analogRead at configured resolution.
class TempHardware : public TempInterface {
public:
	//! Create a hardware temperature reader bound to the given analog pin.
	explicit TempHardware(uint8_t analog_pin)
		: analog_pin_(analog_pin) {}

	//! Configure the analog pin and ADC resolution (12-bit, 0..4095).
	void begin() override {
		pinMode(analog_pin_, INPUT);
		// UNO R4 WiFi effective ADC is 12-bit. Use explicit 12-bit range (0..4095).
		analogReadResolution(12);
	}

	//! Read temperature in Celsius using TMP36 transfer function.
	float read_celsius() override {
		//! Read raw ADC using configured resolution
		int raw = analogRead(analog_pin_);
		//! sensor is most stable at 5V so we use 5.0V
		constexpr float adc_full_scale_volts = 5.0f;
		constexpr float adc_max_counts = 4095.0f; // 12-bit
		float voltage = (static_cast<float>(raw) * adc_full_scale_volts) / adc_max_counts;
		//! TMP36: 500 mV at 0°C, 10 mV/°C slope
		return (voltage - 0.5f) * 100.0f;
	}

private:
	uint8_t analog_pin_;
};

#else
// Native fallback (unused in native monitor, but for interface symmetry)
class TempHardware : public TempInterface {
public:
	explicit TempHardware(uint8_t /*analog_pin*/) {}
	void begin() override {}
	float read_celsius() override { return 0.0f; }
};
#endif


