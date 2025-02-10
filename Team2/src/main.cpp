#include <Arduino.h>

// Define pins
const int       MOTOR_A_BACKWARD = 11;
const int       MOTOR_A_FORWARD = 10;
const int       MOTOR_B_FORWARD = 6;
const int       MOTOR_B_BACKWARD = 5;
const int       BUTTON_1 = 3;
const int       BUTTON_2 = 4;

// Define state variables for the millis
bool            _toggleMillis = false;
bool            _button1Pressed = false;
bool            _button2Pressed = false;
unsigned long   _lastTime = 0;
unsigned long   _millisStartTime = 0;
int             _millisStep = 0;

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
}

void loop()
{
    // Check button presses
    buttonPress();
}

void buttonPress()
{
    // Check if the forward button is pressed
    if(digitalRead(BUTTON_1) == LOW && !_button1Pressed)
    {
        _button1Pressed = true;
        _lastTime = millis(); //50ms
    }

    if (_button1Pressed)
    {
        unsigned long currentTime = millis();
        if (currentTime - _lastTime < 6000) // Drive forward for 6 seconds
        {
            driveForward(255);
        }
        else if (currentTime - _lastTime < 8000) // Turn right for 2 seconds
        {
            turnRight(255);
        }
        else if (currentTime - _lastTime < 12000) // Drive backward for 4 seconds
        {
            driveBackward(255);
        }
        else if (currentTime - _lastTime < 14000) // Turn left for 2 seconds
        {
            turnLeft(255);
        }
        else // Stop the robot
        {
            driveStop();
            _button1Pressed = false; // Reset the button press state
        }
    }

    // Check if the stop button is pressed
    if(digitalRead(BUTTON_2) == LOW)
    {
        _button2Pressed = true;
        _lastTime = millis();
        driveStop();
        _button1Pressed = false; // Reset the button press state
    }
}

void driveForward(int n)
{
    int calibratedA = calibrate(n, CALIBRATION_OFFSET_A); // Calibrate motor A speed
    int calibratedB = calibrate(n, CALIBRATION_OFFSET_B); // Calibrate motor B speed
    analogWrite(MOTOR_A_BACKWARD, 0); // Stop motor A backward
    analogWrite(MOTOR_A_FORWARD, calibratedA); // Drive motor A forward
    analogWrite(MOTOR_B_BACKWARD, 0); // Stop motor B backward
    analogWrite(MOTOR_B_FORWARD, calibratedB); // Drive motor B forward
}

void driveBackward(int n)
{
    int calibratedA = calibrate(n, CALIBRATION_OFFSET_A); // Calibrate motor A speed
    int calibratedB = calibrate(n, CALIBRATION_OFFSET_B); // Calibrate motor B speed
    analogWrite(MOTOR_A_BACKWARD, calibratedA); // Drive motor A backward
    analogWrite(MOTOR_A_FORWARD, 0); // Stop motor A forward
    analogWrite(MOTOR_B_BACKWARD, calibratedB); // Drive motor B backward
    analogWrite(MOTOR_B_FORWARD, 0); // Stop motor B forward
}

void turnLeft(int n)
{
    int calibratedA = calibrate(n, CALIBRATION_OFFSET_A); // Calibrate motor A speed
    int calibratedB = calibrate(n, CALIBRATION_OFFSET_B); // Calibrate motor B speed
    analogWrite(MOTOR_A_BACKWARD, 0); // Stop motor A backward
    analogWrite(MOTOR_A_FORWARD, calibratedA); // Drive motor A forward
    analogWrite(MOTOR_B_BACKWARD, 0); // Drive motor B backward
    analogWrite(MOTOR_B_FORWARD, calibratedB - 100); // Stop motor B forward
}

void turnRight(int n)
{
    int calibratedA = calibrate(n, CALIBRATION_OFFSET_A); // Calibrate motor A speed
    int calibratedB = calibrate(n, CALIBRATION_OFFSET_B); // Calibrate motor B speed
    analogWrite(MOTOR_A_BACKWARD, 0); // Drive motor A backward
    analogWrite(MOTOR_A_FORWARD, calibratedA - 100); // Stop motor A forward
    analogWrite(MOTOR_B_BACKWARD, 0); // Stop motor B backward
    analogWrite(MOTOR_B_FORWARD, calibratedB); // Drive motor B forward
}

void driveStop()
{
    analogWrite(MOTOR_A_BACKWARD, 0); // Stop motor A backward
    analogWrite(MOTOR_A_FORWARD, 0); // Stop motor A forward
    analogWrite(MOTOR_B_BACKWARD, 0); // Stop motor B backward
    analogWrite(MOTOR_B_FORWARD, 0); // Stop motor B forward
}

int calibrate(int n, int offset)
{
    int calibratedValue = n - offset; // Subtract the offset from the input value
    if (calibratedValue < 0) calibratedValue = 0; // Ensure the value doesn't go below 0
    return calibratedValue; // Return the calibrated value
}