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



//left motor
#define LEFT_FWD 8
#define LEFT_BCKWD 9
#define LEFT_SPEED 60

//right motor
#define RIGHT_FWD 10
#define RIGHT_BCKWD 11
#define RIGHT_SPEED 255


#define TIME_TO_TURN_LEFT 450
#define TIME_TO_TURN_RIGHT 280
#define TIME_TO_GO_FWD 1000



DistanceSensor _sensor= DistanceSensor(3, 2);

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_FWD,     OUTPUT);
  pinMode(LEFT_BCKWD,   OUTPUT);
  pinMode(RIGHT_FWD,    OUTPUT);
  pinMode(RIGHT_BCKWD,  OUTPUT);
  Forward();
}

//go forward
//continuously check for obstacles
//if obstacle turn left, go, turn right, go, turn right, go, turn left, go

void loop() {
  if(frontClear()){
    }else{ 
        //stop
        Stop();
      
        //                                                turn LEFT
        Left();
        delay(TIME_TO_TURN_LEFT);
        Stop();
        
        //go a little bit
        Forward();
        delay(TIME_TO_GO_FWD);
        Stop();
        
        //                                                 turn RIGHT
        Right();
        delay(TIME_TO_TURN_RIGHT);
        Stop();
        
        //go a little bit more
        Forward();
        delay(TIME_TO_GO_FWD);
        Stop();
        
        //go                                                turn RIGHT
        Right();
        delay(TIME_TO_TURN_RIGHT);
        Stop();

        //go a little bit
        Forward();
        delay(TIME_TO_GO_FWD);
        Stop();

        //                                                  turn LEFT
        Left();
        delay(TIME_TO_TURN_LEFT);
        Stop();
        
        //go forward
        Forward();
      }
}

void Forward() {
  digitalWrite( LEFT_FWD,      LEFT_SPEED   );
  digitalWrite( LEFT_BCKWD,    0            );
  digitalWrite( RIGHT_FWD,     RIGHT_SPEED  );
  digitalWrite( RIGHT_BCKWD,   0            );
}

void Backward() {
  digitalWrite( LEFT_FWD,      0            );
  digitalWrite( LEFT_BCKWD,    LEFT_SPEED   );
  digitalWrite( RIGHT_FWD,     0            );
  digitalWrite( RIGHT_BCKWD,   RIGHT_SPEED  );
}
void Stop() {
  digitalWrite( LEFT_FWD,      0            );
  digitalWrite( LEFT_BCKWD,    0            );
  digitalWrite( RIGHT_FWD,     0            );
  digitalWrite( RIGHT_BCKWD,   0            );
}
void Right() {
  digitalWrite( LEFT_FWD,      LEFT_SPEED   );
  digitalWrite( LEFT_BCKWD,    0            );
  digitalWrite( RIGHT_FWD,     0            );
  digitalWrite( RIGHT_BCKWD,   RIGHT_SPEED  );
}
void Left() {
  digitalWrite( LEFT_FWD,      0            );
  digitalWrite( LEFT_BCKWD,    LEFT_SPEED   );
  digitalWrite( RIGHT_FWD,     RIGHT_SPEED  );
  digitalWrite( RIGHT_BCKWD,   0            );
}

boolean frontClear() 
{
  if (_sensor.getCuredDistance()<20)
  {
    //something is in front
     return false;
  } else {
    return true;
  }
}