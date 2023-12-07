#ifndef GPIO_BUTTON_H
#define GPIO_BUTTON_H
#include <pthread.h>
#include "../utility/utils.h"

typedef struct ButtonHardware {
    char* pin;
    char* gpioPin;
    char* pinNumber;
} ButtonHardware;

typedef struct GpioButton {
    char* filePath;
    int val;        // for thread-use
    pthread_t tId;
    pthread_mutex_t mId;
} GpioButton;

int readButton(GpioButton *button);

GpioButton* generateButton(ButtonHardware buttonHardware, bool threaded);

void destroyButton(GpioButton* button, bool threaded);
void runButtonThread(GpioButton* button);
void stopButtonThread(GpioButton* button); 


#endif

