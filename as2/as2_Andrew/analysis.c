#include "analysis.h"

#define THRESHOLD_LIGHTS 0.1
#define HYSTERESIS 0.03
#define nano2milli 1/1000000

static void* performCalculations_thread(void* package);

static void getStats(Package* package);

static void displayStats(PackageStats statsPackage);

PackageStats* PackageStat_new() {
    PackageStats* newPackageStats = malloc(sizeof(PackageStats));
    newPackageStats->isDipped = false;
    return newPackageStats;
}

void PackageStat_destroy(PackageStats* statsPackage) {
    free(statsPackage);
}

Package* Package_new(LightS* sensor, PackageStats* statsPackage) {
    Package* newPackage = malloc(sizeof(Package));
    newPackage->sensor = sensor;
    newPackage->statsPackage = statsPackage;
    return newPackage;
}

void Package_destroy(Package* package) {
    free(package);
}

void startAnalysis(Package* package) { // occurs once every second
// mutex this each second so that this can perform updates to prevTimestamp and reset the index and readings array
    if (pthread_create(&package->statsPackage->tID, NULL, performCalculations_thread, package)) {
        perror("Error: pthread_create in 'startAnalysis' failed.\n");
    }
}

void stopAnalysis(Package* package) {
    if (pthread_cancel(package->statsPackage->tID)) {
        perror("Error: pthread_cancel in 'stopAnalysis' failed. \n");
    }
}

static void displayStats(PackageStats statsPackage) {
    printf("Interval ms (%.3f, %.3f) avg = %.3f    ", statsPackage.minInterval, statsPackage.maxInterval, statsPackage.avgInterval);
    printf("Samples V (%.3f, %.3f) avg = %.3f    ", statsPackage.minReading, statsPackage.maxReading, statsPackage.avgReading);
    printf("# Dips:  %d    ", statsPackage.numDips);
    printf("# Samples:  %d \n", statsPackage.numReadings);
}

static void* performCalculations_thread(void* package) {
    Package* inputPackage = (Package*) package;
    LightS* sensor = inputPackage->sensor;
    PackageStats* statsPackage = inputPackage->statsPackage;

    sleepForMs(1000);
    sensor->prevTimestamp = sensor->readings[0].currTimeStamp;
    sensor->avgReading = sensor->readings[0].voltReading;
    while (true) {
        // critical section
        pthread_mutex_lock(&sensor->mID);
        getStats(package);
        statsPackage->numReadings = sensor->readingsIndex + 1;
        reset_LS(sensor, statsPackage->avgReading);
        pthread_mutex_unlock(&sensor->mID);
        displayStats(*statsPackage);
        // end of critical section
        sleepForMs(1000);
    }
    return NULL;
}

static void getStats(Package* package) {
    LightS* sensor = package->sensor;
    PackageStats* statsPackage = package->statsPackage;
    double weightFactor = 0.999;

    int numberDips = 0;
    double avgReading = sensor->avgReading;
    statsPackage->maxReading = sensor->readings[0].voltReading;
    statsPackage->minReading = sensor->readings[0].voltReading;

    long long prevTime = sensor->prevTimestamp;
    double difference = (double) (sensor->readings[0].currTimeStamp - prevTime)* nano2milli;
    statsPackage->avgInterval = difference;
    statsPackage->maxInterval = difference;
    statsPackage->minInterval = difference;
    prevTime = sensor->readings[0].currTimeStamp;
    

    for (int i = 1; i < sensor->readingsIndex; i++) {
        // computing volt reading stats
        if (sensor->readings[i].voltReading > statsPackage->maxReading) {
            statsPackage->maxReading = sensor->readings[i].voltReading;
        }
        if (sensor->readings[i].voltReading < statsPackage->minReading) {
            statsPackage->minReading = sensor->readings[i].voltReading;
        }
        avgReading = weightFactor * avgReading + (1 - weightFactor) * sensor->readings[i].voltReading;

        // computing time stats
        difference = (double) (sensor->readings[i].currTimeStamp - prevTime)* nano2milli;
        if (difference > statsPackage->maxInterval) {
            statsPackage->maxInterval = difference;
        }
        if (difference < statsPackage->minInterval) {
            statsPackage->minInterval = difference;
        }
        statsPackage->avgInterval = statsPackage->avgInterval + difference; 
        prevTime = sensor->readings[i].currTimeStamp;

        // computing dips
        if (statsPackage->isDipped) {
            if (sensor->readings[i].voltReading > (statsPackage->dipThreshold + HYSTERESIS)) {
                statsPackage->isDipped = false;
            }
        }
        else if (!statsPackage->isDipped) {
            if (sensor->readings[i].voltReading < (avgReading - THRESHOLD_LIGHTS)) {
                statsPackage->dipThreshold = avgReading - THRESHOLD_LIGHTS;
                statsPackage->isDipped = true;
                numberDips++;
            }
        }
    }
    // updating statsPackage informations
    statsPackage->avgReading = avgReading;
    // printf("%f\n\n\n", avgReading);
    statsPackage->avgInterval = statsPackage->avgInterval / (sensor->readingsIndex);

    statsPackage->numDips = numberDips;
}
