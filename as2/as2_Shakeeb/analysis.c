#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sampler.h"
#include "utility.h"
#include "analysis.h"

#define HYSTERESIS 0.03
#define DIP_AMOUNT 0.1
#define AVERAGING_OLD_VALUE 0.999
#define AVERAGING_NEW_VALUE 0.001
#define NS_TO_MS 1/1000000


static pthread_t analysisThread;
static pthread_mutex_t analysisMutex = PTHREAD_MUTEX_INITIALIZER;
static dataPackage analyzedData;
static int isDipped = 0;
static double dipThreshold;

static void resetStats() {
    analyzedData.maxV = 0;
    analyzedData.minV = 9;
    analyzedData.maxInterval = 0;
    analyzedData.minInterval = 9;
    analyzedData.avgInterval = 0;
    analyzedData.numDips = 0;
}

static void computeVoltages(double buffV) {
    if (buffV > analyzedData.maxV) {
        analyzedData.maxV = buffV;
    }
    else if (buffV < analyzedData.minV) {
        analyzedData.minV = buffV;
    }
    analyzedData.avgV = AVERAGING_OLD_VALUE*analyzedData.avgV + AVERAGING_NEW_VALUE*buffV;
}

static void computeTimes(long long prevSampleTime, long long buffTime) {
    
    //Edge case when first sample is computed and prevSampleTime is initialized to 0
    if (prevSampleTime == 0) {
        analyzedData.minInterval = 0;
    }
    else {
        //Normal running cases
        if ((buffTime - prevSampleTime) > (long long) (analyzedData.maxInterval * 1000000)) {
            analyzedData.maxInterval = (double) (buffTime - prevSampleTime);
            analyzedData.maxInterval = analyzedData.maxInterval * NS_TO_MS;
        }
        else if ((buffTime - prevSampleTime) < (long long) (analyzedData.minInterval * 1000000)) {
            analyzedData.minInterval = (double) (buffTime - prevSampleTime);
            analyzedData.minInterval = analyzedData.minInterval * NS_TO_MS;
        }
    }
}

static void checkForDip(double buffV) {
    //Define an isDippable va
    //Must get to avg + 0.03 to change isDipped to 0
    //Upper Threshold is defined as average - 0.1 + HYSTERESIS
    //Lower threshold is defined as average - 0.1
    if (!isDipped) {
        if ((buffV < analyzedData.avgV - DIP_AMOUNT)) {
            dipThreshold = analyzedData.avgV - DIP_AMOUNT;
            analyzedData.numDips++;
            isDipped = 1;
        }
    }
    else {
        if (buffV > dipThreshold + HYSTERESIS) {
            isDipped = 0;
        }
    }
}

static void* analyze() {
    int length;
    double buffV;
    long long buffTime;
    long long sumOfTime;
    long long prevSampleTime = 0;

    samplerDatapoint_t firstVal = readFirstValueInHistory();
    analyzedData.avgV = firstVal.sampleInV;
    dipThreshold = firstVal.sampleInV;

    while (1) {
        pthread_mutex_lock(&analysisMutex);
        resetStats();
        sumOfTime = 0;
        samplerDatapoint_t* buff = Sampler_extractAllValues(&length);
        for (int i = 0; i < length; i++) {
            buffV = buff[i].sampleInV;
            buffTime = buff[i].timestampInNanoS;
            computeVoltages(buffV);
            computeTimes(prevSampleTime, buffTime);
            //Edge case when first sample is calculated and prevSampleTime is 0
            if (prevSampleTime == 0) {
                sumOfTime += 0;
            }
            else {
                sumOfTime += buffTime - prevSampleTime;
            }
            prevSampleTime = buffTime;
            checkForDip(buffV);
        }
        analyzedData.avgInterval = (double) (sumOfTime) /((double) (length)*1000000);
        free(buff);
        buff = NULL;
        pthread_mutex_unlock(&analysisMutex);
        printf("Interval ms (%.3lf, %.3lf) avg=%.3lf   Samples V (%.3lf, %.3lf) avg=%.3lf   # Dips:%4d   # Samples:%7d\n", analyzedData.minInterval, analyzedData.maxInterval, analyzedData.avgInterval, analyzedData.minV, analyzedData.maxV, analyzedData.avgV, analyzedData.numDips, length);
        sleepForMs(1000);
    }
    pthread_exit(NULL);
}

void startAnalyzing(void) {
    //Wait 1 second for data to accumulate in sensor
    sleepForMs(1000);
    pthread_mutex_lock(&analysisMutex);
    if (pthread_create(&analysisThread, NULL, analyze, NULL)) {
        perror("Thread creation failed.\n");
        exit(1);
    }
    pthread_detach(analysisThread);
    pthread_mutex_unlock(&analysisMutex);
}

void stopAnalyzing(void) {
    //Mutex lock to ensure that thread is not cancelled while memory is yet to be freed
    //by calls made within analyze
    pthread_mutex_lock(&analysisMutex);
    if (pthread_cancel(analysisThread)) {
        perror("Thread cancellation failed. \n");
        exit(1);
    }
    if (!pthread_join(analysisThread, NULL)) {
        perror("Thread join failed. \n");
        exit(1);
    }
    pthread_mutex_unlock(&analysisMutex);
    pthread_mutex_destroy(&analysisMutex);
}

dataPackage getData(void) {
    pthread_mutex_lock(&analysisMutex);
    dataPackage returnData = analyzedData;
    pthread_mutex_unlock(&analysisMutex);
    return returnData;
}