/**
 * utils.h
 * This file contains all basic utility functions to interact with files and obtaining basic operating system information (time)
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/epoll.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <pthread.h>

#define GPIO_FILE_DIR "/sys/class/gpio/"

#define MAX_LENGTH 1024

FILE* openFile(char* fileDirectory, char* operation);

int writeToFile(char* fileDirectory, char* fileName, char* toFile);

int readFile(char* fileDirectory, char* fileName, char* buffer);

int edgeTrigger(char* fileDirectory, char* fileName, long long timeout, void (*func)(void));

int awaitChange(char* fileDirectory, char* fileName, long long minimumTime, long long timeout, char* activeState, void (*funcEdge)(void), void (*funcHold)(void));

int runCommand(char* command);

void sleepForMs(long long delayInMs);

long long getTimeInMilliS(void);

long long getTimeInMicroS(void);

long long getTimeInNanoS(void);

#endif
