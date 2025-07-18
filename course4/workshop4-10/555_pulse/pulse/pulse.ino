constexpr float RESISTANCE_R2 {47000.0};
constexpr float CAPACITANCE_C1 {0.00000179};

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  uint32_t time_t1 = pulseIn(3, HIGH) / 1000;
  uint32_t time_t2 = pulseIn(3, LOW) / 1000;
  uint32_t period = (time_t1 + time_t2);
  float duty = float(time_t1) / float(period) * 100;
  float resistance_r1 = (float(period) / 1000.0) / (0.693 * CAPACITANCE_C1) - 2 * RESISTANCE_R2;

  if (resistance_r1 >= 0) 
  {
    String message = "T: " + String(period) + "ms\tT1: " + String(time_t1) + "ms\nDuty cycle: " + String(duty) + "%\nR1 = " + String(resistance_r1);
    Serial.println(message);
  }
}