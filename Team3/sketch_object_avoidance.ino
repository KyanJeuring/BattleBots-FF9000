// --- Pin Definitions ---
const int TRIGPIN = 7;  // TRIG pin to trigger the ultrasonic sensor
const int ECHOPIN = 8;  // ECHO pin to receive the signal from the ultrasonic sensor
long _duration;         // Variable to store the pulse duration from the sensor
int _distance;          // Variable to store the calculated distance in centimeters

// Motor pins
const int MOTOR_A_1 = 13;  // Motor A control pin 1
const int MOTOR_A_2 = 12;  // Motor A control pin 2
const int MOTOR_B_1 = 11;  // Motor B control pin 1
const int MOTOR_B_2 = 10;  // Motor B control pin 2

// --- Constants & Thresholds ---
const int OBSTACLE_THRESHOLD = 15;  // Threshold to detect obstacles (in cm)
const int TURN_TIME = 600;          // Time in milliseconds for 90° turns (adjustable)
const int FORWARD_TIME_1 = 500;   // Time to move forward the first distance (short distance)
const int FORWARD_TIME_2 = 800;   // Time to move forward the second distance (long distance)

// Enum(enumeration) to define turn directions
//NONE: Represents no turn.
//LEFT: Represents a left turn.
//RIGHT: Represents a right turn.
enum TurnDirection {
  NONE,
  LEFT,
  RIGHT
};

// Global variable to track last avoidance maneuver
TurnDirection lastAvoidanceTurn = NONE;

// Function prototypes
void robotForward();
void robotRight();
void robotLeft();
void stopMotors();
int getDistance();
void avoidObstacle();
void setLastAvoidance(TurnDirection direction);

// --- Setup Function ---
void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging (9600 baud rate)

  // Set up the ultrasonic sensor pins
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  // Set up motor control pins
  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);

  stopMotors();  // Ensure motors are stopped initially
}

// --- Main Loop Function ---
void loop() {
  // Get front distance
  _distance = getDistance();  // Measure distance with the front sensor

  // Output the distance for debugging (can be removed for faster performance)
  // Serial.print("Front Distance: ");
  // Serial.print(_distance);
  // Serial.println(" cm");

  // Step 1: Always move forward
  robotForward();  // Always keep moving forward

  // Step 2: If obstacle detected, stop and try to move around
  if (_distance <= OBSTACLE_THRESHOLD) {
    stopMotors();  // Stop the robot if an obstacle is detected
    Serial.println("Obstacle detected!");

    avoidObstacle();  // Try to avoid the obstacle
  }

  delay(50);  // Reduced delay to make loop faster (was previously 100)
}

// --- Sensor Functions ---
// Function to measure distance using the ultrasonic sensor
int getDistance() {
  long duration;
  int distance;

  // Send a 10-microsecond pulse to trigger the sensor
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  // Measure the duration of the pulse received on the ECHO pin
  duration = pulseIn(ECHOPIN, HIGH);

  // Calculate the distance in centimeters
  distance = duration * 0.0343 / 2;  // Speed of sound is 0.0343 cm/μs, divide by 2 for round trip
  return distance;
}

// --- Motor Control Functions ---
// Function to move the robot forward
void robotForward() {
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 255);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 255);
}

// Function to stop all motors
void stopMotors() {
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 0);
}

// --- Turning Functions ---
// Function to turn the robot right by ≈ 90º
void robotRight() {
  analogWrite(MOTOR_A_1, 0);    
  analogWrite(MOTOR_A_2, 255);  // Left Wheel moves forward
  analogWrite(MOTOR_B_1, 126);  // Right Wheel moves backward at a lower speed
  analogWrite(MOTOR_B_2, 0);  

  delay(TURN_TIME);  // Delay for the turn
  stopMotors();      // Stop after the turn
}

// Function to turn the robot left ≈ 90º
void robotLeft() {
  analogWrite(MOTOR_A_1, 126);  //Left Wheel moves backward at a lower speed
  analogWrite(MOTOR_A_2, 0);  
  analogWrite(MOTOR_B_1, 0);    
  analogWrite(MOTOR_B_2, 255);  //Right Wheel moves forward

  delay(TURN_TIME);  // Delay for the turn
  stopMotors();      // Stop after the turn
}

// --- Avoidance Functions ---
// Function to avoid obstacles based on the new pattern when initially turning Right
void avoidObstacle() {
  Serial.println("Attempting to avoid obstacle...");

  // Step 1: Turn Right and move forward a short distance
  robotRight();
  robotForward();
  delay(FORWARD_TIME_1);  

  // Step 2: Turn Left and move forward a longer distance
  robotLeft();
  robotForward();
  delay(FORWARD_TIME_2);  

  // Step 3: Turn Left again and move forward the same distance as Step 2
  robotLeft();
  robotForward();
  delay(FORWARD_TIME_1);  

  // Step 4: Turn Right again and move forward the same distance as Step 1
  robotRight();
  robotForward();
  delay(FORWARD_TIME_1);  

  // Step 5: Continue normal path
  Serial.println("Obstacle avoidance completed. Continuing normal behavior.");
}

// Function to store the last avoidance maneuver
void setLastAvoidance(TurnDirection direction) {
  lastAvoidanceTurn = direction;
}