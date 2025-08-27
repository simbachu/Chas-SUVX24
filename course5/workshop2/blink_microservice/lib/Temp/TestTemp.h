#ifndef TEST_TEMP_H
#define TEST_TEMP_H

#include "TempInterface.h"

class TestTemp : public TempInterface {
public:
	void begin() override {}
	float read_celsius() override { return current_c_; }
	void set_celsius(float c) { current_c_ = c; }

private:
	float current_c_ = 0.0f;
};

#endif // TEST_TEMP_H
