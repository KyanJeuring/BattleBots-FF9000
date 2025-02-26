void readBluetooth()
{
    if(Serial.available() > 0)
    {
        char command = Serial.read();
        Serial.println(command);

        switch (command) {
            case 'F':
                driveForward();
                break;
            case 'B':
                driveBackward();
                break;
            case 'L':
                turnLeft();
                break;
            case 'R':
                turnRight();
                break;
            case 'S':
                driveStop();
                break;
            default:
                break;
        }
    }
}