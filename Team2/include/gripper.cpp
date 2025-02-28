void openGripper()
{
    setGripperActionColor();
    gripper.write(122);   // Move to open position
}

void closeGripper()
{
    setGripperActionColor();
    gripper.write(42);    // Move to closed position
}

void gripperSetup()
{
    if (gripper.read() < 122)
    {
        openGripper();
    }
}