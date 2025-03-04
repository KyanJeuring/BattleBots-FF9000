#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

class DistanceSensor {
  private:
    int trigPin;
    int echoPin;

    float getPulseDuration();
    float getProcessedDistance();

  public:
    DistanceSensor(int tPin, int ePin);
    double getCuredDistance(int sampleNum = 7, int removedSamplesMarginWidth = 1);
};

#endif
