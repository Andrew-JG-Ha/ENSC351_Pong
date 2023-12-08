/**
 * utils.h
 * This file contains all basic utility functions to interact with files and obtaining basic operating system information (time)
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define GPIO_FILE_DIR "/sys/class/gpio/"

FILE* openFile(char* fileDirectory, char* operation);

int writeToFile(char* fileDirectory, char* fileName, char* toFile);

int readFile(char* fileDirectory, char* fileName, char* buffer);

int readFromFile(char *fileName, char *buff);

int waitForEdge(char* path, long long int timeInMs);

void configureGPIO(char* gpioPinNumber, char* edgeType);

int edgeTrigger(char* fileDirectory, char* fileName, long long timeout, void (*func)(void));

int awaitChange(char* fileDirectory, char* fileName, long long minimumTime, long long timeout, char* activeState, void (*funcEdge)(void), void (*funcHold)(void));

int runCommand(char* command);

void sleepForMs(long long delayInMs);

void sleepForNs(long long delayInNs);

long long getTimeInMilliS(void);

long long getTimeInMicroS(void);

long long getTimeInNanoS(void);

#endif
