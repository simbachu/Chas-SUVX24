
static const uint8_t speaker {7};

struct Note{
  static const uint16_t c4 {262};
  static const uint16_t d4 {294};
  static const uint16_t e4 {330};
};

struct Button
{
  static const uint8_t C { 2 };
  static const uint8_t D { 3 };
  static const uint8_t E { 4 };
};

void setup()
{
  pinMode( Button::C, INPUT );
  pinMode( Button::D, INPUT );
  pinMode( Button::E, INPUT );
  pinMode(speaker, OUTPUT);
  
}

void loop()
{
  if ( digitalRead( Button::C ) )
  {
  	tone(speaker, Note::c4);
  }
  else if ( digitalRead( Button::D ) )
  {
  	tone(speaker, Note::d4);
  }
  else if ( digitalRead( Button::E ) )
  {
  	tone(speaker, Note::e4);
  }
  else 
  {
      noTone(speaker);
  }
}