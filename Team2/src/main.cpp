#include <Arduino.h>

// Define pins
const int MOTOR_A_BACKWARD = 11;
const int MOTOR_A_FORWARD = 10;
const int MOTOR_B_FORWARD = 6;
const int MOTOR_B_BACKWARD = 5;
const int BUTTON_1 = 3;
const int BUTTON_2 = 4;
const int RED_LED = 2;
const int YELLOW_LED = 8;
const int GREEN_LED = 9;

// Define state variables for the sequence
bool      _toggleSequence = false;

// Calibration offsets
const int CALIBRATION_OFFSET_A = 10; // Adjust this value as needed
const int CALIBRATION_OFFSET_B = 5;  // Adjust this value as needed

// Define the function prototype
void driveForward(int n);
void driveBackward(int n);
void turnRight(int n);
void turnLeft(int n);
void driveStop();
void buttonPress();
void lightSequence();
int calibrate(int n, int offset);

void setup()
{
    // Initialize the input and outputs
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
    pinMode(BUTTON_1, INPUT);
    pinMode(BUTTON_2, INPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    
    // Set the initial state of the pins
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
}

void loop()
{
    // Check button presses
    buttonPress();
}

void buttonPress()
{
    // Check if the forward button is pressed
    if(digitalRead(BUTTON_1) == LOW)
    {
        lightSequence();
    }
    // Check if the backward button is pressed
    if(digitalRead(BUTTON_2) == LOW)
    {
        driveForward(255);
        delay(6000);
        turnRight(255);
        delay(2000);
        driveBackward(255);
        delay(6000);
        turnLeft(255);
        delay(2000);
        driveStop();
    }
}

// Drive forward with calibration
void driveForward(int n)
{
    int calibratedA = calibrate(n, CALIBRATION_OFFSET_A); // Calibrate motor A speed
    int calibratedB = calibrate(n, CALIBRATION_OFFSET_B); // Calibrate motor B speed
    analogWrite(MOTOR_A_BACKWARD, 0); // Stop motor A backward
    analogWrite(MOTOR_A_FORWARD, calibratedA); // Drive motor A forward
    analogWrite(MOTOR_B_BACKWARD, 0); // Stop motor B backward
    analogWrite(MOTOR_B_FORWARD, calibratedB); // Drive motor B forward
}

// Drive backward with calibration
void driveBackward(int n)
{
    int calibratedA = calibrate(n, CALIBRATION_OFFSET_A); // Calibrate motor A speed
    int calibratedB = calibrate(n, CALIBRATION_OFFSET_B); // Calibrate motor B speed
    analogWrite(MOTOR_A_BACKWARD, calibratedA); // Drive motor A backward
    analogWrite(MOTOR_A_FORWARD, 0); // Stop motor A forward
    analogWrite(MOTOR_B_BACKWARD, calibratedB); // Drive motor B backward
    analogWrite(MOTOR_B_FORWARD, 0); // Stop motor B forward
}

// Turn left with calibration
void turnLeft(int n)
{
    int calibratedA = calibrate(n, CALIBRATION_OFFSET_A); // Calibrate motor A speed
    int calibratedB = calibrate(n, CALIBRATION_OFFSET_B); // Calibrate motor B speed
    analogWrite(MOTOR_A_BACKWARD, 0); // Stop motor A backward
    analogWrite(MOTOR_A_FORWARD, calibratedA); // Drive motor A forward
    analogWrite(MOTOR_B_BACKWARD, 0); // Drive motor B backward
    analogWrite(MOTOR_B_FORWARD, calibratedB - 100); // Stop motor B forward
}

// Turn right with calibration
void turnRight(int n)
{
    int calibratedA = calibrate(n, CALIBRATION_OFFSET_A); // Calibrate motor A speed
    int calibratedB = calibrate(n, CALIBRATION_OFFSET_B); // Calibrate motor B speed
    analogWrite(MOTOR_A_BACKWARD, 0); // Drive motor A backward
    analogWrite(MOTOR_A_FORWARD, calibratedA - 100); // Stop motor A forward
    analogWrite(MOTOR_B_BACKWARD, 0); // Stop motor B backward
    analogWrite(MOTOR_B_FORWARD, calibratedB); // Drive motor B forward
}

// Stop the motors
void driveStop()
{
    analogWrite(MOTOR_A_BACKWARD, 0); // Stop motor A backward
    analogWrite(MOTOR_A_FORWARD, 0); // Stop motor A forward
    analogWrite(MOTOR_B_BACKWARD, 0); // Stop motor B backward
    analogWrite(MOTOR_B_FORWARD, 0); // Stop motor B forward
}

// Calibrate the motor speed by subtracting the offset
int calibrate(int n, int offset)
{
    int calibratedValue = n - offset; // Subtract the offset from the input value
    if (calibratedValue < 0) calibratedValue = 0; // Ensure the value doesn't go below 0
    return calibratedValue; // Return the calibrated value
}

void lightSequence()
{
    // Check if the button is pressed and the sequence is not already toggled
    if(digitalRead(BUTTON_1) == LOW && _toggleSequence == false)
    {
        _toggleSequence = true; // Set the sequence toggle to true
        delay(2000); // Wait for 2 seconds

        // Change the lights to red
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        delay(4000); // Wait for 4 seconds

        // Change the lights to yellow
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW); // Yellow LED 
        delay(2000); // Wait for 2 seconds

        // Change the lights back to the initial state
        digitalWrite(RED_LED, LOW);
        digitalWrite(YELLOW_LED, HIGH);

        _toggleSequence = false; // Reset the sequence toggle
    }
}