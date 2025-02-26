// Drive with calibration
int calibrate(int n, int offset)
{
    int calibratedValue = n - offset; // Subtract the offset from the input value
    return (calibratedValue < 0) ? 0 : calibratedValue; // Ensure the value doesn't go below 0
}

void drive(int motorAForward, int motorABackward, int motorBForward, int motorBBackward)
{
    analogWrite(MOTOR_A_FORWARD, calibrate(motorAForward, CALIBRATION_OFFSET_A));
    analogWrite(MOTOR_A_BACKWARD, calibrate(motorABackward, CALIBRATION_OFFSET_A));
    analogWrite(MOTOR_B_FORWARD, calibrate(motorBForward, CALIBRATION_OFFSET_B));
    analogWrite(MOTOR_B_BACKWARD, calibrate(motorBBackward, CALIBRATION_OFFSET_B));
}

void driveForward()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
    drive(255, 0, 255, 0);
}

void driveBackward()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
    NeoPixel.show();
    drive(0, 255, 0, 255);
}

void turnRight()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 100, 0);
    NeoPixel.show();
    drive(0, 255, 255, 0);
}

void turnLeft()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
    drive(255, 0, 0, 255);
}

void driveStop()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
    NeoPixel.show();
    drive(0, 0, 0, 0);
}