//! @brief Controls a LED

class LEDController
{

#ifndef ARDUINO
#define OUTPUT 0
#define pinMode(pin, OUTPUT)
#define digitalWrite(pin, state)
#endif
  private:
    int pin;
    bool state;

  public:

    //! @brief Constructor for LEDController
    //! @param _ledPin The pin number of the LED
    //! @pre The pin parameter must be a constant defined in Arduino.h
    //! @post The pin is set to OUTPUT
    //! @note Do not set a communication pin as the parameter.
    LEDController(int _ledPin) : pin(_ledPin), state(false)
    {
      pinMode(pin, OUTPUT);
    }
    //! @brief Toggles the state of the LED
    //! @post The state of the LED is set to the opposite of the prior state
    void toggle()
    {
      state = !state;
      digitalWrite(pin, state);
    }
}; //! class LEDController