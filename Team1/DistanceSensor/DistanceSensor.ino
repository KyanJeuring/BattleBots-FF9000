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
      if(maxDistance == -1)
      {
        return pulseIn(echoPin, HIGH);
      }else{
      return pulseIn(echoPin, HIGH, 60 * maxDistance); //24000 is for about 400cm, so if you want a limit of one meter just set it to 6000
      }
    }

    // gets the distance measured by the sensor
    float getProcessedDistance()
    {
      float pulseDuration = getPulseDuration();
      float distance;
      if(pulseDuration > 100){
        distance = (pulseDuration * .0343) / 2;
      }else{
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



//=====================================================EXAMPLE
//
//DistanceSensor sensor1 = DistanceSensor(8, 9);
//
//void setup() {
//  // initialize digital pin LED_BUILTIN as an output.
//  Serial.begin(9600);
//  Serial.println("setup done");
//}
//
//void loop() {
//  delay(1000);
//  Serial.println(sensor1.getCuredDistance());
//}
