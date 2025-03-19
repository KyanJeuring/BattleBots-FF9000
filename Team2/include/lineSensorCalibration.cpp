void calibrateSensors()
{
    static bool firstRun = true; // Flag to check if it's the first run
    readSensors(); // Function to read sensor values

    // Initialize sensor min/max values on the first run
    if (firstRun)
    {
        for (int i = 0; i < NUM_SENSORS; i++)
        {
            sensorMin[i] = 1023; // Set min to max ADC value
            sensorMax[i] = 0;    // Set max to min ADC value
        }
        firstRun = false; // Set firstRun to false after initialization
    }

    // Check if target distance is reached
    if (_leftTicks > target || _rightTicks > target)
    {
        stopMotors(); // Stop the motors
        sensorsCalibrated = true; // Set calibration flag to true

        // Calculate and store threshold values
        for (int i = 0; i < NUM_SENSORS; i++)
        {
            sensorThreshold[i] = (sensorMin[i] + sensorMax[i]) / 2; // Calculate threshold
            Serial.print("Threshold [");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(sensorThreshold[i]);
            Serial.println(sensorMin[i]);
            Serial.println(sensorMax[i]);
        }

        return;
    }

    // Read sensor values and update min/max
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        int sensorValue = analogRead(sensorPins[i]); // Read sensor value

        if (sensorValue < sensorMin[i])
        {
            sensorMin[i] = sensorValue; // Update min value
        }
        if (sensorValue > sensorMax[i])
        {
            sensorMax[i] = sensorValue; // Update max value
        }
    }

    moveForwardPID(180, 180, true, false); // Move forward with PID control
}