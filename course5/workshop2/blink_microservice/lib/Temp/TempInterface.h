#pragma once

#include <stdint.h>

class TempInterface {
public:
	virtual ~TempInterface() {}
	virtual void begin() = 0;
	virtual float read_celsius() = 0;
};


