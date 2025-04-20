#include "RTC.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;
RTCTime time(17, Month::APRIL, 2025, 10, 15, 00, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_ACTIVE);


volatile bool irq_time = false;
volatile bool irq_button = false;
bool rolling = true;
uint8_t last_roll { 0 };


const uint32_t animation_delay {200};


const uint32_t dice[][4] = {
	{
		0x600f,
		0x1f81f80,
		0xf0060000,
		animation_delay
	},
	{
		0x1801,
		0x80000001,
		0x80180000,
		animation_delay
	},
	{
		0xc00c00,
		0x600600,
		0x300300,
    animation_delay
	},
	{
		0x30c30c00,
		0x0,
		0x30c30c,
		animation_delay
	},
	{
		0x30c30c00,
		0x600600,
		0x30c30c,
		animation_delay
	},
	{
		0x19819800,
		0x1981980,
		0x198198,
		animation_delay
	}
};


void timer ( )
{
  irq_time = true;
}


void button ( )
{
  irq_button = true;
}


void setup() 
{
  attachInterrupt(digitalPinToInterrupt(3), button, FALLING);


  randomSeed(analogRead(A5));


  RTC.begin();
  RTC.setTime(time);


  RTC.setPeriodicCallback(timer, Period::N8_TIMES_EVERY_SEC);


  matrix.begin();
  matrix.loadSequence(dice);
}


void loop() 
{
  if ( irq_button )
  {
    irq_button = false;
    rolling = !rolling;
  }

  if ( irq_time && rolling )
  {
    irq_time = false;
    last_roll = random(0, 6);
  }

  matrix.renderFrame(last_roll);
}