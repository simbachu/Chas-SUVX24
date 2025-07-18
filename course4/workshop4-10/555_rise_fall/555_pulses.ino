constexpr float RESISTANCE_R2 {47000.0};
constexpr float CAPACITANCE_C1 {0.00000179};

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long highTime = pulseIn(3, HIGH); // µs
  unsigned long lowTime = pulseIn(3, LOW);   // µs
  
  uint32_t period = (highTime + lowTime) / 1000; // ms
  uint32_t t1 = highTime / 1000; // ms
  float duty = float(t1) / float(period) * 100;
  float resistance_r1 = (float(period) / 1000.0) / (0.693 * CAPACITANCE_C1) - 2 * RESISTANCE_R2;

  if (resistance_r1 >= 0) 
  {
    String message = "T: " + String(period) + "ms\tT1: " + String(t1) + "ms\nDuty cycle: " + String(duty) + "%\nR1 = " + String(resistance_r1);
    Serial.println(message);
  }
  delay(100);
}