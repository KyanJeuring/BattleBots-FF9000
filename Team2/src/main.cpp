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
    NeoPixel.begin();
    NeoPixel.clear();
    NeoPixel.show();
    NeoPixel.setBrightness(40);

    // Set Motor Pins
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    digitalWrite(MOTOR_B_FORWARD, LOW);
    digitalWrite(MOTOR_B_BACKWARD, LOW);
    digitalWrite(MOTOR_A_FORWARD, LOW);
    digitalWrite(MOTOR_A_BACKWARD, LOW);
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
    int unsigned currentTime = millis();
    // Check if the game has ended
    if (gameEnded)
    {

        if (currentTime - previousTime >= GRIPPER_INTERVAL)
        {
            previousTime = currentTime;
            openGripper();  // Keep the gripper open to drop the cone
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
            turn180(200, 200);
            return;  // Skip the rest of the loop to start turning
        }
    }

    // Check if the gripper should be opened or closed based on the conePickUp state
    if (conePickedUp)
    {
        if (currentTime - previousTime >= GRIPPER_INTERVAL)
        {
            previousTime = currentTime;
            closeGripper();
        }
    }
    else
    {
        if (currentTime - previousTime >= GRIPPER_INTERVAL)
        {
            previousTime = currentTime;
            openGripper();
        }
    }

    // Check if the cone is in the square and if the robot is not calibrated
    if (coneInSquare && !sensorsCalibrated)
    {
        // Start calibrating the line sensors when a robot is detected
        if (measureDistance() < 30)
        {
            robotDetected = true;
            if(robotDetected)
            {
                calibrateSensors();
            }
        }
        return;  // Skip the rest of the loop to allow for calibration
    }

    // Check if the sensors are calibrated and if the cone is not picked up
    if (sensorsCalibrated && !conePickedUp)
    {
        //pickUpCone
        conePickedUp = true;
        return;
    }

    // Check if the sensors are calibrated and if the game has not started yet
    if (sensorsCalibrated && !gameStarted && conePickedUp)
    {
        // Start the game by turning left for 90 milliseconds
        turnLeftMillis(90);
        if (robotState != FOLLOW_LINE) return; // Ensure the robot is in the FOLLOW_LINE state before proceeding
        gameStarted = true;
    }

    // Check if the game has started and if the game has not ended yet
    if (gameStarted && !gameEnded)
    {
        // Get the line position and take action based on it
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
                    setDriveForwardColor();
                }
                break;

            default:
                // Handle unexpected line positions, if necessary
                break;
        }
    }
}