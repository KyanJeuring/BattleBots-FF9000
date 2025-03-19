#include <Arduino.h>
#include <declare.cpp>
#include <debugLight.cpp>
#include <gripper.cpp>
#include <lineSensor.cpp>
#include <drive.cpp>
#include <lineSensorCalibration.cpp>
#include <pathChecker.cpp>

void setup()
{
    Serial.begin(9600);
    NeoPixel.begin();
    NeoPixel.setBrightness(50);

    // Set Motor Pins
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    digitalWrite(MOTOR_B_FORWARD, LOW);
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);

    digitalWrite(MOTOR_A_BACKWARD, LOW);
    digitalWrite(MOTOR_A_FORWARD, LOW);
    digitalWrite(MOTOR_B_BACKWARD, LOW);

    // Attach Interrupts for Encoders
    attachInterrupt(digitalPinToInterrupt(MOTOR_R1), leftEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(MOTOR_R2), rightEncoderISR, CHANGE);

    // Set Line Sensor Pins
    declareLineSensorPins();

    // SERVO
    pinMode(SERVO, OUTPUT);
    digitalWrite(SERVO, LOW);
}

void loop()
{
    unsigned int currentTime = millis();
    if (currentTime - previousTime >= gripperInterval)
    {
        previousTime = currentTime;
        conePickedUp ? closeGripper() : openGripper();
    }

    if (coneInSquare && !sensorsCalibrated)
    {
        // calibrateSensors
        calibrateSensors();
    }

    if (sensorsCalibrated && !conePickedUp)
    {
        // pickUpCone
        conePickedUp = true;
        return;
    }

    if (sensorsCalibrated && !gameStarted && conePickedUp)
    {
        turnLeftMillis(90);
        if (robotState != FOLLOW_LINE)
            return;
        gameStarted = true;
    }

    if (gameStarted && !gameEnded)
    {
        getLinePosition();
        Serial.print("Robotstate: ");
        Serial.println(robotState);
        Serial.print("LinePosition: ");
        Serial.println(linePosition);

        if (linePosition != CENTER_LINE)
        {
            // checkPathAhead();
            // if(!pathChecked) return;
            if (linePosition == T_JUNCTION)
            {
                turnLeftMillis(90);
                Serial.println(robotState);
                Serial.println("TURNING_LEFT2");
                setTurnLeftColor();
            }
            else if (linePosition == LEFT_LINE)
            {
                turnLeftMillis(70);
                Serial.println(robotState);
                Serial.println("TURNING_LEFT");
                setTurnLeftColor();
            }
            else if (linePosition == NO_LINE)
            {
                turnAroundMillis();
                Serial.println(robotState);
                Serial.println("TURNING_AROUND");
                setTurnAroundColor();
            }
            else if (linePosition == RIGHT_LINE)
            {
                linePosition == CENTER_LINE;
                robotState = FOLLOW_LINE;
                // turnRightMillis(90);
                moveForwardPID(baseSpeed, baseSpeed, false, true);
                Serial.println(robotState);
                Serial.println("TURNING_RIGHT");
                setTurnRightColor();
            }
        }

        if (linePosition == CENTER_LINE)
        {
            moveForwardPID(baseSpeed, baseSpeed, false, true);
            Serial.println(robotState);
            Serial.println("FollowLine");
            setDriveForwardColor();
        }
    }
}