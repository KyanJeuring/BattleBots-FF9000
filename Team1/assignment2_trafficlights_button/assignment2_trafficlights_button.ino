const int BUTTON_PIN = 2;//the number of the pushbutton pin
const int LED_RED = 11;//the number of the red LED pin
const int LED_YELLOW = 12;//the number of the yellow LED pin
const int LED_GREEN = 13;//the number of the green LED pin
//int _buttonState = 0;//variable for reading the pushbutton status
void setup() {
  //initialize the LED pin as an output
  pinMode(LED_RED , OUTPUT);
  //initialize the LED pin as an output
  pinMode(LED_YELLOW , OUTPUT);
  //initialize the LED pin as an output
  pinMode(LED_GREEN , OUTPUT);
  //initialize the pushbutton as an input
  pinMode(BUTTON_PIN , INPUT);
  //in the bigining only the red LED is turned on and the other 2 are off 
  digitalWrite(LED_RED , LOW);
  digitalWrite(LED_GREEN , HIGH);
  digitalWrite(LED_YELLOW , HIGH);
}

void loop() {
  //read the state of the pushbutton value
  int _buttonState = digitalRead(BUTTON_PIN);
  if(_buttonState == LOW) 
  {  //turn off the red LED
    digitalWrite(LED_RED , HIGH);
    // turn on the green LED , wait ,and then turn it back of
    digitalWrite(LED_GREEN , LOW);
    delay(4000);//Wait for 4 seconds
    digitalWrite(LED_GREEN , HIGH);
    //// turn on the yellow LED , wait and then turn it back of
    digitalWrite(LED_YELLOW , LOW);
    delay(1000);//Wait for 1 second
    digitalWrite(LED_YELLOW , HIGH);
    //turn the red LED back on
    digitalWrite(LED_RED , LOW);
  }
}
