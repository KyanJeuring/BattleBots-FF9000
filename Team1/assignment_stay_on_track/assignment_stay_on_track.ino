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

// Sensor array
int _sensorPins[] = { A0, A1, A2, A3, A4, A5, A6, A7 };
int _sensorValues[8];
int _sensorThreshold = 500; // Hardcoded for now, will be automated in the future
int _sensorWeights[] = { -3500, -2500, -1500, -500, 500, 1500, 2500, 3500 }; // Position weights

// PID constants
float _Kp = 0.12; // Increased Proportional Gain slightly   /Proportional gain     corrects error
float _Ki = 0.001; // Small Integral Gain to fix slow drifting  /Integral gain         corrects small errors, that are long term (slightly being off the line)
float _Kd = 0.02; // Reduced Derivative Gain  /Derivate gain         dampens the corrections

int _previousError = 0;
int _integral = 0;
int _baseSpeed = 220; // Adjust based on motor power

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < 8; i++) 
    {
        pinMode(_sensorPins[i], INPUT);
    }
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(BTPIN, OUTPUT);
}

void loop() {
    int position = readSensors();
    int error = position;
    
    PID_Linefollow(error);
}

/*
the readSensors() function returns the position of the line relative to the robot

each sensor has a weight, the weights are stored in the sensorWeights[] array

CASE 1: line in the center
    if the line is exactly in the middle and only the two middle sensors see it, it will calculate the weight of those sensors like this:
          sensorWeights[3]  +   sensorWeights[4]
          -500              +   500
    the result in this case will be 0, that means that the line is directly in the center. If the line is thicker and the 4 sensors in the middle are activated:
          sensorWeights[2]  +   sensorWeights[3]    +   sensorWeights[4]  +   sensorWeights[5]
          -1500             +   -500                +   500               +   1500
    the result is still 0, because the line is in the center.

CASE 2: line on the left
    if the line is on the left, only the sensor weights on the left will add up to the result, for instance, if the 1, 2, 3, 4 sensors detect the line:
          sensorWeights[1]  +   sensorWeights[2]  +   sensorWeights[3]  +   sensorWeights[4]
          -2500             +   -1500             +   -500              +   500
    the result will be -4000, which means that the robot will know that it has to turn to the left.
    If line is further on the left, maybe even the line is so far to the edge as only the outmost two sensors detect it:
          sensorWeights[0]  +   sensorWeights[1]
          -3500             +   -2500             = -6000
          -6000 /2  = -3000
    The result will be -3000, which means that the robot will try really hard to turn left beacause it is about to loose the line.

CASE 3: line on the right
    if the line is on the right, only the sensor weights on the right will add up to the result, for instance, if the 4, 5, 6 sensors detect the line:
          sensorWeights[4]  +   sensorWeights[5]  +   sensorWeights[6]
          500               +   1500              +   2500              = 4500
          4500  / 3 = 1500             
    the result will be 1500, which means that the robot will know just about how much to turn to the right.

    
So, using this algorithm, the width of the line doesn't matter, which is crucial, because for instance when the line is parallel to the robot, it will appear thinner than when it is diagonal
*/
int readSensors() {
    int weightedSum = 0;
    int sum = 0;
    
    for (int i = 0; i < 8; i++) {
        _sensorValues[i] = analogRead(_sensorPins[i]);
        // Serial.print(_sensorValues[i]);
        // Serial.print(" ");
        if (_sensorValues[i] > _sensorThreshold) {
            weightedSum += _sensorWeights[i];
            sum++;
        }
    }
    // Serial.println();
    
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
