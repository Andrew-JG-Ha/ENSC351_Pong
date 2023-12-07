/**
 * joystick.h
 * This file contains a constructor, destructor (for Joystick) and methods to start and stop reading the joystick module
*/

#ifndef JOYSTICK_H
#define JOYSTICK_H

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

Joystick* generateJoystick(char* xPin_file, char* yPin_file);

void destroyJoystick(Joystick* joystick);

void startReading_JS(Joystick* joystick);

void stopReading_JS(Joystick* joystick);

#endif