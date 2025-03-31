// Pin Definitions
const int MOTOR_A_1 = 13;  // Motor A control pin 1
const int MOTOR_A_2 = 12;  // Motor A control pin 2
const int MOTOR_B_1 = 11;  // Motor B control pin 1
const int MOTOR_B_2 = 10;  // Motor B control pin 2
const int LED_GREEN = 6;   // Green LED pin
const int LED_RED = 4;     // Red LED pin
const int LED_YELLOW = 5;  // Yellow LED pin
const int BUTTON_PIN = 2;  // Button pin for triggering sequence
const int HALF_TURN_TIME = 800; // Time in milliseconds for 90° turns (adjustable)
const int FULL_TURN_TIME = 1500; // Time in milliseconds for 90° turns (adjustable)

// Button state variables
int _buttonState = 0;
int _lastButtonState = 0;
bool _sequenceRunning = false;

void setup() {
  // Set motor pins as output
  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  
  // Set LED pins as output
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  
  // Set button pin as input
  pinMode(BUTTON_PIN, INPUT);

  // Initialize LEDs (Green and Red on)
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
}

void loop() {
  // Read button state
  _buttonState = digitalRead(BUTTON_PIN);

  // Check if button is pressed to start sequence
  if (_buttonState == LOW && _lastButtonState == HIGH) {
    _sequenceRunning = true; // Start the robot sequence
  }
  
  // Save button state for next loop iteration
  _lastButtonState = _buttonState; 

  // If the sequence is running, perform robot movements
  if (_sequenceRunning) {
    // Robot movement sequence
    robotForward();
    delay(6500);
    robotRight();
    delay(HALF_TURN_TIME);
    robotForward();
    delay(3000);
    robotLeft();
    delay(FULL_TURN_TIME);
    robotForward();
    delay(3000);
    robotLeft();
    delay(HALF_TURN_TIME);
    robotForward();
    delay(4000);
    robotBackward();
    delay(1000);
    robotStop();
    delay(2000);

    // Stop the sequence after completing all movements
    _sequenceRunning = false; 
  }
}

// Motor Control Functions

// Right wheel forward
void rightWheelForward() {
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 255); 
}

// Right wheel backward
void rightWheelBackward() {
  analogWrite(MOTOR_B_1, 255);
  analogWrite(MOTOR_B_2, 0); 
}

// Left wheel forward
void leftWheelForward() {
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 255);
}

// Left wheel backward
void leftWheelBackward() {
  analogWrite(MOTOR_A_1, 255);
  analogWrite(MOTOR_A_2, 0);
}

// Robot Movement Control Functions

// Move robot forward (both wheels forward)
void robotForward() {
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 255);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 255);
}

// Move robot backward (both wheels backward)
void robotBackward() {
  analogWrite(MOTOR_A_1, 255);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_2, 0);
  analogWrite(MOTOR_B_1, 255);
}

// Stop the robot (set all motors to 0)
void robotStop() {
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 0);
}

// Turn robot right (left wheel forward, right wheel backward)
void robotRight() {
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 255);
  analogWrite(MOTOR_B_1, 126);
  analogWrite(MOTOR_B_2, 0);
}

// Turn robot left (right wheel forward, left wheel backward)
void robotLeft() {
  analogWrite(MOTOR_A_1, 126);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 255);
}


