#include "light_sensor.h"

static void* sample_thread(void* sensor);

LightS* LightS_new(char* fileDirectory, char* fileName) {
    LightS* newLightS = malloc(sizeof(LightS));
    sprintf(newLightS->fileDir, "%s", fileDirectory);
    sprintf(newLightS->fileName, "%s", fileName);
    newLightS->readingsIndex = 0;
    newLightS->avgReading = 0;
    newLightS->prevTimestamp = 0;
    pthread_mutex_init(&newLightS->mID, NULL);
    return newLightS;
}

void LightS_destroy(LightS* sensor) {
    pthread_mutex_destroy(&sensor->mID);
    free(sensor);
    sensor = NULL;
}

void startSampling_LS(LightS *sensor) { 
    // create a pthread and begin sampling indefinitely
    if (pthread_create(&sensor->tID, NULL, sample_thread, sensor)) {
        perror("Error: pthread_create in 'startSampling_LS' failed. \n");
    }
    pthread_detach(sensor->tID);
}

void stopSampling_LS(LightS *sensor) {
    // stop sampling
    if (pthread_cancel(sensor->tID)) {
        perror("Error: pthread_cancel in 'stopSampling_LS' failed. \n");
    }
}

void reset_LS(LightS *sensor, double avgReading) {
    sensor->prevTimestamp = sensor->readings[sensor->readingsIndex - 1].currTimeStamp;
    sensor->avgReading = avgReading;
    sensor->readingsIndex = 0;
}

static void* sample_thread(void* arg) { 
    LightS* sensor = (LightS*) arg;
    while(true) {
        pthread_mutex_lock(&sensor->mID);
        double reading = getConvertedReadings(sensor->fileDir, sensor->fileName);
        sensor->readings[sensor->readingsIndex].currTimeStamp = getTimeInNanoS();
        sensor->readings[sensor->readingsIndex].voltReading = reading;
        sensor->readingsIndex++;
        pthread_mutex_unlock(&sensor->mID);
        sleepForMs(1);
        }
    return NULL;
}