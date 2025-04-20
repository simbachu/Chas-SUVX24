#include <cstdint>

// C++ code
//

struct Distance_Sensor{
const uint8_t echo {2};
const uint8_t trig {3};


void setup()
{
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);
}


const float poll() const
{
        float duration_us, distance_cm;
        
        
        digitalWrite(trig, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig, LOW);
        
        duration_us = pulseIn(echo, HIGH);
        
        distance_cm = 0.017 * duration_us;
        
        return distance_cm;
    }
};
  
  
  
  struct Alert{
    
    enum struct Tempo : uint8_t
    {
     Low = 255,
     Medium = 128,
     High = 64
    };
    
    const uint8_t LSD_speaker {10};
    const uint8_t LSD_light {11};
    const uint16_t tone {131};
  
    bool active {false};
    uint8_t timer {0};
    Tempo time_step = Tempo::Low;
    
    
    void setup(){
      pinMode (LSD_speaker, OUTPUT);
      pinMode (LSD_light, OUTPUT);
    }
    
    
    void play_signal () 
    {
      digitalWrite(LSD_light, LOW);
      tone(LSD_speaker, tone);
    }
    
    
    void stop_signal () 
    {
      digitalWrite(LSD_light, HIGH);
      noTone(LSD_speaker);
    }
    
    
    void set_tempo (Tempo new_tempo)
    {
      
    }
    
    
    void() run() 
    {
      while (active){
        play_signal();
        
        
      
    }
    
  };
  
  Alert alert;
  Distance_Sensor distance_sensor;
  
  void setup()
  {
    alert.setup();
    distance_sensor.setup();
  }
  
  
  void loop()
  {
    const uint8_t measured_distance 
    {
      distance_sensor.measure();
    };
    
    
    
    if ( measured_distance <= 20 ) 
    { 
    }
    else if ( measured_distance < 40 ) 
    {
    }
    else if ( measured_distance < 60 ) 
    {
    }
    else if ( measured_distance < 80 ) 
    {
    }
    else if ( measured_distance < 100 ) 
    {
    }
    else 
    { 
    }
   
  }