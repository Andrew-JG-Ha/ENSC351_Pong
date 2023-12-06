/**
 * joystick.h
 * This file contains a constructor, destructor (for Joystick) and methods to start and stop reading the joystick module
*/
#include <pthread.h>
#ifndef JOYSTICK_H
#define JOYSTICK_H

typedef struct JoystickHardware {
    char* xpin;
    char* ypin;
} JoystickHardware;

typedef struct Joystick {
    char* xPin;
    char* yPin;
    int yReading;
    int xReading;
    int upDownDirection;        // [up:-1, none:0, down:1]
    int leftRightDirection;     // [left:-1, none:0, right:1]
    pthread_mutex_t mID;
    pthread_t tID;
} Joystick;

Joystick* Joystick_new(char* xPin_file, char* yPin_file);

void Joystick_destroy(Joystick* joystick);

void startReading_JS(Joystick* joystick);

void stopReading_JS(Joystick* joystick);

int getUpDownDirection(Joystick* joystick);

#endif