const int ledRed = 11;  // Pin number where the LED is connected
const int ledYellow= 12; // Pin for the yellow light
const int ledGreen= 13;  // Pin for the green light
const int buttonPin = 2;

int buttonState = 0;
int lastButtonState = 0;
bool sequenceRunning = false;

void setup() {
  pinMode(ledRed, OUTPUT);  // Set the LED pin as an output
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);//initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);

  digitalWrite(ledRed, HIGH);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledGreen, HIGH);
}

void loop() {
  
  buttonState = digitalRead (buttonPin);

  Red();

  if(buttonState == LOW && lastButtonState == HIGH)
  {
    sequenceRunning = true;
  }

  lastButtonState = buttonState;

  if (sequenceRunning)
  {
    digitalWrite(ledRed, LOW);
    delay(3000);
    digitalWrite (ledRed, HIGH);

    digitalWrite(ledGreen, LOW);
    delay(4000);
    digitalWrite (ledGreen, HIGH);

    digitalWrite(ledYellow, LOW);
    delay(1000);
    digitalWrite (ledYellow, HIGH);
    
    sequenceRunning = false;
  }
}

void Red()
{
digitalWrite(ledRed, LOW);
}

