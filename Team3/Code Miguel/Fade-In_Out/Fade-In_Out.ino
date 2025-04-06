int ledYellow = 11;
int brightness = 0;

void setup() 
{
  pinMode(ledYellow, OUTPUT);
}

void loop() 
{
  for (brightness = 0; brightness <= 255; brightness += 5)
  {
    analogWrite(ledYellow, brightness);
    delay(30);
  }

  for (brightness = 255; brightness >= 0; brightness -= 5)
  {
    analogWrite(ledYellow, brightness);
    delay(30);
  }
}
