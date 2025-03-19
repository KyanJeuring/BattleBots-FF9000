#include <Adafruit_NeoPixel.h>

// Robot States
enum RobotState         {FOLLOW_LINE, TURNING, TURNING_LEFT, TURNING_RIGHT, TURNING_AROUND, CHECKING_FOR_PATH_AHEAD};
enum LinePosition       {T_JUNCTION, LEFT_LINE, RIGHT_LINE, NO_LINE, CENTER_LINE};

RobotState              robotState = FOLLOW_LINE;
LinePosition            linePosition = CENTER_LINE;

// Line Sensors
const int               NUM_SENSORS = 8; // Number of sensors
const int               sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Sensor pin mapping
int                     sensorValues[NUM_SENSORS];        // Array to store sensor readings
int                     sensorMin[NUM_SENSORS];
int                     sensorMax[NUM_SENSORS];
int                     sensorThreshold[NUM_SENSORS]; // Array to store thresholds for each sensor

// Line Positions
bool                    leftTurn;
bool                    rightTurn;
bool                    tJunctionOrBase;
bool                    deadEnd;

// Measurements
const float             WHEEL_CIRCUMFERENCE = 20.4;
const int               PULSE_PER_REVOLUTION = 20;
const float             DISTANCE_BETWEEN_WHEELS = 22.75;
static const int        DISTANCE_FROM_BASE_TO_CONE = 55; // Distance is in ticks
const int               target = DISTANCE_FROM_BASE_TO_CONE;

// Encoder Pulse Counters
volatile signed int     _leftTicks = 0;
volatile signed int     _rightTicks = 0;

// Motor Pins
const int               MOTOR_A_BACKWARD = 11; // Right Backward
const int               MOTOR_A_FORWARD = 10;  // Right Forward
const int               MOTOR_B_FORWARD = 6;   // Left Forward
const int               MOTOR_B_BACKWARD = 5;  // Left Backward
int                     baseSpeed = 255;

// Servo Control
const int               GRIPPER_OPEN = 1800;
const int               GRIPPER_CLOSE = 990;
const int               SERVO = 9;
const int               pulse = 2000;
int                     previousTime = 0;
const int               gripperInterval = 20;

// Rotation Sensors
const int               MOTOR_R1 = 2;
const int               MOTOR_R2 = 3;

const int               ISR_INTERVAL = 20; // interval of 20 milli seconds to update counter by interupt

// Conditions
bool                    coneInSquare = true;
bool                    sensorsCalibrated = false;
bool                    conePickedUp = false;
bool                    gameStarted = false;
bool                    coneDroppedOff = false;
bool                    gameEnded = false;
bool                    motionComplete = true;
static bool             pathChecked = false;

// PID Control Variables
int                     error = 0, lastError = 0;
float                   integral = 0;
float                   derivative = 0;

// PID
float                   Kp; // Proportional Gain
float                   Ki; // Integral Gain
float                   Kd; // Derivative Gain
int                     correction;

int                     pulses;
int                     angle;
int                     raduis = DISTANCE_BETWEEN_WHEELS;
int                     turn_Circumference = 2 * 3.14 * raduis;
float                   turnDistances = 0; // ARC of a circle

// Define NeoPixel
const int               NEOPIXEL_PIN = 4;
const int               NUM_PIXELS = 4;
const int               PIXEL_BOTTOM_LEFT = 0;
const int               PIXEL_BOTTOM_RIGHT = 1;
const int               PIXEL_TOP_RIGHT = 2;
const int               PIXEL_TOP_LEFT = 3;
Adafruit_NeoPixel       NeoPixel = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);

// Function to reset encoder ticks
void resetTicks()
{
    _leftTicks = 0;
    _rightTicks = 0;
}