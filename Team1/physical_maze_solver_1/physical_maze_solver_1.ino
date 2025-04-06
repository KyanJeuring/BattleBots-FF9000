//distance between walls is 30cm
//robot is about 15cm wide

#include <Adafruit_NeoPixel.h>

//---------------------------------------------------------------STATE VARIABLES
boolean       isStartSequenceActive     =         false;
boolean       isMazeNavigationActive    =         false;

//---------------------------------------------------------------LINE SENSORS
const int     LINE_SENSOR_PINS[]        =         {A0, A1, A2, A3, A4, A5, A6, A7};
#define       BLACK_LINE_THRESHOLD                800

//---------------------------------------------------------------GRIPPER
#define       GRIPPER_SERVO_PIN                   11
#define       SERVO_OPEN_POSITION                 1600
#define       SERVO_CLOSE_POSITION                970
#define       SERVO_CYCLE_REPEAT                  10

//---------------------------------------------------------------DistanceSensor class
class DistanceSensor
{
  private:
    int trigPin;
    int echoPin;
    const int MAX_DISTANCE = 100; //set to -1 if you want no max Distance

    // gets raw data from the sensor
    float getPulseDuration()
    {

      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      if (MAX_DISTANCE == -1)
      {
        return pulseIn(echoPin, HIGH);
      } else {
        return pulseIn(echoPin, HIGH, 60 * MAX_DISTANCE);
        //for MAX_DISTANCE 24000 is for about 400cm,
        // so if you want a limit of one meter just set it to 6000
      }
    }

    // gets the distance measuLED_COLOR_RED by the sensor
    float getProcessedDistance()
    {
      float pulseDuration = getPulseDuration();
      float distance;
      if (pulseDuration > 100) {
        distance = (pulseDuration * .0343) / 2;
      } else {
        distance = MAX_DISTANCE;
      }
      return distance;
    }

  public:
    //constructor
    DistanceSensor(int tPin, int ePin)
    {
      trigPin = tPin;
      echoPin = ePin;
      pinMode(trigPin, OUTPUT);
      digitalWrite(trigPin, HIGH);
      pinMode(echoPin, INPUT);
    }

    double getCuLED_COLOR_REDDistance(int sampleNum = 7, int removedSamplesMarginWidth = 1)
    {
      int samples[sampleNum];

      // get all the samples
      for (int index = 0; index < sampleNum; index++)
      {
        samples[index] = getProcessedDistance();
      }

      // sort the samples using insert sort
      int key;
      for (int i = 1; i < sampleNum; i++)
      {
        key = samples[i];
        int j = i - 1;
        while (j >= 0 && samples[j] > key)
        {
          samples[j + 1] = samples[j];
          j = j - 1;
        }
        samples[j + 1] = key;
      }

      // remove margins; after this the length of the array will change
      // to remove the smallest measurements taken,
      // the entire array will be shifted to the front
      for (int index = 0; index < sampleNum; index++)
      {
        samples[index] = samples[index + removedSamplesMarginWidth];
      }
      // to remove the largest measurements taken, 
      // sampleNum will be decreased, so the last elements will just be ignored
      sampleNum = sampleNum - removedSamplesMarginWidth * 2;

      // calculate average of remaining samples
      double average;
      double sum = 0;
      for (int index = 0; index < sampleNum; index++) {
        sum += samples[index];
      }
      average = sum / sampleNum;
      // return the result
      return average;
    }
};

DistanceSensor distanceSensorFront = DistanceSensor(4, 7);
int distanceFront;

DistanceSensor distanceSensorLeft = DistanceSensor(8, 13);
int distanceLeft;

//---------------------------------------------------------------MOTOR DISTANCES
#define       IN1                                 6             //Left motor forward
#define       IN2                                 10            //Left motor backwards
#define       IN3                                 5             //Right motor forward
#define       IN4                                 9             //Right motor backwards
#define       MOTOR_STALL_TIMEOUT                 1700
#define       MAX_MOTOR_SPEED                     255

//---------------------------------------------------------------MOTOR ENCODERS
#define       LEFT_ENCODER_PIN                    2
#define       RIGHT_ENCODER_PIN                   3
int           leftEncoderCount            =       0;
int           rightEncoderCount           =       0;
int           totalLeftEncoderCount       =       0, previousleftEncoderCount;
int           totalRightEncoderCount      =       0, previousrightEncoderCount;

//---------------------------------------------------------------statusLEDs
#define       NEOPIXEL_PIN                        12
Adafruit_NeoPixel statusLEDs(4, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);
const uint32_t  LED_COLOR_RED             =       statusLEDs.Color(255, 0, 0);
const uint32_t  LED_COLOR_YELLOW          =       statusLEDs.Color(255, 150, 0);
const uint32_t  LED_COLOR_BLUE            =       statusLEDs.Color(0, 0, 255);
const uint32_t  LED_COLOR_WHITE           =       statusLEDs.Color(255, 255, 255);
const uint32_t  LED_COLOR_OFF             =       statusLEDs.Color(0, 0, 0);

//---------------------------------------------------------------FUNCTIONS
void updateFrontDistance() {
  distanceFront = distanceSensorFront.getCuLED_COLOR_REDDistance();
}

void updateLeftDistance() {
  distanceLeft = distanceSensorLeft.getCuLED_COLOR_REDDistance();
}

void lineSensorsPinsInit()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
}

void pauseExecutionForMillis(int duration)
{
  unsigned long startTime = millis();
  while (millis() < startTime + duration)
  {
    //do nothing
  }
}

boolean reachedBlackZone()
{
  int count = 0;
  for (int i = 0; i < 8; i++)
  {
    if (analogRead(LINE_SENSOR_PINS[i]) > BLACK_LINE_THRESHOLD)
    {
      count++;
    }
  }
  if (count < 6)
  {
    return false;
  }
  return true;
}


void setServo(int pulse)
{
  for (int i = 0; i < SERVO_CYCLE_REPEAT; i++)
  {
    digitalWrite(GRIPPER_SERVO_PIN, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(GRIPPER_SERVO_PIN, LOW);
    delay(20);
  }
}

void openGripper()
{
  setServo(SERVO_OPEN_POSITION);
}

void closeGripper()
{
  setServo(SERVO_CLOSE_POSITION );
}

void setupMotorPins()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void driveForward()
{
  statusLEDs.fill(LED_COLOR_WHITE, 0, 4);
  statusLEDs.show();

  analogWrite(IN1,    MAX_MOTOR_SPEED * 0.9   );
  analogWrite(IN2,    0                       );
  analogWrite(IN3,    MAX_MOTOR_SPEED         );
  analogWrite(IN4,    0                       );
}

void driveForwardSlow()
{
  statusLEDs.fill(LED_COLOR_WHITE, 0, 4);
  statusLEDs.show();

  analogWrite(IN1,    MAX_MOTOR_SPEED * 0.6   );
  analogWrite(IN2,    0                       );
  analogWrite(IN3,    MAX_MOTOR_SPEED * 0.6   );
  analogWrite(IN4,    0                       );
}

void driveBackwardsRotate()
{
  analogWrite(IN1,    0                        );
  analogWrite(IN2,    MAX_MOTOR_SPEED * 0.4    );
  analogWrite(IN3,    0                        );
  analogWrite(IN4,    MAX_MOTOR_SPEED          );
}

void driveBackwards()
{
  analogWrite(IN1,    0                        );
  analogWrite(IN2,    MAX_MOTOR_SPEED          );
  analogWrite(IN3,    0                        );
  analogWrite(IN4,    MAX_MOTOR_SPEED          );
}

void stopMotors()
{
  analogWrite(IN1,    0                        );
  analogWrite(IN2,    0                        );
  analogWrite(IN3,    0                        );
  analogWrite(IN4,    0                        );
}

void turnLeft()
{
  statusLEDs.fill(LED_COLOR_RED, 0, 4);
  statusLEDs.show();

  analogWrite(IN1,    0                        );
  analogWrite(IN2,    0                        );
  analogWrite(IN3,    MAX_MOTOR_SPEED * 0.8    );
  analogWrite(IN4,    0                        );
}

void shiftLeft()
{
  analogWrite(IN1,    MAX_MOTOR_SPEED          );
  analogWrite(IN2,    0                        );
  analogWrite(IN3,    MAX_MOTOR_SPEED * 0.7    );
  analogWrite(IN4,    0                        );
}

void shiftRight()
{
  analogWrite(IN1,    MAX_MOTOR_SPEED * 0.6    );
  analogWrite(IN2,    0                        );
  analogWrite(IN3,    MAX_MOTOR_SPEED          );
  analogWrite(IN4,    0                        );
}

void rotate()
{
  analogWrite(IN1,    MAX_MOTOR_SPEED * 0.8    );
  analogWrite(IN2,    0                        );
  analogWrite(IN3,    0                        );
  analogWrite(IN4,    MAX_MOTOR_SPEED * 0.8    );
}

void CountEncoder1()
{
  noInterrupts();
  leftEncoderCount++;
  interrupts();
}

void CountEncoder2()
{
  noInterrupts();
  rightEncoderCount++;
  interrupts();
}

void turnLeftOnPulses(int targetEncoderCount)
{
  stopMotors();
  leftEncoderCount = 0;
  rightEncoderCount = 0;

  int lastrightEncoderCount = -1;
  int lastleftEncoderCount = -1;
  long stallRecoveryDeadline = millis() + MOTOR_STALL_TIMEOUT;

  updateLeftDistance();
  turnLeft();

  while (
    (leftEncoderCount < targetEncoderCount && rightEncoderCount < targetEncoderCount)
    && distanceLeft >= 10)
  {
    if (
      leftEncoderCount == lastleftEncoderCount
      &&
      rightEncoderCount == lastrightEncoderCount)
    {
      //wheels didn't spin
      if (millis() > stallRecoveryDeadline && distanceLeft > 10)
      {
        //robot has been stuck for a while
        stallRecoveryDeadline = millis() + MOTOR_STALL_TIMEOUT;
        statusLEDs.fill(LED_COLOR_WHITE, 0, 4);
        statusLEDs.show();
        driveBackwardsRotate();
        pauseExecutionForMillis(400);
        stopMotors();
      }
    }
    else
    {
      stallRecoveryDeadline = millis() + MOTOR_STALL_TIMEOUT;
      lastleftEncoderCount = leftEncoderCount;
      lastrightEncoderCount = rightEncoderCount;
    }
    updateFrontDistance();
    updateLeftDistance();
  }
  stopMotors();
}

void driveForwardOnPulses(int targetEncoderCount)
{
  stopMotors();
  leftEncoderCount = 0;
  rightEncoderCount = 0;

  int lastrightEncoderCount = -1;
  int lastleftEncoderCount = -1;
  long stallRecoveryDeadline = millis() + MOTOR_STALL_TIMEOUT;

  if (distanceLeft <= 5)
  {
    shiftLeft();
  }
  else if (distanceLeft >= 13 && distanceLeft < 20)
  {
    shiftRight();
  }
  else
  {
    driveForward();
  }
  while ((leftEncoderCount < targetEncoderCount && rightEncoderCount < targetEncoderCount))
  {
    if (
      leftEncoderCount == lastleftEncoderCount
      &&
      rightEncoderCount == lastrightEncoderCount)
      
    { //wheel has not pulsed yet
      if (millis() > stallRecoveryDeadline)
      { //if the robot not moved for duration
        stallRecoveryDeadline = millis() + MOTOR_STALL_TIMEOUT;
        statusLEDs.fill(LED_COLOR_WHITE, 0, 4);
        statusLEDs.show();
        driveBackwards();
        pauseExecutionForMillis(150);
        driveBackwardsRotate();
        pauseExecutionForMillis(250);
      }
    }
    else
    {
      stallRecoveryDeadline = millis() + MOTOR_STALL_TIMEOUT;
      lastleftEncoderCount = leftEncoderCount;
      lastrightEncoderCount = rightEncoderCount;
    }
    distanceFront = distanceSensorFront.getCuLED_COLOR_REDDistance();
    distanceLeft = distanceSensorLeft.getCuLED_COLOR_REDDistance();
  }
  //  stopMotors();
  driveForwardSlow();
}

//---------------------------------------------------------------SETUP
void setup()
{
  pinMode(LEFT_ENCODER_PIN , INPUT);
  pinMode(RIGHT_ENCODER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN ), CountEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), CountEncoder2, CHANGE);
  lineSensorsPinsInit();
  statusLEDs.begin();
  statusLEDs.fill(LED_COLOR_BLUE, 0, 4);
  statusLEDs.show();
  leftEncoderCount = 0;
  rightEncoderCount = 0;
}

//---------------------------------------------------------------MAIN LOOP
void loop()
{
  if (isMazeNavigationActive)
  {
    closeGripper();
    updateFrontDistance();
    updateLeftDistance();

    if (reachedBlackZone())
    {
      //In case that there is a line, follow line
      stopMotors();
      openGripper();
      pauseExecutionForMillis(300);
      isMazeNavigationActive = false;
      isStartSequenceActive = false;
      driveBackwards();
      pauseExecutionForMillis(1000);
      stopMotors();
      pauseExecutionForMillis(60000);
    }
    if (distanceLeft <= 17 && distanceFront >= 12)
    {
      //In case the robot can't go left, can only go forward
      driveForwardOnPulses(10);
    }
    else if (distanceLeft > 17 && distanceFront > 12)
    {
      //In case the robot can go left and forward, will choose left
      turnLeftOnPulses(36);
      pauseExecutionForMillis(200);
      driveForwardOnPulses(20);
      turnLeftOnPulses(36);
    }
    else
    {
      //In case the robot can't go left, can't go forward, will rotate to the right
      statusLEDs.fill(LED_COLOR_YELLOW, 0, 4);
      statusLEDs.show();
      driveBackwards();
      pauseExecutionForMillis(100);
      stopMotors();
      while (distanceFront < 15)
      {
        rotate();
        updateFrontDistance();
      }
    }
  }
  else
  {
    if (isStartSequenceActive == true)
    {
      driveForward();
      pauseExecutionForMillis(800);
      closeGripper();
      leftEncoderCount = 0;
      rightEncoderCount = 0;
      turnLeftOnPulses(36);
      driveForwardOnPulses(70);
      isMazeNavigationActive = true;
    }
    else
    {
      pauseExecutionForMillis(1000);
      updateFrontDistance();
      if (distanceFront < 30)
      {
        openGripper();
        pauseExecutionForMillis(1500);
        isStartSequenceActive = true;
      }
    }
  }
}
