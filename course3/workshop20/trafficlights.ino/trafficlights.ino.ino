// C++ code
//
struct Digital{
    enum Pins{
      red_light_control = 2,    //HDO
      yellow_light_control = 3, //HDO
      green_light_control = 4   //HDO
    };
};
  

void setup()
{
  pinMode(Digital::red_light_control, OUTPUT);
  pinMode(Digital::yellow_light_control, OUTPUT);
  pinMode(Digital::green_light_control, OUTPUT);
  digitalWrite(Digital::red_light_control, HIGH);
  digitalWrite(Digital::yellow_light_control, LOW);
  digitalWrite(Digital::green_light_control, LOW);
}

void loop()
{
  digitalWrite(Digital::red_light_control, HIGH);
  delay(2000);
  digitalWrite(Digital::red_light_control, LOW);
  digitalWrite(Digital::yellow_light_control, HIGH);
  delay(2000);
  digitalWrite(Digital::yellow_light_control, LOW);
  digitalWrite(Digital::green_light_control, HIGH);
  delay(2000);
  digitalWrite(Digital::green_light_control, LOW);
  digitalWrite(Digital::yellow_light_control, HIGH);
  delay(2000);
  digitalWrite(Digital::yellow_light_control, LOW);
}