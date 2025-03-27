/**
 *  Controls the gripper servo motor by generating a PWM signal.
 *
 *  This function generates a PWM signal to control a servo motor connected to the gripper.
 *  It updates the PWM signal at a fixed interval of 20 milliseconds.
 *
 *  The pulse width in microseconds to control the servo position. If the pulse
 *  width is greater than 0, it updates the last pulse width. If the pulse width
 *  is 0 or less, it uses the last pulse width.
 **/

 void gripper(int pulse)
 {
     static unsigned long timer;
     static int lastPulse;
     if (millis() > timer)
     {
         if (pulse > 0)
         {
             lastPulse = pulse;
         }
         else
         {
             pulse = lastPulse;
         }
 
         digitalWrite(SERVO, HIGH);
         delayMicroseconds(pulse);
         digitalWrite(SERVO, LOW);
         timer = millis() + 20;
     }
 }

void openGripper()
{
    gripper(GRIPPER_OPEN);
    setGripperActionColor();
}

void closeGripper()
{
    gripper(GRIPPER_CLOSE);
}