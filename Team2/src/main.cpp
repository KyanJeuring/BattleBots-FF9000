#include <Arduino.h>

// Define pins
const int       MOTOR_A_BACKWARD = 11;
const int       MOTOR_A_FORWARD = 10;
const int       MOTOR_B_FORWARD = 6;
const int       MOTOR_B_BACKWARD = 5;
const int       SONAR_SENSOR_ECHO = 8;
const int       SONAR_SENSOR_TRIGGER = 7;
const int       BUTTON_1 = 2;
const int       BUTTON_2 = 3;
const int       BUTTON_3 = 4;

// Define state variables for the millis
bool            _button1Pressed = false;
bool            _button2Pressed = false;
unsigned long   _lastTime = 0;
unsigned long   _startMillis = 0;
bool            _turning = false;

// Calibration offsets
const int CALIBRATION_OFFSET_A = 10; // Adjust this value as needed
const int CALIBRATION_OFFSET_B = 5;  // Adjust this value as needed

// Define the function prototype
void drive(int motorAForward, int motorABackward, int motorBForward, int motorBBackward);
void buttonPress();
int calibrate(int n, int offset);
long readSonarSensor();

void setup()
{
    // Initialize the input and outputs
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
    pinMode(BUTTON_1, INPUT);
    pinMode(BUTTON_2, INPUT);
    pinMode(SONAR_SENSOR_TRIGGER, OUTPUT);
    pinMode(SONAR_SENSOR_ECHO, INPUT);
}

void loop()
{
    unsigned long currentMillis = millis();

    if (_turning && currentMillis - _startMillis >= 1000)
    {
        _turning = false;
    }
    else if (!_turning)
    {
        long distance = readSonarSensor();
        if (distance < 3)
        {
            drive(0, 255, 255, 0); // Turn right
            _startMillis = currentMillis;
            _turning = true;
        }
        else
        {
            drive(255, 0, 255, 0); // Move forward
        }
    }
}

long readSonarSensor()
{
    // Send a pulse to trigger the sonar sensor
    digitalWrite(SONAR_SENSOR_TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(SONAR_SENSOR_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(SONAR_SENSOR_TRIGGER, LOW);

    // Read the echo pin and calculate the distance
    long duration = pulseIn(SONAR_SENSOR_ECHO, HIGH);
    return duration * 0.034 / 2; // Convert duration to distance in cm
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
