#include "DistanceSensor.h"
#include <Arduino.h>

DistanceSensor::DistanceSensor(int tPin, int ePin) {
  trigPin = tPin;
  echoPin = ePin;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, HIGH);
  pinMode(echoPin, INPUT);
}

float DistanceSensor::getPulseDuration() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  return pulseIn(echoPin, HIGH);
}

float DistanceSensor::getProcessedDistance() {
  float pulseDuration = getPulseDuration();
  float distance = (pulseDuration * .0343) / 2;
  return distance;
}

double DistanceSensor::getCuredDistance(int sampleNum, int removedSamplesMarginWidth) {
  int samples[sampleNum];

  // Get all the samples
  for (int index = 0; index < sampleNum; index++) {
    samples[index] = getProcessedDistance();
  }

  // Sort the samples using insertion sort
  int key;
  for (int i = 1; i < sampleNum; i++) {
    key = samples[i];
    int j = i - 1;
    while (j >= 0 && samples[j] > key) {
      samples[j + 1] = samples[j];
      j = j - 1;
    }
    samples[j + 1] = key;
  }

  // Remove margins
  for (int index = 0; index < sampleNum; index++) {
    samples[index] = samples[index + removedSamplesMarginWidth];
  }
  sampleNum = sampleNum - removedSamplesMarginWidth * 2;

  // Calculate average of remaining samples
  double average;
  double sum = 0;
  for (int index = 0; index < sampleNum; index++) {
    sum += samples[index];
  }
  average = sum / sampleNum;

  return average;
}
