#ifndef LCD_H
#define LCD_H
#include <pthread.h>

typedef struct Pin {
    char* pin;
    char* gpioPin;
    char* pinNumber;
} Pin;

typedef struct LcdHardware {
    Pin* dataPins;
    Pin rsPin;
    Pin ePin;
} LcdHardware;

typedef struct LcdScreen {
    char* ePinFilePath;
    char* rsPinFilePath;
    char** dataPinsFilePaths;
    int numDataPins;
} LcdScreen;

LcdScreen* generateLcd(LcdHardware lcdHardware, int numDataPins);

void destroyLcd(LcdScreen* screen);

void writeCharToLcd(LcdScreen* screen, char character);

void writeMessageToLcd(LcdScreen* screen, char* text);

#endif