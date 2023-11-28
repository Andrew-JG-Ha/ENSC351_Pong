/**
 * joystick.h
 * This file contains a constructor, destructor (for Joystick) and methods to start and stop reading the joystick module
*/

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "utility/a2d_utils.h"

/** REFERENCE JOYSTICK DIRECTIONS
 * _______________
 * |             |
 * |     -X      |
 * |      ^      |
 * |+Y <- | -> -Y|
 * |      ⌵      |
 * |     +X      |
 * |_____________|
 * |  +  Y  X  - |
 * |_____________|
 * 
 * to -y : -y > |x| : -x < -y < x
 * to +y :  y < |x| : -x < y < x
 * to -x : -x > |y| : -y < -x < y
 * to +x :  x < |y| : -y < x < y
*/

typedef struct Joystick {
    char xPin[MAX_LENGTH];
    char yPin[MAX_LENGTH];
    int yReading;
    int xReading;
    pthread_mutex_t mID;
    pthread_t tID;
} Joystick;

Joystick* Joystick_new(char* xPin_file, char* yPin_file);

void Joystick_destroy(Joystick* joystick);

void startReading_JS(Joystick* joystick);

void stopReading_JS(Joystick* joystick);

#endif
