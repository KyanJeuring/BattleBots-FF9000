#include <Arduino.h>

// Define pins
const int       MOTOR_A_BACKWARD = 11;
const int       MOTOR_A_FORWARD = 10;
const int       MOTOR_B_FORWARD = 6;
const int       MOTOR_B_BACKWARD = 5;
const int       BUTTON_1 = 2;
const int       BUTTON_2 = 3;
const int       BUTTON_3 = 4;

// Define state variables for the millis
bool            _button1Pressed = false;
bool            _button2Pressed = false;
unsigned long   _startMillis = 0;

// Calibration offsets
const int CALIBRATION_OFFSET_A = 10; // Adjust this value as needed
const int CALIBRATION_OFFSET_B = 5;  // Adjust this value as needed

// Define the function prototype
void drive(int motorAForward, int motorABackward, int motorBForward, int motorBBackward);
void buttonPress();
int calibrate(int n, int offset);

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);
    
    // Initialize the input and outputs
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
    pinMode(BUTTON_1, INPUT);
    pinMode(BUTTON_2, INPUT);
}

void loop()
{
    buttonPress();
    unsigned long currentMillis = millis();

    if (_button1Pressed)
    {
        if (currentMillis - _startMillis < 6000)
        {
            drive(255, 0, 255, 0); // Move forward
        }
        else if (currentMillis - _startMillis < 8000)
        {
            drive(160, 0, 255, 0); // Turn right
        }
        else if (currentMillis - _startMillis < 12000)
        {
            drive(0, 255, 0, 255); // Move backward
        }
        else if (currentMillis - _startMillis < 14000)
        {
            drive(255, 0, 160, 0); // Turn left
        }
        else
        {
            drive(0, 0, 0, 0); // Stop
            _button1Pressed = false; // Reset button state
        }
    }

    if (_button2Pressed)
    {
        drive(0, 0, 0, 0); // Stop
        _button2Pressed = false; // Reset button state
        _button1Pressed = false; // Reset button 1 state
        _startMillis = currentMillis; // Reset start time
    }
    else
    {
        _startMillis = currentMillis; // Reset start time if no button is pressed
    }
}

void buttonPress()
{
    if (digitalRead(BUTTON_1) == LOW)
    {
        _button1Pressed = true;
    }
    else if (digitalRead(BUTTON_2) == LOW)
    {
        _button2Pressed = true;
    }
}

// Drive with calibration
void drive(int motorAForward, int motorABackward, int motorBForward, int motorBBackward)
{
    int calibratedAForward = calibrate(motorAForward, CALIBRATION_OFFSET_A);
    int calibratedABackward = calibrate(motorABackward, CALIBRATION_OFFSET_A);
    int calibratedBForward = calibrate(motorBForward, CALIBRATION_OFFSET_B);
    int calibratedBBackward = calibrate(motorBBackward, CALIBRATION_OFFSET_B);

    analogWrite(MOTOR_A_FORWARD, calibratedAForward);
    analogWrite(MOTOR_A_BACKWARD, calibratedABackward);
    analogWrite(MOTOR_B_FORWARD, calibratedBForward);
    analogWrite(MOTOR_B_BACKWARD, calibratedBBackward);
}

int calibrate(int n, int offset)
{
    int calibratedValue = n - offset; // Subtract the offset from the input value
    return (calibratedValue < 0) ? 0 : calibratedValue; // Ensure the value doesn't go below 0
}