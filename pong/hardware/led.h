#ifndef LED_H
#define LED_H

#include <stdbool.h>
typedef struct LedHardware {
    char* pin;
    char* gpioPin;
    char* pinNumber;
} LedHardware;

typedef struct Led {
    char* filePath;
} Led;

void turnLedOn(Led *led);
void turnLedOff(Led *led);

Led* generateLed(LedHardware ledHardware);
void destroyLed(Led *led);

#endif

