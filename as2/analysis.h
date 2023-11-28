/**
 * analysis.h
 * This file contains two constructors, destructors (for Package and PackageStats) and methods to start and stop analysis of a given sensor
*/

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "light_sensor.h"

typedef struct PackageStats {
    int numReadings;
    int numDips;
    double minReading;
    double maxReading;
    double avgReading;
    double minInterval;
    double maxInterval;
    double avgInterval;
    bool isDipped;
    double dipThreshold;
    pthread_t tID;
} PackageStats;

typedef struct Package {
    LightS* sensor;
    PackageStats* statsPackage;
} Package;

PackageStats* PackageStat_new();

Package* Package_new(LightS* sensor, PackageStats* statsPackage);

void PackageStat_destroy(PackageStats* statsPackage);

void Package_destroy(Package* package);

void startAnalysis(Package* package);

void stopAnalysis(Package* package);

#endif
