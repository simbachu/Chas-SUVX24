namespace rgb_led 
{
  enum Pin : uint8_t
  {
    red = 4,
    green = 3,
    blue = 2
  };
  
  static const Pin Pins[] = { red, green, blue };
};


namespace Sound
{

  uint8_t speaker {6};

  enum Note : uint16_t
  {
    a3 = 220,
    b3 = 247,
    c4 = 262,
    d4 = 294,
    e4 = 330
  };

  static const Note Notes[] = { a3, b3, c4, d4, e4 };

};


void setup()
{
  for ( const auto c : rgb_led::Pins )
  {
    pinMode(c, OUTPUT);
  }

  pinMode(Sound::speaker, OUTPUT);
}


void loop()
{
  for ( const auto c : rgb_led::Pins )
  {
    tone( Sound::speaker, Sound::Notes[c]);
    digitalWrite(c, HIGH);
    delay(1000);
    digitalWrite(c, LOW);
    noTone(Sound::speaker);
    delay(50);
  }
}