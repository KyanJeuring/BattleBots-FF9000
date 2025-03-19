// Left rotation sensor interrupt service routine (ISR)
void leftEncoderISR()
{
    static unsigned long timer;
    if (millis() > timer)
    {
        _leftTicks++; // Increment left ticks
        timer = millis() + ISR_INTERVAL; // Update timer
    }
}

// Right rotation sensor interrupt service routine (ISR)
void rightEncoderISR()
{
    static unsigned long timer;
    if (millis() > timer)
    {
        _rightTicks++; // Increment right ticks
        timer = millis() + ISR_INTERVAL; // Update timer
    }
}

// Move the robot forward with specified speeds for left and right motors
void moveForward(int _leftSpeed, int _rightSpeed)
{
    analogWrite(MOTOR_A_FORWARD, _rightSpeed);
    analogWrite(MOTOR_A_BACKWARD, 0);
    analogWrite(MOTOR_B_FORWARD, _leftSpeed);
    analogWrite(MOTOR_B_BACKWARD, 0);
}

// Stop both motors
void stopMotors()
{
    analogWrite(MOTOR_A_FORWARD, 0);
    analogWrite(MOTOR_A_BACKWARD, 0);
    analogWrite(MOTOR_B_FORWARD, 0);
    analogWrite(MOTOR_B_BACKWARD, 0);
    setDriveStopColor();
    NeoPixel.show();
}

// Turn the robot 180 degrees
void turn180(int _leftSpeed, int _rightSpeed)
{
    analogWrite(MOTOR_A_BACKWARD, _rightSpeed);
    analogWrite(MOTOR_A_FORWARD, 0);
    analogWrite(MOTOR_B_FORWARD, _leftSpeed);
    analogWrite(MOTOR_B_BACKWARD, 0);
}

// Move the robot forward using PID control
void moveForwardPID(int _leftSpeed, int _rightSpeed, bool withOutLine, bool lineTracking)
{
    if (withOutLine)
    {
        // PID Variables
        Kp = 6.5; // Proportional Gain
        Ki = 0.1; // Integral Gain
        Kd = 5;   // Derivative Gain

        // Calculate error
        error = _leftTicks - _rightTicks;

        // Update PID terms
        integral += error;
        derivative = error - lastError;
        lastError = error;

        // Prevent integral windup
        integral = constrain(integral, -10, 10);
    }
    else if (lineTracking)
    {
        readSensors();
        int position = calculateLinePosition();

        // PID Variables
        Kp = 0.4;     // Proportional Gain
        Ki = 0.00001; // Integral Gain
        Kd = 0.1;     // Derivative Gain

        int center = (NUM_SENSORS - 1) * 1000 / 2; // Midpoint of sensor array

        // Calculate error
        error = position - center;

        // Update PID terms
        integral += error;
        derivative = error - lastError;
        lastError = error;
    }

    // Apply PID correction
    correction = (Kp * error) + (Ki * integral) + (Kd * derivative);
    _leftSpeed -= correction;
    _rightSpeed += correction;

    // Ensure PWM values are within valid range (0 - 255)
    _leftSpeed = constrain(_leftSpeed, 0, 255);
    _rightSpeed = constrain(_rightSpeed, 0, 255);

    // Move motors
    moveForward(_leftSpeed, _rightSpeed);
}

// Turn the robot left by a specified angle using a time-based approach
void turnLeftMillis(int angle)
{
    static unsigned long lastCheck = 0;
    const unsigned long checkInterval = 5;
    static int targetPulses;

    if (robotState != TURNING_LEFT)
    {
        resetTicks();
        int targetPulses = 0;
        float turnDistance = (angle / 360.0) * turn_Circumference;
        targetPulses = (turnDistance / WHEEL_CIRCUMFERENCE) * PULSE_PER_REVOLUTION;
        Serial.print("Target Pulses: ");
        Serial.println(targetPulses);
        stopMotors();
        moveForward(200, 0);
        robotState = TURNING_LEFT; // Lock state to "TURNING_LEFT"
        motionComplete = false;
    }

    if (robotState == TURNING_LEFT)
    {
        lastCheck = millis();
        Serial.print("Turning Left: ");
        Serial.println(robotState);
        if (_rightTicks >= targetPulses)
        {
            stopMotors();
            robotState = FOLLOW_LINE; // Unlock state after turn is complete
            Serial.print("Turn Complete ");
            motionComplete = true;
            linePosition = CENTER_LINE;
        }
    }
}

// Turn the robot right by a specified angle using a time-based approach
void turnRightMillis(int angle)
{
    static unsigned long lastCheck = 0;
    const unsigned long checkInterval = 5;
    static int targetPulses;

    if (robotState != TURNING_RIGHT)
    {
        resetTicks(); // Reset left encoder ticks
        targetPulses = 0;
        float turnDistance = (angle / 360.0) * turn_Circumference;
        targetPulses = (turnDistance / WHEEL_CIRCUMFERENCE) * PULSE_PER_REVOLUTION;
        Serial.print("Target Pulses: ");
        Serial.println(targetPulses);
        stopMotors();
        moveForward(200, 0);        // Right turn: left motor moves forward, right motor stops
        robotState = TURNING_RIGHT; // Lock state to "TURNING_RIGHT"
        motionComplete = false;
    }

    if (robotState == TURNING_RIGHT && millis() - lastCheck >= checkInterval)
    {
        lastCheck = millis();
        readSensors();
        if (_leftTicks >= targetPulses)
        {
            stopMotors();
            robotState = FOLLOW_LINE; // Unlock state after turn is complete
            Serial.print("Turn Complete ");
            motionComplete = true;
            linePosition = CENTER_LINE;
        }
    }
}

// Turn the robot around (180 degrees) using a time-based approach
void turnAroundMillis()
{
    static unsigned long lastCheck = 0;
    const unsigned long checkInterval = 5;
    static int targetPulses;
    if (robotState != TURNING_AROUND)
    {
        resetTicks();
        targetPulses = 0;

        float turnDistance = (3.14 * (DISTANCE_BETWEEN_WHEELS / 2)); // Half the turning circumference
        targetPulses = (turnDistance / WHEEL_CIRCUMFERENCE) * PULSE_PER_REVOLUTION;
        Serial.print("Target Pulses: ");
        Serial.println(targetPulses);

        turn180(200, 200); // Left wheel moves backward, right moves forward

        robotState = TURNING_AROUND; // Lock state to "TURNING_AROUND"
        motionComplete = false;
    }

    if (robotState == TURNING_AROUND)
    {
        Serial.print("Target Pulses: ");
        Serial.println(targetPulses);
        sensorValues[0] = analogRead(sensorPins[0]);
        if (sensorValues[0] > sensorThreshold[0] || sensorValues[4] > sensorThreshold[4])
        {
            stopMotors();
            robotState = FOLLOW_LINE; // Unlock state after turn is complete
            Serial.println("Turn Around Complete");
            motionComplete = true;
            linePosition = CENTER_LINE;
        }
    }
}