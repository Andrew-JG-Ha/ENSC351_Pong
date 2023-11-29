#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include "utility.h"

/*
Reads data from a file and stores the value within buff.
@parameter char* fileName: character pointer to array that stores the file we want to read
@parameter char* buff: character pointer to array that will store the read value
@parameter int MAX_LENGTH: The maximum length of the buff array
returns 1 if file is read correctly, else returns -1
*/
int readFromFile(char *fileName, char *buff,int MAX_LENGTH) {
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        return -1;
    }

    //Read string (line)
    fgets(buff, MAX_LENGTH, pFile);
    fclose(pFile);
    return 1;
}

/*
Runs a linux command in terminal
@parameter char* command: A pointer to a character array that holds the linux command
*/
void runCommand(char* command) {
    FILE *pipe = popen(command, "r");
    char buffer[1024]; // Create buffer to eat the output of the command
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL) {
            break;
        }
    }

    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:\n");
        printf("command: %s\n", command);
        printf("exit code: %d\n", exitCode);
    }
}

void sleepForMs(long long delayInMs) {
 const long long NS_PER_MS = 1000 * 1000;
 const long long NS_PER_SECOND = 1000000000;
 long long delayNs = delayInMs * NS_PER_MS;
 int seconds = delayNs / NS_PER_SECOND;
 int nanoseconds = delayNs % NS_PER_SECOND;
 struct timespec reqDelay = {seconds, nanoseconds};
 nanosleep(&reqDelay, (struct timespec *) NULL);
}

long long getTimeInNs(void) {
 struct timespec spec;
 clock_gettime(CLOCK_REALTIME, &spec);
 long long seconds = spec.tv_sec;
 long long nanoSeconds = spec.tv_nsec;
 //long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;
 return seconds* 1000000000 + nanoSeconds;
}

void configPin(char* pin, char* configuration) {
    char command [30] = "";
    sprintf(command, "%s %s %s", "config-pin", pin, configuration);
    runCommand(command);
}

