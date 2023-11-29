/**
 * light_sensor.h
 * This file contains a constructor, destructor (for LightS) and methods to start and stop sampling, and to reset the sensor readings 
*/

#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "utility/a2d_utils.h"

typedef struct PackageVT {
    double voltReading;
    long long currTimeStamp;
} PackageVT;

typedef struct LightS {
    char fileDir[MAX_LENGTH];
    char fileName[MAX_LENGTH];
    long long prevTimestamp;
    double avgReading;
    int readingsIndex;
    PackageVT readings[MAX_LENGTH * 2];
    pthread_t tID;
    pthread_mutex_t mID;
} LightS;

LightS* LightS_new(char* fileDirectory, char* fileName);

void LightS_destroy(LightS* sensor);

void reset_LS(LightS *sensor, double avgReading);

void startSampling_LS(LightS* sensor);

void stopSampling_LS(LightS* sensor);

#endif
