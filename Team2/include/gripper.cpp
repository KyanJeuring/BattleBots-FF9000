void openGripper()
{
    setGripperActionColor();
    myServo.write(122);   // Move to open position
}

void closeGripper()
{
    setGripperActionColor();
    myServo.write(42);    // Move to closed position
}