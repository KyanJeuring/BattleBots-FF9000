const int MOTOR_A_1 = 13;  // Motor A control pin 1
const int MOTOR_A_2 = 12;  // Motor A control pin 2
const int MOTOR_B_1 = 11;  // Motor B control pin 1
const int MOTOR_B_2 = 10;
const int LED_GREEN = 6;
const int LED_RED = 4;
const int LED_YELLOW = 5;
const int BUTTON_PIN = 2;

int _buttonState = 0;
int _lastButtonState = 0;
bool _sequenceRunning = false;

void setup() {
  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
}

void loop() 
{
  _buttonState = digitalRead(BUTTON_PIN);

  if (_buttonState == LOW && _lastButtonState == HIGH) {
    _sequenceRunning = true; // Start sequence
  }
  
  _lastButtonState = _buttonState; 

  
  if (_sequenceRunning) {
    robotForward();
    delay(6500);
    robotRight();
    delay(700);
    robotForward();
    delay(3000);
    robotLeft();
    delay(1700);
    robotForward();
    delay(3000);
    robotLeft();
    delay(700);
    robotForward();
    delay(4000);
    robotBackward();
    delay(1000);
    robotStop();
    delay(2000);

    _sequenceRunning = false; 
  }
}

void rightWheelForward() //Setting the Right wheel to go Forward
{
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 255); 
}

void rightWheelBackward() //Setting the Right Wheel to go Backward
{
  analogWrite(MOTOR_B_1, 255);
  analogWrite(MOTOR_B_2, 0); 
}

void leftWheelForward() //Setting the Left Wheel to go Forward
{
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 255);
}

void leftWheelBackward() //Setting the Left Wheel to go Backward
{
  analogWrite(MOTOR_A_1, 255);
  analogWrite(MOTOR_A_2, 0);
}

void robotForward()  //Settings both the left and right wheel to go forward
{
  leftWheelForward();
  rightWheelForward();
  greenLEDOn();
}

void robotBackward()
{
  leftWheelBackward();
  rightWheelBackward();
  yellowLEDOn();
}

void robotStop()
{
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 0);

  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 0);

  redLEDOn();
}

void robotRight()
{
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 255);

  analogWrite(MOTOR_B_1, 126);
  analogWrite(MOTOR_B_2, 0);
}

void robotLeft()
{
  analogWrite(MOTOR_A_1, 126);
  analogWrite(MOTOR_A_2, 0);

  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 255);
}

void greenLEDOn()
{
  digitalWrite(LED_GREEN, LOW);   // Green LED on, Red LED off
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, HIGH);
}

void redLEDOn()
{
  digitalWrite(LED_GREEN, HIGH);   // Red LED on, Green LED off
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, HIGH);
}

void yellowLEDOn()
{
  digitalWrite(LED_GREEN, HIGH);   // Red LED on, Green LED off
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, LOW);
}