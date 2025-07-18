
enum class LightStates
{
  RED,
  RED_YELLOW,
  YELLOW,
  GREEN
};


enum Lights: uint8_t
{
  RED_LED = 7,
  YELLOW_LED = 6,
  GREEN_LED = 5
};


enum LightTimes: uint32_t
{
  BUTTON_WAIT = 1000,
  TO_GREEN = 1500,
  TO_RED = 3000,
  GREEN = 5000
};


LightStates light_state;

volatile bool irq_button {false};

uint32_t transition_time {0};
uint32_t target_time {0};

bool button_waiting {false};


void button ( )
{
  if ( light_state == LightStates::RED )
  {
    irq_button = true;
  }
}


void setup() 
{
  pinMode(Lights::RED_LED, OUTPUT);
  pinMode(Lights::YELLOW_LED, OUTPUT);
  pinMode(Lights::GREEN_LED, OUTPUT);


  attachInterrupt(digitalPinToInterrupt(3), button, FALLING);
}

void loop() 
{
  switch ( light_state )
  {
    case LightStates::RED:
    digitalWrite(Lights::RED_LED, LOW);
    digitalWrite(Lights::YELLOW_LED, HIGH);
    digitalWrite(Lights::GREEN_LED, HIGH);

    if ( irq_button )
    {
      irq_button = false;
        if ( !button_waiting )
        {
          transition_time = millis();
          target_time = transition_time + LightTimes::BUTTON_WAIT;
          button_waiting = true;
        }
    }

    if ( button_waiting && millis() > target_time )
    {
      button_waiting = false;
      light_state = LightStates::RED_YELLOW;
      transition_time = millis();
      target_time = transition_time + LightTimes::TO_GREEN;
    }
    break;


    case LightStates::RED_YELLOW:
    digitalWrite(Lights::RED_LED, LOW);
    digitalWrite(Lights::YELLOW_LED, LOW);
    digitalWrite(Lights::GREEN_LED, HIGH);

    if ( millis() > target_time )
    {
      light_state = LightStates::GREEN;
      transition_time = millis();
      target_time = transition_time + LightTimes::GREEN;
    }
    break;


    case LightStates::GREEN:
    digitalWrite(Lights::RED_LED, HIGH);
    digitalWrite(Lights::YELLOW_LED, HIGH);
    digitalWrite(Lights::GREEN_LED, LOW);
    
    if ( millis() > target_time )
    {
      light_state = LightStates::YELLOW;
      transition_time = millis();
      target_time = transition_time + LightTimes::TO_RED;
    }
    break;

    case LightStates::YELLOW:
    digitalWrite(Lights::RED_LED, HIGH);
    digitalWrite(Lights::YELLOW_LED, LOW);
    digitalWrite(Lights::GREEN_LED, HIGH);
    
    if ( millis() > target_time )
    {
      light_state = LightStates::RED;
    }
    break;
  }
}