#pragma once

#include <stdint.h>
#include "LedInterface.h"

#ifdef ARDUINO

class LedHardware : public LedInterface {
public:
	explicit LedHardware(uint8_t pin)
		: pin_(pin) {
		pinMode(pin_, OUTPUT);
		digitalWrite(pin_, LOW);
	}

	bool turn_on() override {
		digitalWrite(pin_, HIGH);
		return is_on();
	}

	bool turn_off() override {
		digitalWrite(pin_, LOW);
		return is_on();
	}

	bool toggle() override {
		int current = digitalRead(pin_);
		digitalWrite(pin_, current == HIGH ? LOW : HIGH);
		return is_on();
	}

	bool is_on() const override {
		return digitalRead(pin_) == HIGH;
	}

private:
	uint8_t pin_;
};

#else
// Native build without Arduino: emulate LED state.
class LedHardware : public LedInterface {
public:
	explicit LedHardware(uint8_t /*pin*/) {}
	bool turn_on() override { state_ = true; return state_; }
	bool turn_off() override { state_ = false; return state_; }
	bool toggle() override { state_ = !state_; return state_; }
	bool is_on() const override { return state_; }
private:
	bool state_ = false;
};
#endif

