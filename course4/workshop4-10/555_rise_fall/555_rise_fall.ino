uint32_t last_rise {0};
uint32_t t1 {0};
uint32_t period {0};

float duty;
bool msg {false};

volatile bool fell {false};
volatile bool rose {false};


constexpr float RESISTANCE_R2 {47000.0};
constexpr float CAPACITANCE_C1 {0.00000179}; // 1.79 µF

void signal_change ()
{


  if ( digitalRead(3) == HIGH )
  {
    rose = true;
  }


  else 
  {
    fell = true;
  }


}


void setup() 
{
  attachInterrupt(digitalPinToInterrupt(3), signal_change, CHANGE);

  Serial.begin(9600);
}


void loop() 
{


  if ( rose )
  {
    period = millis() - last_rise;
    last_rise = millis();

    duty = float(t1) / float(period) * 100;
    msg = true;
    rose = false;
  }


  if ( fell )
  {
    t1 = millis() - last_rise;
    fell = false;
  }


  if ( msg )
  {
    float resistance_r1 = ( float(period) / 1000.0 ) / ( 0.693 * CAPACITANCE_C1 ) - 2 * RESISTANCE_R2;
    String message {"T: " + String(period) + "ms" + '\t' + "T1: " + String(t1) + "ms" + '\n' + "Duty cycle: " + String(duty) + "%" + '\n' + "R1 = " + resistance_r1};

    Serial.println(message);
    msg = false;
  }


}
