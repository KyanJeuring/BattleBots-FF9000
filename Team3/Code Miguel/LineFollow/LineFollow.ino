#include <Adafruit_NeoPixel.h>

// ============================
// MOTOR PIN SETUP
// ============================
#define BTPIN 1  // Bluetooth pin (not used in current code, but defined for future use)

// Left motor control pins
#define IN1 12  // IN1 controls left motor forward direction
#define IN2 13  // IN2 controls left motor backward direction

// Right motor control pins
#define IN3 10  // IN3 controls right motor forward direction
#define IN4 11  // IN4 controls right motor backward direction

// ============================
// NeoPixel Setup
// ============================
#define NEO_PIN 3  // Pin connected to NeoPixel data line
#define NUM_PIXELS 4  // Number of NeoPixels in the array
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

// ============================
// SENSOR ARRAY SETUP
// ============================
int _sensorPins[] = { A0, A1, A2, A3, A4, A5, A6, A7 };  // Pins for sensor array (connected to analog inputs A0 to A7)
int _sensorValues[8];  // Array to store sensor readings
int _sensorThreshold = 600;  // Threshold for sensor activation, above this value the sensor is considered to detect the line
int _sensorWeights[] = { -3500, -2500, -1500, -500, 500, 1500, 2500, 3500 }; // Weight values for each sensor position (left to right)

// ============================
// PID CONSTANTS
// ============================
float _Kp = 0.12;  // Proportional gain (controls how much the robot adjusts based on the error)
float _Ki = 0.001;  // Integral gain (helps correct small, long-term errors)
float _Kd = 0.04;   // Derivative gain (helps dampen sudden changes in error)

int _previousError = 0;  // Store the previous error value for calculating the derivative term
int _integral = 0;  // Integral of the error, to help fix long-term errors
int _baseSpeed = 220;  // Base speed of the motors, will be adjusted by the PID controller

// ============================
// SETUP FUNCTION
// ============================
void setup() {
    // Start serial communication for debugging purposes
    Serial.begin(9600);

    // Initialize sensor pins as input
    for (int i = 0; i < 8; i++) {
        pinMode(_sensorPins[i], INPUT);  // Set each sensor pin as input
    }

    // Initialize motor control pins as output
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(BTPIN, OUTPUT);  // Bluetooth pin setup (not used in current code)

    // Initialize NeoPixel strip
    strip.begin();
    strip.show();  // Initialize all pixels to 'off'
}

// ============================
// MAIN LOOP
// ============================
void loop() {
    // Read sensor values and calculate the error (position relative to the line)
    int position = readSensors();  
    int error = position;  // The error is the position, which can be positive or negative depending on the line's position

    // Call the PID controller function to adjust motor speeds based on the error
    PID_Linefollow(error);

    // Update the NeoPixel display based on the robot's status
    updateNeoPixels(error);
}

/*
The readSensors() function reads values from the sensor array and calculates the position of the line relative to the robot's current position.
It returns an "error" value that the PID controller will use to adjust motor speeds.
*/

// ============================
// SENSOR READING FUNCTION
// ============================
int readSensors() {
    int weightedSum = 0;  // Variable to hold the weighted sum of sensor readings
    int sum = 0;  // Variable to count how many sensors detect the line

    // Loop through all sensors (A0 to A7)
    for (int i = 0; i < 8; i++) {
        _sensorValues[i] = analogRead(_sensorPins[i]);  // Read sensor values (0-1023)

        // If a sensor detects the line (sensor value is above the threshold)
        if (_sensorValues[i] > _sensorThreshold) {
            weightedSum += _sensorWeights[i];  // Add the sensor's weight to the weighted sum
            sum++;  // Increment the count of sensors detecting the line
        }
    }

    // If no sensors detect the line, continue turning based on the last error
    if (sum == 0) {
        return _previousError > 0 ? 1000 : -1000;  // Return a value to continue turning in the last direction
    }
    
    // Calculate and return the average weighted position of the line based on the detected sensors
    return weightedSum / sum;  // Return the calculated position
}

/*
The PID_Linefollow() function adjusts the robot's motor speeds based on the error value (calculated position).
It uses a PID control algorithm to make continuous adjustments to correct the robot's path.
*/

// ============================
// PID CONTROL FUNCTION
// ============================
void PID_Linefollow(int error) {
    // Proportional term: directly proportional to the error
    int P = error;
    
    // Integral term: sum of past errors
    _integral += error;

    // Limit the integral term to avoid excessive buildup
    _integral = constrain(_integral, -1000, 1000);

    // Derivative term: change in error (compared to previous error)
    int D = error - _previousError;

    // Calculate the PID value based on the formula
    int PIDvalue = (_Kp * P) + (_Ki * _integral) + (_Kd * D);

    // Update the previous error for the next cycle
    _previousError = error;

    // Calculate the left and right motor speeds by adjusting the base speed
    int leftSpeed = _baseSpeed - PIDvalue;  // Left motor speed (adjusted by the PID correction)
    int rightSpeed = _baseSpeed + PIDvalue;  // Right motor speed (adjusted by the PID correction)

    // Constrain motor speeds to prevent them from going out of bounds (-255 to 255)
    leftSpeed = constrain(leftSpeed, -200, 200);  // Limited to -200 to 200
    rightSpeed = constrain(rightSpeed, -200, 200);  // Limited to -200 to 200
    
    // Send the motor speed values to the motor control function
    motor_drive(leftSpeed, rightSpeed);
}

/*
The motor_drive() function takes the adjusted motor speeds (left and right) and controls the motors' direction and speed.
It ensures that the motors are set to move at the correct speed and in the correct direction.
*/

// ============================
// MOTOR CONTROL FUNCTION
// ============================
void motor_drive(int left, int right) {
    // Apply a minimum speed limit to avoid motor jitter (keep motors at least moving)
    if (abs(left) < 50) {
        left = 0;  // Stop the left motor if speed is too low
    }
    if (abs(right) < 50) {
        right = 0;  // Stop the right motor if speed is too low
    }

    // Left motor control: Forward if speed is positive, backward if negative
    if (left >= 0) {
        analogWrite(IN1, left);  // Forward direction (speed)
        analogWrite(IN2, 0);  // Stop backward direction
    } else {
        analogWrite(IN1, 0);  // Stop forward direction
        analogWrite(IN2, -left);  // Backward direction (negative speed)
    }

    // Right motor control: Forward if speed is positive, backward if negative
    if (right >= 0) {
        analogWrite(IN3, right);  // Forward direction (speed)
        analogWrite(IN4, 0);  // Stop backward direction
    } else {
        analogWrite(IN3, 0);  // Stop forward direction
        analogWrite(IN4, -right);  // Backward direction (negative speed)
    }

    // Print motor speeds to the serial monitor for debugging purposes (optional)
    Serial.print(left);  // Print left motor speed
    Serial.print("   ");
    Serial.print(right);  // Print right motor speed
    Serial.println("");  // Newline for readability in the Serial Monitor
}

/*
The updateNeoPixels() function controls the NeoPixels based on the robot's behavior.
It will change the colors of the NeoPixels based on the robot's line-following actions.
*/
void updateNeoPixels(int error) {
    if (error == 0) {
        // On line, turn NeoPixels green
        // Set all pixels to green
        setNeoPixelsColor(0, 255, 0);  // Green for all
    } else if (error > 0) {
        // Turn right, set NeoPixels to blue
        // Set the first few pixels to blue (for turning right)
        strip.setPixelColor(0, 252, 219, 56);  // Turn the 3rd pixel blue (example)
        strip.setPixelColor(1, 0, 0, 0);
        strip.setPixelColor(2, 0, 0, 0);
        strip.setPixelColor(3, 252, 219, 56);
    } else if (error < 0) {
        // Turn left, set NeoPixels to red
        // Set the first few pixels to red (for turning left)
        strip.setPixelColor(0, 0, 0, 0);  // Turn the 3rd pixel blue (example)
        strip.setPixelColor(1, 252, 219, 56);
        strip.setPixelColor(2, 252, 219, 56);
        strip.setPixelColor(3, 0, 0, 0);  // Turn the 3rd pixel blue (example)
    }
    strip.show();  // Make sure to call this after setting the colors to apply the changes
}


/*
Sets all NeoPixels to a specific color
*/
void setNeoPixelsColor(int red, int green, int blue) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        strip.setPixelColor(i, 0, 0, 0);  // Set color
    }
    strip.show();  // Update the strip to show changes
}
