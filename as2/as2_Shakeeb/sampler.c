#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "utility.h"
#include "sampler.h"
#include "sensor.h"

#define AIN1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"

//Define samplingThread statically so it can be used by start and stop sampling functions
static pthread_t samplingThread;
// Create an unlocked mutex in a static manner
static pthread_mutex_t samplingMutex = PTHREAD_MUTEX_INITIALIZER;
static samplerDatapoint_t history[1024] = {[0 ... 1023] = {.sampleInV = .0, .timestampInNanoS = -1}};
static int location = 0;
static void* sampling() {

    for(;;) {
        pthread_mutex_lock(&samplingMutex);
        //int location = Sampler_getNumSamplesTaken();
        history[location].sampleInV = readVoltage(AIN1);
        history[location].timestampInNanoS = getTimeInNs();
        location++;
        pthread_mutex_unlock(&samplingMutex);

        //Sleep for 1 ms
        sleepForMs(1);
    }
    pthread_exit(NULL);
}

void Sampler_startSampling(void) {
    
    //Pass in function to run in thread
    pthread_mutex_lock(&samplingMutex);
    if (pthread_create(&samplingThread, NULL, sampling, NULL)) {
        perror("Thread creation failed.\n");
        exit(1);
    }
    pthread_detach(samplingThread);
    pthread_mutex_unlock(&samplingMutex);
}

void Sampler_stopSampling(void) {
    pthread_mutex_lock(&samplingMutex);
    if (pthread_cancel(samplingThread)) {
        perror("Thread cancellation failed. \n");
        exit(1);
    }
    if (!pthread_join(samplingThread, NULL)) {
        perror("Thread join failed. \n");
        exit(1);
    }
    pthread_mutex_unlock(&samplingMutex);
    pthread_mutex_destroy(&samplingMutex);
}

samplerDatapoint_t* Sampler_extractAllValues(int *length) {
    pthread_mutex_lock(&samplingMutex);
    *length = location;
    samplerDatapoint_t* extracted = malloc(*length * sizeof(samplerDatapoint_t));
    for (int i = 0; i < *length; i++) {
        extracted[i] = history[i];
        history[i].sampleInV = 0.0;
        history[i].timestampInNanoS = -1;
    }
    location = 0;
    pthread_mutex_unlock(&samplingMutex);
    return extracted;
}

samplerDatapoint_t readFirstValueInHistory(void) {
    samplerDatapoint_t firstVal;
    pthread_mutex_lock(&samplingMutex);
    firstVal.sampleInV = history[0].sampleInV;
    firstVal.timestampInNanoS = history[0].sampleInV;
    pthread_mutex_unlock(&samplingMutex);
    return firstVal;
}