// Function to declare the sensor pins as input
void declareLineSensorPins()
{
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        pinMode(sensorPins[i], INPUT);
    }
}

// Function to read the sensor values
void readSensors()
{
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        sensorValues[i] = analogRead(sensorPins[i]);
    }
}

// Function to calculate the position of the line
int calculateLinePosition()
{
    long weightedSum = 0;
    long sum = 0;

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        int value = sensorValues[i];

        // Calculate the weighted sum of sensor values
        weightedSum += (long)value * i * 1000;
        sum += value;
    }

    // Return the calculated line position
    return weightedSum / sum; // No line detected
}

// Function to determine the line position and set the appropriate flags
void getLinePosition()
{
    readSensors();
    
    leftTurn = sensorValues[4] > sensorThreshold[4] && sensorValues[5] > sensorThreshold[5] && sensorValues[6] > sensorThreshold[6] && sensorValues[7] > sensorThreshold[7] && sensorValues[0] < sensorThreshold[0] && sensorValues[1] < sensorThreshold[1] && sensorValues[2] < sensorThreshold[2];
    rightTurn = sensorValues[5] < sensorThreshold[5] && sensorValues[6] < sensorThreshold[6] && sensorValues[7] < sensorThreshold[7] && sensorValues[0] > sensorThreshold[0] && sensorValues[1] > sensorThreshold[1] && sensorValues[2] > sensorThreshold[2];
    tJunctionOrBase = sensorValues[0] > sensorThreshold[0] && sensorValues[1] > sensorThreshold[1] && sensorValues[2] > sensorThreshold[2] && sensorValues[3] > sensorThreshold[3] && sensorValues[4] > sensorThreshold[4] && sensorValues[5] > sensorThreshold[5] && sensorValues[6] > sensorThreshold[6] && sensorValues[7] > sensorThreshold[7];
    deadEnd = sensorValues[0] < sensorThreshold[0] && sensorValues[1] < sensorThreshold[1] && sensorValues[2] < sensorThreshold[2] && sensorValues[3] < sensorThreshold[3] && sensorValues[4] < sensorThreshold[4] && sensorValues[5] < sensorThreshold[5] && sensorValues[6] < sensorThreshold[6] && sensorValues[7] < sensorThreshold[7];
      
    // If motion is complete, update the line position based on the detected flags
    if (motionComplete)
    { 
        if (leftTurn)
        {
            linePosition = LEFT_LINE;
        }
        else if (rightTurn)
        {
            linePosition = RIGHT_LINE;
        }
        else if (deadEnd)
        {
            linePosition = NO_LINE;
        }
        else if (tJunctionOrBase)
        {
            linePosition = T_JUNCTION;
        }
        else
        {
            linePosition = CENTER_LINE;
        }
    }
}