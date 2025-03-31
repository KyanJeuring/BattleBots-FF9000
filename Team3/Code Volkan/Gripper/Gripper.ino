// --- Pin Definitions ---
// Define pin numbers for various components of the robot
#define TRIGPIN 7  // TRIG pin to trigger the ultrasonic sensor
#define ECHOPIN 8  // ECHO pin to receive the echo from ultrasonic sensor
#define GRPIN 5    // Gripper control pin
#define IN1 12     // Motor driver pin for Motor 1 (Forward direction)
#define IN2 13     // Motor driver pin for Motor 1 (Backward direction)
#define IN3 10     // Motor driver pin for Motor 2 (Forward direction)
#define IN4 11     // Motor driver pin for Motor 2 (Backward direction)
#define BUTTON 4   // Pin connected to the button to start the sequence

// --- Constants ---
// These constants are used to define fixed values throughout the program
#define GRIPPER_OPEN 1600  // Pulse width to open gripper
#define GRIPPER_CLOSE 990  // Pulse width to close gripper
#define MOVE_TIME 2500     // Time to move the robot forward (in milliseconds)
#define OBJECT_THRESHOLD 15  // Distance threshold to detect the object (in cm)

// --- Global Variables ---
// These variables are used to store sensor readings and states of various components
long duration;  // Variable to store the duration of the ultrasonic sensor echo
int distance;   // Variable to store the calculated distance from the ultrasonic sensor

int _buttonState = 0;           // Variable to store the current button state (HIGH or LOW)
int _lastButtonState = 0;       // Variable to store the last button state to detect state changes
bool _sequenceRunning = false;  // Flag to indicate if the robot's sequence should run

unsigned long currentTime;      // Variable to store the current time (in milliseconds)
unsigned long previousTime = 0; // Variable to store the last time the gripper was operated
bool conePickedUp = false;      // Flag to track whether the cone has been picked up (replace with actual condition)

bool gripperState = false;      // Gripper state: false = closed, true = open

// --- Setup Function ---
// The setup function runs once at the start of the program to initialize the robot's state
void setup() {
  Serial.begin(9600);

  // Set pin modes for the various components (inputs and outputs)
  pinMode(TRIGPIN, OUTPUT);  // Set TRIG pin as output
  pinMode(ECHOPIN, INPUT);   // Set ECHO pin as input
  pinMode(GRPIN, OUTPUT);    // Set gripper pin as output
  pinMode(BUTTON, INPUT);    // Set button pin as input
  pinMode(IN1, OUTPUT);      // Set Motor 1 direction pin as output
  pinMode(IN2, OUTPUT);      // Set Motor 1 reverse direction pin as output
  pinMode(IN3, OUTPUT);      // Set Motor 2 direction pin as output
  pinMode(IN4, OUTPUT);      // Set Motor 2 reverse direction pin as output

  digitalWrite(GRPIN, LOW);  // Initialize gripper in the closed position (LOW = closed)
}

// --- Main Loop ---
// The loop function runs continuously after setup() and controls the robot's actions
void loop() {
  controlGripperBasedOnPickup();

  _buttonState = digitalRead(BUTTON);  // Read the current button state (HIGH or LOW)

  // Check if the button was pressed (transition from HIGH to LOW)
  if (_buttonState == LOW && _lastButtonState == HIGH) {
    _sequenceRunning = true;  // Start the robot's sequence when the button is pressed
  }

  // Store the button state for the next iteration of the loop
  _lastButtonState = _buttonState;

  // If the sequence is running, execute the sequence function
  if (_sequenceRunning) {
    gripper(GRIPPER_OPEN);
    Function1();  // Call the function that controls the robot's movements
    _sequenceRunning = false;  // Reset the sequence flag after the sequence runs
  } else if (!_sequenceRunning)
  {
    gripper(GRIPPER_CLOSE);
  }

  // Call the function to control the gripper based on the pickup status
    // Ensure the gripper operates based on the robot's state
}

// --- Gripper Control Function ---
// This function controls the gripper by sending a pulse to open or close it
void gripper(int pulse) {
    static unsigned long timer;
    static int lastPulse;

    Serial.print("Gripper function called with pulse: ");
    Serial.println(pulse);  // Print the pulse width being sent

    if (millis() > timer) {
        if (pulse > 0) { 
            lastPulse = pulse;
        } else {
            pulse = lastPulse;
        }

        Serial.println("Activating gripper...");
        digitalWrite(GRPIN, HIGH);
        delayMicroseconds(pulse);
        digitalWrite(GRPIN, LOW);
        Serial.println("Gripper pulse sent.");

        timer = millis() + 20;
    }
}

// --- Motor Movement Functions ---
// Function to move the robot forward
void moveForward() {
  // Motor control: Set both motors to move the robot forward
  analogWrite(IN1, 255);  // Set Motor 1 to move forward (maximum speed)
  analogWrite(IN2, 0);    // Set Motor 1 to stop reverse direction
  analogWrite(IN3, 255);  // Set Motor 2 to move forward (maximum speed)
  analogWrite(IN4, 0);    // Set Motor 2 to stop reverse direction
}

// Function to stop the robot's motors
void stopMotors() {
  // Motor control: Stop both motors
  analogWrite(IN1, 0);    // Stop Motor 1
  analogWrite(IN2, 0);    // Stop Motor 1 reverse direction
  analogWrite(IN3, 0);    // Stop Motor 2
  analogWrite(IN4, 0);    // Stop Motor 2 reverse direction
}

// --- Gripper Control Based on Pickup Status ---
// This function controls the gripper depending on whether the cone is picked up or not
void controlGripperBasedOnPickup() {
  currentTime = millis();  // Store the current time in milliseconds

  if (conePickedUp) {  // If the cone has been picked up
    // If enough time has passed (500ms interval), check and close the gripper if needed
    if (currentTime - previousTime >= 500) {
      previousTime = currentTime;  // Update the previous time for the next check
      if (!gripperState) {  // If the gripper is closed, do nothing
        gripper(GRIPPER_CLOSE);  // Close the gripper (pulse width for close)
        gripperState = false;    // Update the gripper state to closed
      }
    }
  } else {  // If the cone is not picked up
    // If enough time has passed (500ms interval), check and open the gripper if needed
    if (currentTime - previousTime >= 500) {
      previousTime = currentTime;  // Update the previous time for the next check
      if (gripperState) {  // If the gripper is already open, skip opening it again
        return;  // Skip opening if already open
      }
      gripper(GRIPPER_OPEN);  // Open the gripper (pulse width for open)
      gripperState = true;    // Update the gripper state to open
    }
  }
}

// --- Sequence Function ---
// Function to perform a sequence of actions for the robot (gripper and movement)
void Function1() {
  controlGripperBasedOnPickup();

  gripper(GRIPPER_OPEN);  // Open the gripper to prepare for picking up the object
  delay(1000);  // Wait for 1 second (time for the gripper to open)

  gripper(GRIPPER_CLOSE);  // Close the gripper to grab the object
  delay(1000);  // Wait for 1 second (time for the gripper to close)

  gripper(GRIPPER_OPEN);  // Open the gripper again (to simulate releasing the object)
  moveForward();  // Move the robot forward for a predefined time
  delay(MOVE_TIME);  // Wait for the robot to move forward (defined movement time)

  gripper(GRIPPER_CLOSE);  // Close the gripper again to simulate grabbing the object
  delay(MOVE_TIME);  // Wait for another predefined time while the robot is moving

  stopMotors();  // Stop the motors after the sequence is completed
}
