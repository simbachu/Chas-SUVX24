#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

const uint32_t numbers[][4] = {
	{
		0x41f59169,
		0x1d114114,
		0x1141f391,
		66
	},
	{
		0x0,
		0x1f80000,
		0x0,
		66
	},
	{
		0x0,
		0x1f80000,
		0x3fc000,
		66
	},
	{
		0x1f,
		0x80000f00,
		0x3fc000,
		66
	},
	{
		0x3fc2a42a,
		0x42a433c2,
		0x43fc204,
		66
	},
	{
		0x3fc08,
		0x1007f81,
		0x82107fc,
		66
	},
	{
		0x8004000,
		0x3fc0000,
		0x88088104,
		66
	},
	{
		0x10011816,
		0x380d001,
		0x1081f8,
		66
	},
	{
		0x11011,
		0x1101101,
		0x8208404,
		66
	},
	{
		0x400401f,
		0x80480480,
		0x4808a10e,
		66
	},
	{
		0x4004,
		0x403f80,
		0x40040040,
		66
	}
};


volatile bool irq_button {false};
uint8_t page { 0 };


void button ( )
{
  irq_button = true;
}


void setup() 
{
  attachInterrupt(digitalPinToInterrupt(3), button, FALLING);
  matrix.begin();
  matrix.loadSequence(numbers);
}

void loop() 
{
	if ( irq_button )
	{
		irq_button = false;
		page++;
		if ( page > 10 )
		{
			page = 0;
		}
	}
	
	matrix.renderFrame(page);
}
