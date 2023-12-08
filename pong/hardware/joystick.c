#include "joystick.h"
#include "../utility/a2d_utils.h"
#include <pthread.h>
#include <math.h>

#define MAX_LENGTH 1024

static int joystickReadX(char* fileDirectory, char* xPin);

static int joystickReadY(char* fileDirectory, char* yPin);

static void* joystickRead_thread(void* joystickObject);

static int normalizeAroundZero(int value);

static int encodeDirection(Joystick joystick, int previousDescision);

static void decodeDirection(Joystick* joystick, int direction);

Joystick* generateJoystick(char* xPin_file, char* yPin_file, int cent, int f) {
    Joystick* newJoystick = malloc(sizeof(Joystick));
    newJoystick->xPin = calloc(sizeof(char), MAX_LENGTH);
    newJoystick->yPin = calloc(sizeof(char), MAX_LENGTH);
    sprintf(newJoystick->xPin, "%s", xPin_file);
    sprintf(newJoystick->yPin, "%s", yPin_file);
    newJoystick->leftRightDirection = 0;
    newJoystick->upDownDirection = 0;
    newJoystick->center = cent;
    newJoystick->flip = f;
    newJoystick->xReading = A2D_MAX_READING / 2;
    newJoystick->yReading = A2D_MAX_READING / 2;
    pthread_mutex_init(&newJoystick->mID, NULL);
    return newJoystick;
}

void destroyJoystick(Joystick* joystick) {
    pthread_mutex_destroy(&joystick->mID);
    /**
    for (int i = 0; i < MAX_LENGTH; i++) {
        joystick->xPin[i] = NULL;
        joystick->yPin[i] = NULL;
    }
    **/
    free(joystick->xPin);
    free(joystick->yPin);
    joystick->xPin = NULL;
    joystick->yPin = NULL;
    free(joystick);
    joystick = NULL;
}

void startReading_JS(Joystick* joystick) {
    if (pthread_create(&joystick->tID, NULL, joystickRead_thread, joystick)) {
        perror("Error: pthread_create in 'startReading_JS' failed.\n");
    }
    pthread_detach(joystick->tID);
}

void stopReading_JS(Joystick* joystick) {
    if (pthread_cancel(joystick->tID)) {
        perror("Error: pthread_cancel in 'stopReading_JS' failed.\n");
    }
}

int getUpDownDirection(Joystick* joystick) {
    //int direction = 0;
    int upBound = joystick->center + 400;
    int lowBound = joystick->center - 400;
    joystick->xReading = joystickReadX(A2D_FILE_DIR, joystick->xPin);
    joystick->yReading = joystickReadY(A2D_FILE_DIR, joystick->yPin);
    if (joystick->yReading > upBound) {
        joystick->upDownDirection = -1;
    } else if (joystick->yReading < lowBound) {
        joystick->upDownDirection = 1;
    } else {
        joystick->upDownDirection = 0;
    }
    //direction = encodeDirection(*joystick, direction);
    //decodeDirection(joystick, direction);
    return joystick->upDownDirection * joystick->flip;
}

static void* joystickRead_thread(void* joystickObject) { // constantly running and getting data
    Joystick* joystick = (Joystick*) joystickObject;
    int direction = 0;
    while(true) {
        pthread_mutex_lock(&joystick->mID);
        // critical section
        joystick->xReading = joystickReadX(A2D_FILE_DIR, joystick->xPin);
        joystick->yReading = joystickReadY(A2D_FILE_DIR, joystick->yPin);
        direction = encodeDirection(*joystick, direction);
        decodeDirection(joystick, direction);
        // critical section end
        pthread_mutex_unlock(&joystick->mID);
    }
    return NULL;
}

static int joystickReadX(char* fileDirectory, char* xPin) {
    int xVal = getRawReadings(fileDirectory, xPin);
    return xVal;
}

static int joystickReadY(char* fileDirectory, char* yPin) {
    int yVal = getRawReadings(fileDirectory, yPin);
    return yVal;
}



static int normalizeAroundZero(int value) {
    int result = (value - A2D_MAX_READING / 2);
    return result;
}

static int encodeDirection(Joystick joystick, int previousDescision) {
    /**
     * 0 = joystick centered (number of dips)
     * 1 = joystick up (max sample voltage) -X, Y don't care
     * 2 = joystick down (min sample voltage) +X, Y don't care
     * 3 = joystick left (min interval between samples) +Y, X don't care
     * 4 = joystick right (max interval between samples) -Y, X don't care
    */
    //int middleValue = A2D_MAX_READING / 2;
    int joystickThreshold = 300;
    int mappedX = normalizeAroundZero(joystick.xReading);
    int mappedY = normalizeAroundZero(joystick.yReading);
    // x == y is along the diagonal
    if (pow(mappedX, 2) + pow(mappedY, 2) <= pow(joystickThreshold, 2)) {
        return 0;
    }
    else {
        if (abs(mappedX) > abs(mappedY) && abs(mappedY) < joystickThreshold) {
            if (mappedX < abs(mappedY)) {
                return 1;
            }
            else {
                return 2;
            }
        }
        else if (abs(mappedX) < abs(mappedY) && abs(mappedX) < joystickThreshold) {
            if (mappedY < abs(mappedX)) {
                return 4;
            }
            else {
                return 3;
            }
        }
        else {
            return previousDescision;
        }
    }
}

static void decodeDirection(Joystick* joystick, int direction) {
    switch (direction) {
    case 1:
        joystick->upDownDirection = -1;
        joystick->leftRightDirection = 0;
        break;
    case 2:
        joystick->upDownDirection = 1;
        joystick->leftRightDirection = 0;
        break;
    case 3:
        joystick->upDownDirection = 0;
        joystick->leftRightDirection = -1;
        break;
    case 4:
        joystick->upDownDirection = 0;
        joystick->leftRightDirection = 1;
        break;
    default:
        joystick->leftRightDirection = 0;
        joystick->upDownDirection = 0;
        break;
    }
}