const int LED_RED = 11;
const int LED_YELLOW = 12;
const int LED_GREEN = 13;
const int BUTTON_PIN = 2;

int buttonState = 0;
int lastButtonState = 0;
bool sequenceRunning = false;

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(LED_RED, HIGH); // Ensure default state is OFF
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_GREEN, HIGH);
}

void loop() 
{

  buttonState = digitalRead(BUTTON_PIN);

  Red();

  if (buttonState == LOW && lastButtonState == HIGH) {
    sequenceRunning = true; // Start sequence
  }
  
  lastButtonState = buttonState; 

  
  if (sequenceRunning) {
    digitalWrite(LED_RED, LOW);
    delay(3000);
    digitalWrite(LED_RED, HIGH);
    
    digitalWrite(LED_GREEN, LOW);
    delay(4000);
    digitalWrite(LED_GREEN, HIGH);
    
    digitalWrite(LED_YELLOW, LOW);
    delay(1000);
    digitalWrite(LED_YELLOW, HIGH);

    sequenceRunning = false; 
  }
}

void Red()
{
  digitalWrite(LED_RED, LOW); 
}

