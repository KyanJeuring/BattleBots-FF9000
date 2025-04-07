//Exemple 1- Blink
const int BUTTON_PIN = 2;//the number of the pushbutton pin
const int LED_RED = 13;//the number of the LED pin
int _buttonState = 0;//variable for reading the pushbutton status
bool _slowSpeed = true;//variable for the LED's speed status 
void setup() {
  //initialize the LED pin as an output
  pinMode(LED_RED , OUTPUT);
  //initialize the pushbutton as an input
  pinMode(BUTTON_PIN , INPUT);
}

void loop() {
  //read the state of the pushbutton value
  _buttonState = digitalRead(BUTTON_PIN);
  //check the state of the pushbutton value
  if(_buttonState == LOW) 
  {
    _slowSpeed = !_slowSpeed;
    
  }

 //check the value of the LED's speed
  if(_slowSpeed==false)
  {
    //slow blink
    digitalWrite(LED_RED , LOW);
    delay(500);//Wait for 500 milliseconds
    digitalWrite(LED_RED , HIGH);
    delay(500);//Wait for 500 milliseconds
  }
  else
 {
  //fast blink
    digitalWrite(LED_RED , LOW);
    delay(100);//Wait for 100 milliseconds
    digitalWrite(LED_RED , HIGH);
    delay(100);//Wait for 100 milliseconds
 }
}
