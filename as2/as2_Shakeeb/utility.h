// utility.h
// Module that contains various utility modules utilized within the main program.
#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include "utility.h"


int readFromFile(char *fileName, char *buff,int MAX_LENGTH);
void runCommand(char* command);
void sleepForMs(long long delayInMs);
long long getTimeInNs(void);
void configPin(char* pin, char* configuration);

#endif