float ratio_from_analog ( int analog_input )
{
  if ( analog_input == 0 ) 
  {
    return 0.0;
  }
  else
  {
    return float(analog_input) / 1023.0;
  }
}


float gamma_corrected_ratio ( float ratio, float gamma = 2.2 )
{
  return pow ( ratio, gamma );
}


int duty_from_a ( int analog_input, float gamma = 2.2 )
{    
  return int(255.0 * gamma_corrected_ratio ( ratio_from_analog( analog_input ) ) );
}


int duty_from_ratio ( float ratio, float gamma = 2.2 )
{
  return int(255.0 * gamma_corrected_ratio ( ratio ) );
}

const uint8_t cold_led_pin { 3 };
const uint8_t warm_led_pin { 5 };


void setup()
{
  pinMode(cold_led_pin, OUTPUT);
  pinMode(warm_led_pin, OUTPUT);
}

void loop()
{
  const float brightness_ratio = { ratio_from_analog( analogRead( A0 ) ) };
  const float warmth_ratio = { 1.0 - ratio_from_analog( analogRead( A1 ) ) };

  analogWrite(warm_led_pin, duty_from_ratio( brightness_ratio * ( 1.0 - warmth_ratio ) ) );
  analogWrite(cold_led_pin, duty_from_ratio( brightness_ratio * warmth_ratio ) );
  delay(50);
}