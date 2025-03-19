void checkPathAhead()
{
    static bool checkingPath = false; // Flag to indicate if the path is being checked
    static int forwardTicks = 10; // Number of ticks to move forward before checking the path
    static LinePosition storedPosition = CENTER_LINE; // Variable to store the initial line position
    pathChecked = false; // Reset the pathChecked flag

    if (!checkingPath)
    {
        // Store the intended LinePosition before checking
        storedPosition = linePosition;
        resetTicks(); // Reset the tick counters

        // Move forward slightly
        moveForwardPID(200, 200, true, false); // Move forward with PID control
        checkingPath = true; // Set the checkingPath flag
        Serial.print("Checking Path: ");
    }

    // Wait until the robot moves forward by forwardTicks
    if (checkingPath && (_leftTicks >= forwardTicks || _rightTicks >= forwardTicks))
    {
        stopMotors(); // Stop the motors
        checkingPath = false; // Reset the checkingPath flag
        Serial.println("PathChecked");
        
        // Check if a path exists ahead
        readSensors(); // Read the sensor values
        bool pathExists = sensorValues[2] > sensorThreshold[2] || sensorValues[3] > sensorThreshold[3] || sensorValues[4] > sensorThreshold[4] || sensorValues[5] > sensorThreshold[5];

        if (pathExists)
        {
            robotState = FOLLOW_LINE; // Set the robot state to follow the line
            linePosition = CENTER_LINE; // Set the line position to center
            motionComplete = true; // Indicate that the motion is complete
            pathChecked = true; // Set the pathChecked flag
        }
        else
        {
            linePosition = storedPosition; // Restore the original line position
            motionComplete = false; // Indicate that the motion is not complete
            pathChecked = true; // Set the pathChecked flag
        }
    }
}