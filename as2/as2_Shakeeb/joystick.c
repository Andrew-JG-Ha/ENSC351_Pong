#include "joystick.h"
#include "utility.h"
#include "analysis.h"
#include "ledMatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define JOYSTICK_X "/sys/bus/iio/devices/iio:device0/in_voltage2_raw"
#define JOYSTICK_Y "/sys/bus/iio/devices/iio:device0/in_voltage3_raw"

static pthread_t joystickThread;
static pthread_mutex_t joystickMutex = PTHREAD_MUTEX_INITIALIZER;
static dataPackage storedData;

void Joystick_readXY(double* x, double* y) {
    char outputX[10];
    char outputY[10];
    char *eptr;
    double ADCValue;
    readFromFile(JOYSTICK_X, outputX, 10);
    ADCValue = strtod(outputX, &eptr);
    *x = (ADCValue - 2047.0)/2047.0;
    readFromFile(JOYSTICK_Y, outputY, 10);
    ADCValue = strtod(outputY, &eptr);
    *y = (ADCValue - 2047.0)/2047.0;
}

static void* readAndWrite(void* i2cFileDesc) {
    double x, y;
    int* i2cFileDescriptor =  (int*) i2cFileDesc;
    while(1) {
        pthread_mutex_lock(&joystickMutex);
        storedData = getData();
        Joystick_readXY(&y, &x);
        if (y > 0.5) {
            //UP
            displayFloat(*i2cFileDescriptor, storedData.maxV);
        }
        else if (y < -0.5) {
            //DOWN
            displayFloat(*i2cFileDescriptor, storedData.minV);
        }
        else if (x > 0.5) {
            //RIGHT
            displayFloat(*i2cFileDescriptor, storedData.maxInterval);
        }
        else if (x < -0.5) {
            //LEFT
            displayFloat(*i2cFileDescriptor, storedData.minInterval);
        }
        else {
            //CENTER
            displayInt(*i2cFileDescriptor, storedData.numDips);
        }
        pthread_mutex_unlock(&joystickMutex);
        
    }
    pthread_exit(NULL);
}

void Joystick_startReading(int* i2cFileDesc) {
    pthread_mutex_lock(&joystickMutex);
    if (pthread_create(&joystickThread, NULL, readAndWrite, i2cFileDesc)) {
        perror("Joystick Thread creation failed. \n");
        exit(1);
    }
    pthread_detach(joystickThread);
    pthread_mutex_unlock(&joystickMutex);
}

void Joystick_stopReading(int* i2cFileDesc) {
    pthread_mutex_lock(&joystickMutex);
    shutdownLEDMatrix(i2cFileDesc);
    if (pthread_cancel(joystickThread)) {
        perror("Joystick Thread cancellation failed.\n");
        exit(1);
    }
    if (!pthread_join(joystickThread, NULL)) {
        perror("Thread join failed. \n");
        exit(1);
    }
    pthread_mutex_unlock(&joystickMutex);
    pthread_mutex_destroy(&joystickMutex);
}