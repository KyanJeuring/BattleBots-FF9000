
class DistanceSensor {
  private:
    int trigPin;
    int echoPin;

    // Gets raw data from the sensor
    float getPulseDuration() {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      return pulseIn(echoPin, HIGH);
    }

    // Gets the distance measured by the sensor
    float getProcessedDistance() {
      float pulseDuration = getPulseDuration();
      float distance = (pulseDuration * 0.0343) / 2;
      return distance;
    }

  public:
    // Constructor
    DistanceSensor(int tPin, int ePin) {
      trigPin = tPin;
      echoPin = ePin;
      pinMode(trigPin, OUTPUT);
      digitalWrite(trigPin, HIGH);
      pinMode(echoPin, INPUT);
    }

    double getCuredDistance(int sampleNum = 7, int removedSamplesMarginWidth = 1) {
      int samples[sampleNum];

      // Get all the samples
      for (int i = 0; i < sampleNum; i++) {
        samples[i] = getProcessedDistance();
      }

      // Sort the samples using insertion sort
      int key;
      for (int i = 1; i < sampleNum; i++) {
        key = samples[i];
        int j = i - 1;
        while (j >= 0 && samples[j] > key) {
          samples[j + 1] = samples[j];
          j--;
        }
        samples[j + 1] = key;
      }

      // Remove margins (smallest and largest values)
      for (int i = 0; i < sampleNum; i++) {
        samples[i] = samples[i + removedSamplesMarginWidth];
      }
      sampleNum -= removedSamplesMarginWidth * 2;

      // Calculate average of remaining samples
      double sum = 0;
      for (int i = 0; i < sampleNum; i++) {
        sum += samples[i];
      }
      return sum / sampleNum;
    }
};

// Gripper (servo motor)
#define SERVO 11
#define GRIPPER_OPEN 1600
#define GRIPPER_CLOSE 1050

// Motor control
#define LEFT_FWD 6
#define LEFT_BCKWD 10
#define LEFT_SPEED 50

#define RIGHT_FWD 5
#define RIGHT_BCKWD 9
#define RIGHT_SPEED 255

DistanceSensor _sensor(2, 4);

void setup() {
  Serial.begin(9600);
  pinMode(SERVO, OUTPUT);
  pinMode(LEFT_FWD, OUTPUT);
  pinMode(LEFT_BCKWD, OUTPUT);
  pinMode(RIGHT_FWD, OUTPUT);
  pinMode(RIGHT_BCKWD, OUTPUT);
}

void loop() {

  // Step 1: Open gripper
  if (millis() < 1000) 
  {
    gripper(GRIPPER_OPEN);
  }
  // Step 2: Close gripper after 1 sec
  else if (millis() < 2000) 
  {
    gripper(GRIPPER_CLOSE);
  }
  // Step 3: Open gripper again after another second
  else if (millis()< 3000) 
  {
    gripper(GRIPPER_OPEN);
  }
  // Step 4: Move forward 25 cm to reach cone
  else if (millis() < 5000) 
  {
    Forward();
  }
  // Step 5: Grab the cone
  else if (millis() < 7000) 
  {
    Stop();
    gripper(GRIPPER_CLOSE); 
  }
}

// Keep gripper active using millis()
void gripper(int pulse) 
{
  static unsigned long lastUpdate;
  static int lastPulse;

  if (millis() > lastUpdate) {
    if (pulse > 0) {
      lastPulse = pulse;
    } else {
      pulse = lastPulse;
    }

    digitalWrite(SERVO, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(SERVO, LOW);

    lastUpdate = millis() + 20; // Refresh every 20ms
  }
}

// Motor functions
void Forward() {
  digitalWrite(LEFT_FWD, LEFT_SPEED);
  digitalWrite(LEFT_BCKWD, 0);
  digitalWrite(RIGHT_FWD, RIGHT_SPEED);
  digitalWrite(RIGHT_BCKWD, 0);
}

void Stop() 
{
  digitalWrite(LEFT_FWD, 0);
  digitalWrite(LEFT_BCKWD, 0);
  digitalWrite(RIGHT_FWD, 0);
  digitalWrite(RIGHT_BCKWD, 0);
}
