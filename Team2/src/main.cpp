#include <Arduino.h>
#include <declare.cpp>
#include <debugLight.cpp>
#include <gripper.cpp>
#include <lineSensor.cpp>
#include <drive.cpp>
#include <logic.cpp>
#include <lineSensorCalibration.cpp>

void setup()
{
    Serial.begin(9600);
    NeoPixel.show();
    NeoPixel.setBrightness(50);

    // Set Motor Pins
    pinMode(MOTOR_A_1, OUTPUT);
    pinMode(MOTOR_A_2, OUTPUT);
    pinMode(MOTOR_B_1, OUTPUT);
    pinMode(MOTOR_B_2, OUTPUT);
    digitalWrite(MOTOR_A_1, LOW);
    digitalWrite(MOTOR_A_2, LOW);
    digitalWrite(MOTOR_B_1, LOW);
    digitalWrite(MOTOR_B_2, LOW);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // Attach Interrupts for Encoders
    attachInterrupt(digitalPinToInterrupt(MOTOR_R1), leftEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(MOTOR_R2), rightEncoderISR, CHANGE);

    //SERVO
    pinMode(SERVO, OUTPUT);
    digitalWrite(SERVO, LOW);
}

void loop()
{
    // put your main code here, to run repeatedly:
    int unsigned currentTime = millis();
    if (gameEnded)
    {
        setDriveStopColor();

        if (currentTime - previousTime >= gripperInterval)
        {
            previousTime = currentTime;
            gripper(GRIPPER_OPEN);  // Keep the gripper open to drop the cone
        }
        return;  // Skip the rest of the loop
    }

    // Check for obstacles (only when following line, not during turns)
    if (robotState == FOLLOW_LINE && gameStarted && !gameEnded)
    {
        float distance = measureDistance();

        // If obstacle detected within 12cm, turn around
        if (distance < 12)
        {
            turnAroundMillis();
            return;  // Skip the rest of the loop to start turning
        }
    }

    if (conePickedUp)
    {
        if (currentTime - previousTime >= gripperInterval)
        {
            previousTime = currentTime;
            gripper(GRIPPER_CLOSE);
        }
    }
    else
    {
        if (currentTime - previousTime >= gripperInterval)
        {
            previousTime = currentTime;
            gripper(GRIPPER_OPEN);
        }
    }

    if (coneInSquare && !sensorsCalibrated)
    {
        //calibrateSensors
        calibrateSensors();
    }

    if (sensorsCalibrated && !conePickedUp)
    {
        //pickUpCone
        conePickedUp = true;
        return;
    }

    if (sensorsCalibrated && !gameStarted && conePickedUp)
    {
        turnLeftMillis(90);
        if (robotState != FOLLOW_LINE) return;
        gameStarted = true;
    }

    if (gameStarted && !gameEnded)
    {
        getLinePosition();

        switch (linePosition)
        {
            case T_JUNCTION:
                turnLeftMillis(90);
                break;

            case LEFT_LINE:
                turnLeftMillis(70);
                break;

            case NO_LINE:
                turnAroundMillis();
                break;

            case RIGHT_LINE:
                robotState = FOLLOW_LINE;
                moveForwardPID(baseSpeed, baseSpeed, false, true);
                break;

            case CENTER_LINE:
                if (robotState == FOLLOW_LINE)
                {
                    moveForwardPID(baseSpeed, baseSpeed, false, true);
                }
                break;

            default:
                // Handle unexpected line positions, if necessary
                break;
        }
    }
}