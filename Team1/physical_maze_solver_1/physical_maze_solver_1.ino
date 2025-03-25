//distance between walls is 30cm
//robot is about 15cm wide






//Bluetooth pin
#define BTPIN 1

// Left motor
//IN1 is for turning the left wheel forward
#define IN1 6
//IN2 is for turning the left wheel backwards
#define IN2 10
// Right motor
//IN3 is for turning the left wheel forward
#define IN3 5
//IN4 is for turning the left wheel backwards
#define IN4 9

class DistanceSensor
{
  private:
    int trigPin;
    int echoPin;

    // gets raw data from the sensor
    float getPulseDuration()
    {

      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      return pulseIn(echoPin, HIGH);
    }

    // gets the distance measured by the sensor
    float getProcessedDistance()
    {
      float pulseDuration = getPulseDuration();
      float distance = (pulseDuration * .0343) / 2;
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

//Distance sensors
DistanceSensor sensorForward(2, 4);
DistanceSensor sensorLeft(7, 8);
DistanceSensor sensorRight(11, 12);


// PID constants
float _Kp = 0.12; // Increased Proportional Gain slightly   /Proportional gain     corrects error
float _Ki = 0.001; // Small Integral Gain to fix slow drifting  /Integral gain         corrects small errors, that are long term (slightly being off the line)
float _Kd = 0.02; // Reduced Derivative Gain  /Derivate gain         dampens the corrections

int _previousError = 0;
int _integral = 0;
int _baseSpeed = 220; // Adjust based on motor power

int triggerTime = 0;
int interval = 400;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(BTPIN, OUTPUT);
}

void loop() {
  if (millis() > triggerTime) {
    triggerTime = millis() + interval;

    int position = readSensors();
    int error = position;

    PID_Linefollow(error);

    int distanceLeft = sensorLeft.getCuredDistance();

    int distanceForward = sensorForward.getCuredDistance();

    Serial.println("Left= " + String(distanceLeft) + " Forward= " + String(distanceForward));

  }
}

int readSensors() {
  int weightedSum = 0;
  int sum = 0;

  //  for (int i = 0; i < 8; i++) {
  //    _sensorValues[i] = analogRead(_sensorPins[i]);
  //    // Serial.print(_sensorValues[i]);
  //    // Serial.print(" ");
  //    if (_sensorValues[i] > _sensorThreshold) {
  //      weightedSum += _sensorWeights[i];
  //      sum++;
  //    }
  //  }



  int distanceLeft = sensorLeft.getCuredDistance();
  //should have a distance of 7.5cm ideally

  weightedSum = (7.5 - distanceLeft) * 100;

  sum = 1;

  if (sum == 0)
  {
    return _previousError > 0 ? 1000 : -1000;; // If line is lost, continue turning
  }
  return weightedSum / sum; // Calculate position
}

void PID_Linefollow(int error) {
  int P = error;
  _integral += error;

  // Prevent integral too big or small
  //constraing here will make integral limit the integral from being smaller than -1000 or larger than 1000. So even if the integral is -6000, it will become -1000
  _integral = constrain(_integral, -1000, 1000);

  //Substract the past error from the new error
  int D = error - _previousError;

  //here is our main formula, we adjust Kp, Ki, Kd to see what works
  int PIDvalue = (_Kp * P) + (_Ki * _integral) + (_Kd * D);
  _previousError = error;

  int leftSpeed = _baseSpeed - PIDvalue;
  int rightSpeed = _baseSpeed + PIDvalue;

  //limit the speeds to not be too big
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  motor_drive(leftSpeed, rightSpeed);
}

void motor_drive(int left, int right) {
  // Apply a minimum speed limit to prevent motor jitter
  if (abs(left) < 50)
  {
    left = 0;
  }
  if (abs(right) < 50)
  {
    right = 0;
  }
  // Left motor control
  if (left >= 0)
  {
    analogWrite(IN1, left);
    analogWrite(IN2, 0);
  } else
  {
    analogWrite(IN1, 0);
    analogWrite(IN2, -left);
  }

  // Right motor control
  if (right >= 0)
  {
    analogWrite(IN3, right);
    analogWrite(IN4, 0);
  }
  else
  {
    analogWrite(IN3, 0);
    analogWrite(IN4, -right);
  }
  Serial.print(left);
  Serial.print("   ");
  Serial.print(right);
  Serial.println("");
}
