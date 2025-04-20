// C++ code
//

struct Distance_Sensor
{
  const uint8_t echo {8};
  const uint8_t trig {9};
  
  void setup()
  {
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);
  }

  const uint8_t measure() const{
    float duration_us, distance_cm;
    
    
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    
    duration_us = pulseIn(echo, HIGH);
    
    distance_cm = 0.017 * duration_us;
    
    return distance_cm;
  }
};

struct Lightbulb
{
  const uint8_t LSD_control {11};
  
  void setup() const
  {
    pinMode(LSD_control, OUTPUT);
  }
  
  void set_brightness(uint8_t percent) const
  {
    float output {255.0};
    float percentage_decimal {percent};
    float reduction {255.0};
    percentage_decimal = percent / 100.0;
    reduction *= percentage_decimal;
    output -= reduction;
    
    analogWrite( LSD_control, output);
  }
};

Lightbulb lightbulb;
Distance_Sensor distance_sensor;

void setup()
{
  distance_sensor.setup();
  lightbulb.setup();
}

void loop()
{
  const uint8_t measured_distance 
  {
    distance_sensor.measure()
  };
  
  
  
  if ( measured_distance <= 20 ) 
  {
    lightbulb.set_brightness(100);  
  }
  else if ( measured_distance < 40 ) 
  {
    lightbulb.set_brightness(80);  
  }
  else if ( measured_distance < 60 ) 
  {
    lightbulb.set_brightness(60);  
  }
  else if ( measured_distance < 80 ) 
  {
    lightbulb.set_brightness(40);  
  }
  else if ( measured_distance < 100 ) 
  {
    lightbulb.set_brightness(20);  
  }
  else {
    lightbulb.set_brightness(0);  
  }
 
}