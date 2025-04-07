#include "mpu9250.h"

bfs::Mpu9250 imu; // MPU 9250 object

// Bluetooth pin
#define BTPIN 1

// Left motor
#define IN1 6
#define IN2 10
// Right motor
#define IN3 5
#define IN4 9

// Turn parameters
const float TURN_ANGLE = 90.0f; // degrees
const float TURN_THRESHOLD = 12.0f; // degrees tolerance
const int BASE_SPEED = 255; // Base motor speed (0-255)
const float TURN_FACTOR = 0.65f; // How much to reduce inside wheel speed (0-1)


class DistanceSensor
{
  private:
    int trigPin;
    int echoPin;
    const int maxDistance = 100; //set to -1 if you want no max Distance

    // gets raw data from the sensor
    float getPulseDuration()
    {

      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      if (maxDistance == -1)
      {
        return pulseIn(echoPin, HIGH);
      } else {
        return pulseIn(echoPin, HIGH, 60 * maxDistance); //24000 is for about 400cm, so if you want a limit of one meter just set it to 6000
      }
    }

    // gets the distance measured by the sensor
    float getProcessedDistance()
    {
      float pulseDuration = getPulseDuration();
      float distance;
      if (pulseDuration > 100) {
        distance = (pulseDuration * .0343) / 2;
      } else {
        distance = maxDistance;
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

    double getCuredDistance(int sampleNum = 7, int removedSamplesMarginWidth = 1)
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
      // to remove the smallest measurements taken, the entire array will be shifted to the front
      for (int index = 0; index < sampleNum; index++)
      {
        samples[index] = samples[index + removedSamplesMarginWidth];
      }
      // to remove the largest measurements taken, sampleNum will be decreased, so the last elements will just be ignored
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

DistanceSensor sensorForward(4, 7);
DistanceSensor sensorLeft(8, 13);
DistanceSensor sensorRight(2, 3);


const int wait = 0;
const int forward = 1;
const int left = 2;
const int right = 3;
const int rotate = 4;

int navigationState = forward;


float _Kp = 0.08;
float _Ki = 0.01;
float _Kd = 0.02;
int _previousError = 0;
int _integral = 0;
int _baseSpeed = 220;

unsigned long triggerTime = 0;
int interval = 200;
int latestTriggerTime = 0;


// Function to turn left while moving forward

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);

  imu.Config(&Wire, bfs::Mpu9250::I2C_ADDR_PRIM);
  if (!imu.Begin() || !imu.ConfigSrd(19)) {
    Serial.println("IMU init failed");
    while (1);
  }

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(BTPIN, OUTPUT);

  // Initialize IMU
  Wire.begin();
  Wire.setClock(400000);
  imu.Config(&Wire, bfs::Mpu9250::I2C_ADDR_PRIM);
  if (!imu.Begin()) {
    Serial.println("Error initializing communication with IMU");
    while (1) {}
  }
  if (!imu.ConfigSrd(19)) {
    Serial.println("Error configuring SRD");
    while (1) {}
  }
}

void loop() {
  while (true) {
    rotate180();
    delay(1000);
  }



  processNavigationState();

  //check what status do we have right now
  switch (navigationState)
  {
    case wait:
      motor_drive(0, 0);
      break;
    case forward:
      //forward state corresponds to going forward, which means that the robot only cares about holding onto a wall using PID

      if (millis() > triggerTime) {
        latestTriggerTime = millis();
        triggerTime = millis() + interval;

        int distanceLeft = sensorLeft.getCuredDistance();
        /*
          PID_WallFollow expects a value from -3500 to 3500
          7.5cm is 0

        */
        int error = constrain((distanceLeft - 8.25), -10, 10);
        error = error * 350;

        PID_WallFollow(error);
        Serial.println(
          //      "Distance=" + String(distanceLeft) +
          "Error= " + String(error)
        );
      }
      break;
    case left:
      turn_degrees(80.0);
      navigationState = forward;
      break;
    case right:
      turn_degrees(-80.0);
      navigationState = forward;
      break;
    case rotate:
      motor_drive(-255, -255);
      break;
  }
}

void processNavigationState() {
  //the maze is about 29.5cm wide, but between pillars it is 28cm
  //so the center of the robot should be at a distance of 14.75
  //the robot width(including both side sensors) is 13cm
  //so the offset of a side sensor is 6.5cm
  //we update navigationState here
  int distanceLeft = sensorLeft.getCuredDistance();
  if (distanceLeft >= 25)
  {
    //turn left if possible
    Serial.println("Started turning left, Ldistance=" + String(distanceLeft));
    //    motor_drive(0,0);
    //    delay(10000);
    navigationState = left;
  } else {
    int distanceForward = sensorForward.getCuredDistance();
    if (distanceForward > 20)
    {
      navigationState = forward;
    } else {
      int distanceRight = sensorRight.getCuredDistance();
      if (distanceRight >= 25)
      {
        navigationState = right;
      } else {
        navigationState = rotate;
      }

    }

  }
}

void PID_WallFollow(int error) {
  int P = error;
  _integral += error;
  _integral = constrain(_integral, -1000, 1000);
  int D = error - _previousError;

  int PIDvalue = (_Kp * P) + (_Ki * _integral) + (_Kd * D);
  _previousError = error;

  int leftSpeed = _baseSpeed - PIDvalue;
  int rightSpeed = _baseSpeed + PIDvalue;

  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  motor_drive(leftSpeed, rightSpeed);
}

void turn_degrees(float target_degrees) {
  Serial.println("Started turning " + String(target_degrees));
  // Reset IMU data
  imu.Read();

  float current_angle = 0;
  float initial_angle = 0;
  bool first_reading = true;
  unsigned long turn_start = millis();

  // Determine turn direction (left or right)
  int direction = (target_degrees > 0) ? 1 : -1;

  while ( target_degrees > 0 ? ((current_angle - target_degrees) < - TURN_THRESHOLD) : ( (current_angle - target_degrees) > TURN_THRESHOLD ) ) {
    Serial.println("current_angle - target_degrees=" + String(current_angle - target_degrees) + " > TURN_THRESHOLD" + String(TURN_THRESHOLD));
    if (imu.Read()) {
      float gyro_z = imu.gyro_z_radps() * (180.0 / PI); // Convert to degrees/sec

      if (first_reading) {
        initial_angle = 0;
        first_reading = false;
      } else {
        // Integrate gyro reading to get angle
        unsigned long now = millis();
        static unsigned long last_time = now;
        float dt = (now - last_time) / 1000.0f; // seconds
        last_time = now;

        current_angle += gyro_z * dt;
      }

      // Soft turn - both motors active but at different speeds
      int left_speed = BASE_SPEED;
      int right_speed = BASE_SPEED;

      if (direction > 0) {
        // Left turn - reduce left motor speed
        left_speed = BASE_SPEED * (1.0 - TURN_FACTOR);
      } else {
        // Right turn - reduce right motor speed
        right_speed = BASE_SPEED * (1.0 - TURN_FACTOR);
      }

      //        Serial.println("TURN_FACTOR"+String(TURN_FACTOR));

      motor_drive(left_speed, right_speed);
      //      Serial.println("current_angle - target_degrees=" + String(current_angle - target_degrees) + " > TURN_THRESHOLD" + String(TURN_THRESHOLD) + "------left_speed" + String(left_speed) + "right_speed" + String(right_speed));
    }
  }
  Serial.println("Finished turning, (in turn_degrees)");
}

void rotate180() {
  Serial.println("Starting 180-degree rotation");
  imu.Read(); // Reset IMU data

  const float TARGET_DEGREES = 180.0f;
  const float ROTATION_THRESHOLD = 5.0f; // degrees of tolerance
  const int ROTATION_SPEED = BASE_SPEED; 

  float current_angle = 0;
  float initial_angle = 0;
  bool first_reading = true;
  int active_wheel = 1; // 1 = right wheel, 2 = left wheel

  unsigned long last_time = millis();

  while ( -current_angle - TARGET_DEGREES < -ROTATION_THRESHOLD) {
    Serial.println(String(current_angle)+"-"+String(TARGET_DEGREES)+"<"+String(-ROTATION_THRESHOLD));
    if (imu.Read()) {
      float gyro_z = imu.gyro_z_radps() * (180.0 / PI); // Convert to degrees/sec

      if (first_reading) {
        initial_angle = 0;
        first_reading = false;
      } else {
        // Integrate gyro reading to get angle
        unsigned long now = millis();
        static unsigned long last_time = now;
        float dt = (now - last_time) / 1000.0f; // seconds
        last_time = now;

        current_angle += gyro_z * dt;
      }
      if (active_wheel == 1) {
        //right wheel goes backwards while the robot didn't reach TARGET_DEGREES and it is still rotating
        motor_drive(0, -ROTATION_SPEED);
        delay(100);
        if (abs(gyro_z) > 1.0f) { // 1 degree/sec threshold
          // Wait until rotation stops
          while (abs(gyro_z) > 1.0f && current_angle - TARGET_DEGREES < -ROTATION_THRESHOLD) {
            if (imu.Read()) {
              gyro_z = imu.gyro_z_radps() * (180.0 / PI);
              delay(10); // small delay to prevent busy-wait
            }
          }
          motor_drive(0, 0); // Check if we've started moving
          active_wheel = 2;
        }
      } else {
        //right wheel goes backwards while the robot didn't reach TARGET_DEGREES and it is still rotating
        motor_drive(ROTATION_SPEED, 0);
        delay(100);
        if (abs(gyro_z) > 1.0f && current_angle - TARGET_DEGREES < -ROTATION_THRESHOLD) {
          // Wait until rotation stops
          while (abs(gyro_z) > 1.0f) {
            if (imu.Read()) {
              gyro_z = imu.gyro_z_radps() * (180.0 / PI);
              delay(10);
            }
          }
          active_wheel = 1;
        }
      }
    }
  }
  // Stop motors when done
  motor_drive(0, 0);
  Serial.println("Finished 180-degree rotation");
}

void motor_drive(int left, int right) {
  if (abs(left) < 50) left = 0;
  if (abs(right) < 50) right = 0;

  if (left >= 0) {
    analogWrite(IN1, left);
    analogWrite(IN2, 0);
  } else {
    analogWrite(IN1, 0);
    analogWrite(IN2, -left);
  }

  if (right >= 0) {
    analogWrite(IN3, right);
    analogWrite(IN4, 0);
  } else {
    analogWrite(IN3, 0);
    analogWrite(IN4, -right);
  }
  //  Serial.print(left);
  //  Serial.print("   ");
  //  Serial.print(right);
  //  Serial.println("");
}
