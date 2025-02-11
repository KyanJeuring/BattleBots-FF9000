#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Define pins
const int       MOTOR_A_BACKWARD = 11;
const int       MOTOR_A_FORWARD = 10;
const int       MOTOR_B_FORWARD = 6;
const int       MOTOR_B_BACKWARD = 5;
const int       SONAR_SENSOR_ECHO = 8;
const int       SONAR_SENSOR_TRIGGER = 7;
const int       BUTTON_1 = 2;
const int       BUTTON_2 = 3;
const int       NEOPIXEL_PIN = 4;
const int       NUM_PIXELS = 4;

// Define the NeoPixel
const int       NEOPIXEL_BOTTOM_RIGHT = 0;
const int       NEOPIXEL_TOP_RIGHT = 1;
const int       NEOPIXEL_TOP_LEFT = 2;
const int       NEOPIXEL_BOTTOM_LEFT = 3;

// Define state variables for the millis
bool            _button1Pressed = false;
bool            _button2Pressed = false;
unsigned long   _lastTime = 0;
unsigned long   _startMillis = 0;
bool            _avoidObject = false;

// Calibration offsets
const int CALIBRATION_OFFSET_A = 10; // Adjust this value as needed
const int CALIBRATION_OFFSET_B = 5;  // Adjust this value as needed

// Initialize the NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Define the function prototype
void drive(int motorAForward, int motorABackward, int motorBForward, int motorBBackward);
void buttonPress();
int calibrate(int n, int offset);
long readSonarSensor();
void setNeoPixel(int index, uint32_t color);

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
    pinMode(SONAR_SENSOR_TRIGGER, OUTPUT);
    pinMode(SONAR_SENSOR_ECHO, INPUT);
    pinMode(NEOPIXEL_PIN, OUTPUT);

    // Initialize the outputs
    digitalWrite(MOTOR_A_BACKWARD, HIGH);
    digitalWrite(MOTOR_A_FORWARD, HIGH);
    digitalWrite(MOTOR_B_FORWARD, HIGH);
    digitalWrite(MOTOR_B_BACKWARD, HIGH);
    digitalWrite(NEOPIXEL_PIN, HIGH);

    // Initialize the NeoPixel strip
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
    unsigned long currentMillis = millis();

    if (_avoidObject && currentMillis - _startMillis >= 1000)
    {
        _avoidObject = false;
    }
    else if (!_avoidObject)
    {
        long distance = readSonarSensor();
        if (distance < 15)
        {
            // Turn right
            drive(255, 0, 0, 255);
            setNeoPixel(NEOPIXEL_TOP_RIGHT, strip.Color(255, 0, 0));
            delay(500);

            // Go forward
            drive(255, 0, 255, 0);
            strip.clear();
            delay(1000);

            // Turn left
            drive(NEOPIXEL_TOP_LEFT, 255, 255, 0);
            setNeoPixel(0, strip.Color(0, 255, 0));
            delay(500);

            // Go forward
            drive(255, 0, 255, 0);
            strip.clear();
            delay(1000);

            // Turn left
            drive(0, 255, 255, 0);
            setNeoPixel(NEOPIXEL_TOP_LEFT, strip.Color(0, 255, 0));
            delay(500);

            // Go forward
            drive(255, 0, 255, 0);
            strip.clear();
            delay(1000);

            // Turn right
            drive(255, 0, 0, 255);
            setNeoPixel(NEOPIXEL_TOP_RIGHT, strip.Color(255, 0, 0));
            delay(500);
            _startMillis = currentMillis;
            _avoidObject = true;
        }
        else
        {
            // Drive forward
            drive(255, 0, 255, 0);
            strip.clear();
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
    return duration * 0.034 / 2; // Convert duration to distance in cm by using the speed of sound (0.034 cm per microsecond)
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

void setNeoPixel(int index, uint32_t color)
{
    strip.setPixelColor(index, color);
    strip.show();
}