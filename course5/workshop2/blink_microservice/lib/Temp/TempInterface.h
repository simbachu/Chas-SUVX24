#ifndef TEMP_INTERFACE_H
#define TEMP_INTERFACE_H

#include <stdint.h>

class TempInterface {
public:
	virtual ~TempInterface() {}
	virtual void begin() = 0;
	virtual float read_celsius() = 0;
};

#endif // TEMP_INTERFACE_H
