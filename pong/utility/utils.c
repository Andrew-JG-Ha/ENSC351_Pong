#include "utils.h"
#include <time.h>
#include <string.h>
#include <sys/epoll.h> 
#include <unistd.h> 
#include <fcntl.h> 

#define MAX_EVENTS 2
#define MAX_LENGTH 1024

/**
 * Given a file location (fileDirectory), opens and returns the file 
*/
FILE* openFile(char* fileDirectory, char* operation) {
    FILE* file = fopen(fileDirectory, operation);
    if (file == NULL) {
        if (strncmp(operation, "r", 2) == 0) {
            printf("ERROR: Unable to open file (%s) for read\n", fileDirectory);
            return NULL;
        }
        else if (strncmp(operation, "w", 2) == 0) {
            printf("ERROR: Unable to open file (%s) for write\n", fileDirectory);
            return NULL;
        }  
    }
    return file;
}

/**
 * Given a fileDirectory and a fileName, opens the file and writes to the file the string stored in 'toFile'
*/
int writeToFile(char* fileDirectory, char* fileName, char* toFile) {
    char fullFileDir[MAX_LENGTH];
    if (!fileName) {
        sprintf(fullFileDir, "%s", fileDirectory);
    }
    else {
        sprintf(fullFileDir, "%s%s", fileDirectory, fileName);
    }
    
    FILE* file = openFile(fullFileDir, "w");
    if (file == NULL) {
        return(-1);
    }
    else {
        int printRes = fprintf(file, toFile);
        if (printRes <= 0) {
            printf("ERROR: Unable to write to file (%s)\n", fullFileDir);
            return(-1);
        }
        fclose(file);
        return(1);
    }
}

/**
 * Given a fileDirectory and a fileName, opens the file and outputs its contents to 'buffer'
*/
int readFile(char* fileDirectory, char* fileName, char* buffer) {
    char fullFileDir[MAX_LENGTH];
    if (!fileName) {
        sprintf(fullFileDir, "%s", fileDirectory);
    }
    else {
        sprintf(fullFileDir, "%s%s", fileDirectory, fileName);
    }
    
    FILE* file = openFile(fullFileDir, "r");
    if (file == NULL) {
        return(-1);
    }
    else {
        fgets(buffer, MAX_LENGTH, file);
        fclose(file);
        return(1);
    }
}

/*
Reads data from a file and stores the value within buff.
@parameter char* fileName: character pointer to array that stores the file we want to read
@parameter char* buff: character pointer to array that will store the read value
@parameter int MAX_LENGTH: The maximum length of the buff array
returns 1 if file is read correctly, else returns -1
*/
int readFromFile(char *fileName, char *buff) {
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        return -1;
    }

    //Read string (line)
    fgets(buff, MAX_LENGTH, pFile);
    fclose(pFile);
    return 1;
}

/**
 * Runs a linux command given in 'command'
*/
int runCommand(char* command) {
    char buff[MAX_LENGTH];
    FILE* pipe = popen(command, "r");
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buff, MAX_LENGTH, pipe) == NULL) break;
    }
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf("    command:   %s\n", command);
        printf("    exit code: %d\n", exitCode);
        return(-1);
    }
    return(1);
}

/**
 * Sleeps the program for (delayInMs)ms amount of time 
*/
void sleepForMs(long long delayInMs) {
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SEC = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SEC;
    int nanoseconds = delayNs % NS_PER_SEC;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec*) NULL);
}

/**
 * Provides the current clock time in ms
*/
long long getTimeInMilliS(void) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;
    return milliSeconds;
}

long long getTimeInMicroS(void) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long microSeconds = seconds * 1000000 + nanoSeconds / 1000;
    return microSeconds;
}

long long getTimeInNanoS(void) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    nanoSeconds = seconds * 1000000000 + nanoSeconds;
    return nanoSeconds;
}

/*
Waits for timeInMs milliseconds for a change in value in path. If there is a change in value then the function returns early.
Requires that the pin has it's edge set to the intended edge setting. Do not use if edge is set to 'none'.
@parameter char* path: path to the value you want to check the edge of
@parameter long long int timeInMs: maximum time to wait for a change in value, value is represented in milliseconds
*/
int waitForEdge(char* path, long long int timeInMs) {
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    int nfds, fd, n;
    int epollFileDescriptor = epoll_create1(0);
    
    if (epollFileDescriptor == -1) {
        perror("epoll_create1 failed.");
        exit(EXIT_FAILURE);
    }

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("opening of file path failed.");
        close(epollFileDescriptor);
        exit(EXIT_FAILURE);
    }

    event.events = EPOLLET;

    if (epoll_ctl(epollFileDescriptor, EPOLL_CTL_ADD, fd, &event) == -1) {
        perror("epoll_ctl failed.");
        close(epollFileDescriptor);
        close(fd);
        exit(EXIT_FAILURE);
    }

    for (n = 0; n < MAX_EVENTS; ++n) {
        nfds = epoll_wait(epollFileDescriptor, &event, MAX_EVENTS, timeInMs);

        if (nfds == -1) {
            perror("epoll_wait.");
            close(epollFileDescriptor);
            close(fd);
            exit(EXIT_FAILURE);
        }
    }
    close(epollFileDescriptor);
    close(fd);
    return 1;
}

void configureGPIO(char* gpioPinNumber, char* edgeType) {
    if (strcmp(edgeType, "rising") & strcmp(edgeType, "falling") & strcmp(edgeType, "both") & strcmp(edgeType, "none")) {
        perror("Invalid Edge Type entered.\n");
        exit(EXIT_FAILURE);
    }
    else {
        char dest[50] = "/sys/class/gpio/gpio";
        char setup[60] = "echo ";
        char buff[32];
        char command[128] = "echo ";
        strncat(setup, gpioPinNumber, 4);
        strncat(setup, " > /sys/class/gpio/export", 26);
        strncat(command, edgeType, 10);
        strncat(command, " > ", 10);
        strncat(dest, gpioPinNumber,36);
        strncat(dest, "/edge", 30);
        int res = readFromFile(dest, buff);
        if (res == -1) {
            runCommand(setup);
        }
        runCommand(strncat(command, dest, 60));
    }
}

/**
 * Given a fileDirectory and a fileName, creates an epoll instance to watch the file, edgeTrigger will wait until the event described in
 * the GPIO's edge folder occurs or until the timeout duration has elasped before returning to the main program. This function calls 'func'
 * whenever the edge is detected - NULL may be entered into func, if nothing is to be called
*/
int edgeTrigger(char* fileDirectory, char* fileName, long long timeout, void (*func)(void)) {
    char fullFileDir[MAX_LENGTH];
    struct epoll_event event;
    int epollFD = epoll_create1(0);
    if (epollFD == -1) {
        printf("ERROR: failed to create an epoll file descriptor\n");
        return(-1);
    }
    sprintf(fullFileDir, "%s%s", fileDirectory, fileName);
    int fd = open(fullFileDir, O_RDONLY);
    if (fd == -1) {
        printf("ERROR: failed to open() file (%s) for read access\n", fullFileDir);
        return(-1);
    }
    event.events = EPOLLET;
    event.data.fd = fd;
    if (epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &event) == -1) {
        printf("ERROR: failed to add control epoll_ctl() on file descriptor (%d)\n", fd);
        return(-1);
    }
    for (int i = 0; i < 2; i++) {
        int waitRes = epoll_wait(epollFD, &event, MAX_EVENTS, timeout);
        if (func != NULL) {
            func();
        }
        if (waitRes == -1) {
            printf("ERROR: epoll_wait() failed (%d)\n", waitRes);
            close(fd);
            close(epollFD);
            return(-1);
        }
    }
    close(fd);
    close(epollFD);
    return(1);
}

/**
 * Given a fileDirectory and a fileName, this function takes in a minimumTime that the user defines for a change state to be considered valid, this function
 * also takes in a timeout for when it has stalled for too long. This function calls 'func' while it is in the considered hold state.
*/
int awaitChange(char* fileDirectory, char* fileName, long long minimumTime, long long timeout, char* activeState, void (*funcEdge)(void), void (*funcHold)(void)) {
    char currentVal[MAX_LENGTH];
    strcpy(currentVal, activeState);
    edgeTrigger(fileDirectory, fileName, timeout, funcEdge);
    long long startTime =  getTimeInMilliS();
    long long currentTime = startTime;
    readFile(fileDirectory, fileName, currentVal);
    while (strncmp(activeState, currentVal, 5) == 0) {
        currentTime = getTimeInMilliS();
        if (funcHold != NULL) {
            funcHold();
        }
        readFile(fileDirectory, fileName, currentVal);
    }
    if ((currentTime - startTime) > minimumTime) {
        return(1);
    }
    else {
        return(0);
    }
}