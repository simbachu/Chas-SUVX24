#pragma once

#include "LedInterface.h"

class TestLed : public LedInterface {
public:
	TestLed() : state_on_(false) {}

	bool turn_on() override {
		state_on_ = true;
		return state_on_;
	}

	bool turn_off() override {
		state_on_ = false;
		return state_on_;
	}

	bool toggle() override {
		state_on_ = !state_on_;
		return state_on_;
	}

	bool is_on() const override {
		return state_on_;
	}

	// Helpers for tests
	void set_state(bool on) { state_on_ = on; }

private:
	bool state_on_;
};


