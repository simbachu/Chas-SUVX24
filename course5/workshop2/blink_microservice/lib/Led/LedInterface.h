#pragma once

class LedInterface {
public:
	virtual ~LedInterface() {}
	virtual bool turn_on() = 0;
	virtual bool turn_off() = 0;
	virtual bool toggle() = 0;
	virtual bool is_on() const = 0;
};


