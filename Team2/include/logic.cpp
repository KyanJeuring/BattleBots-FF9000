// Function to measure distance using ultrasonic sensor
float measureDistance()
{
    // Clear the TRIG pin
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    // Set the TRIG pin HIGH for 10 microseconds
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    // Read the ECHO pin, returns the sound wave travel time in microseconds
    long duration = pulseIn(ECHO, HIGH, 30000);  // Timeout after 30ms (approx 5m range)

    // Calculate the distance
    float distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 (go and back)

    // Filter out unreliable readings
    if (distance == 0 || distance > 200)
    {
        return 999;  // Return a large value if no echo received or out of range
    }

    return distance;
}

void endGame()
{
    // Only end game if we're not already ending it
    if (!gameEnded)
    {
        // Stop the robot
        stopMotors();

        // Open gripper to drop the cone and move backward
        conePickedUp = false;
        unsigned long startTime = millis();
        while (millis() - startTime < 200)
        {
            setDriveBackwardColor();
            moveBackward(200, 200);
        }
        stopMotors();

        // Set flags to indicate game is over
        coneDroppedOff = true;
        gameEnded = true;

        // Ensure we can't accidentally start again
        robotState = FOLLOW_LINE;
        motionComplete = true;
    }
}