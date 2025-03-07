#include <Arduino.h>
#define SERVO         9     //define servo control pin
#define GRIPPER_OPEN  1800  //value for opening the gripper
#define GRIPPER_CLOSE 990   //value for closing gripper  

// define motor control pins 
const int MOTOR_A_FORWARD = 10;
const int MOTOR_B_FORWARD = 6;

//calibration offset for motors
const int CALIBRATION_OFFSET_A = 14;
const int CALIBRATION_OFFSET_B = 5;

unsigned long startTime; //define variable for millis

void setup() {
  Serial.begin(9600); // Initialize communication for debugging
  //setup servo pin
  pinMode(SERVO, OUTPUT);
  digitalWrite(SERVO, LOW);
  //setup motor pins
  pinMode(MOTOR_A_FORWARD, OUTPUT);
  pinMode(MOTOR_B_FORWARD, OUTPUT);

  digitalWrite(MOTOR_A_FORWARD, LOW);
  digitalWrite(MOTOR_B_FORWARD, LOW);

  startTime = millis(); //record the start time
}

void loop()
{
  unsigned long currentMillis = millis(); //get the current time

//move forward and open gripper
  if (currentMillis - startTime < 1500) {
    Serial.println("Go forward, gripper open");
    drive(255, 255);
    gripper(GRIPPER_OPEN);

  } //stop for 2 seconds and close gripper
  else if (currentMillis - startTime < 3000){
    Serial.println("Stop, gripper close");
    drive(0, 0);
    gripper(GRIPPER_CLOSE);

  } //go gorward for 3 seconds
  else if (currentMillis - startTime < 6000) {
    Serial.println("Go forward");
    drive(255, 255);
  } //stop open gripper
  else {
    Serial.println("Stop, gripper open");
    drive(0, 0);
    gripper(GRIPPER_OPEN);

  }
}

//function to control motor speed
void drive(int motorAForward, int motorBForward) {
  //set the motor speed and calibrate
  analogWrite(MOTOR_A_FORWARD, calibrate(motorAForward, CALIBRATION_OFFSET_A));
  analogWrite(MOTOR_B_FORWARD, calibrate(motorBForward, CALIBRATION_OFFSET_B));
}

//function to calibrate/adjust motor speed
int calibrate(int n, int offset) {
  int calibratedValue = n - offset;
  return (calibratedValue < 0) ? 0 : calibratedValue;
}

//function to control gripper
void gripper(int pulse) {
  static unsigned long timer;
  static int lastPulse;
  if (millis() > timer) {
    if (pulse  == 0) {
      pulse = lastPulse;
    } else {
      lastPulse = pulse;
    }
    digitalWrite(SERVO, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(SERVO, LOW);

    timer = millis() + 20; //reset the timer for next pulse
  }
}