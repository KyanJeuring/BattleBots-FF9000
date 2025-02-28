#include <Arduino.h>
#include <declare.cpp>
#include <drive.cpp>
#include <sonarSensor.cpp>

// Define pins
const int       BLUETOOTH_TRANSMIT= 1;
const int       MOTOR_A_BACKWARD = 10;
const int       MOTOR_A_FORWARD = 9;
const int       MOTOR_B_FORWARD = 6;
const int       MOTOR_B_BACKWARD = 5;
const int       SONAR_SENSOR_ECHO = 8;
const int       SONAR_SENSOR_TRIGGER = 7;
const int       LINE_SENSORS[] = {A0, A1, A2, A3, A4, A5, A6, A7};

// Define state variables for the millis 
unsigned long   _lastTime = 0;
unsigned long   _startMillis = 0;
bool            _avoidObject = false;

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
    // Initialize serial communication
    Serial.begin(9600);

    for(int i = 0; i < sizeof(LINE_SENSORS); i++)
    {
        pinMode(LINE_SENSORS[i], INPUT);
    }
    // Initialize the input and outputs
    pinMode(BLUETOOTH_TRANSMIT, OUTPUT);
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
    pinMode(SONAR_SENSOR_TRIGGER, OUTPUT);
    pinMode(SONAR_SENSOR_ECHO, INPUT);

    // Initialize the outputs
    digitalWrite(MOTOR_A_BACKWARD, HIGH);
    digitalWrite(MOTOR_A_FORWARD, HIGH);
    digitalWrite(MOTOR_B_FORWARD, HIGH);
    digitalWrite(MOTOR_B_BACKWARD, HIGH);
}

void loop()
{
    
}

long readSonarSensor()
{
    long distance1 = 0;
    long distance2 = 0;
    long distance3 = 0;

    // Read the distance 3 times with delays between readings
    for (int i = 0; i < 3; i++)
    {
        // Send a pulse to trigger the sonar sensor
        digitalWrite(SONAR_SENSOR_TRIGGER, LOW);
        delayMicroseconds(2);
        digitalWrite(SONAR_SENSOR_TRIGGER, HIGH);
        delayMicroseconds(10);
        digitalWrite(SONAR_SENSOR_TRIGGER, LOW);

        // Read the echo pin and calculate the distance
        long duration = pulseIn(SONAR_SENSOR_ECHO, HIGH);
        long distance = duration * 0.034 / 2; // Convert duration to distance in cm by using the speed of sound (0.034 cm per microsecond)

        // Store the distance in the corresponding variable
        if (i == 0) distance1 = distance;
        else if (i == 1) distance2 = distance;
        else if (i == 2) distance3 = distance;

        // Small delay between readings
        delay(50);
    }
    // Check if the distances are the same
    if (distance1 == distance2 && distance2 == distance3)
    {
        return distance1; // Return the distance if all 3 readings are the same
    }
    else
    {
        return -1; // Return -1 if the readings are not consistent
    }
}

// Drive with calibration
void drive(int motorAForward, int motorABackward, int motorBForward, int motorBBackward)
{
    analogWrite(MOTOR_A_FORWARD, calibrate(motorAForward, CALIBRATION_OFFSET_A));
    analogWrite(MOTOR_A_BACKWARD, calibrate(motorABackward, CALIBRATION_OFFSET_A));
    analogWrite(MOTOR_B_FORWARD, calibrate(motorBForward, CALIBRATION_OFFSET_B));
    analogWrite(MOTOR_B_BACKWARD, calibrate(motorBBackward, CALIBRATION_OFFSET_B));
}

int calibrate(int n, int offset)
{
    int calibratedValue = n - offset; // Subtract the offset from the input value
    return (calibratedValue < 0) ? 0 : calibratedValue; // Ensure the value doesn't go below 0
}