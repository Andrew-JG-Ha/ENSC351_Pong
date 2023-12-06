#ifndef GPIO_BUTTON_H
#define GPIO_BUTTON_H

typedef struct ButtonHardware {
    char* pin;
    char* gpioPin;
    char* pinNumber;
} ButtonHardware;

typedef struct GpioButton {
    char* filePath;
    int val;        // for thread-use
} GpioButton;

int readButton(GpioButton *button);

GpioButton* generateButton(ButtonHardware buttonHardware);

void destroyButton(GpioButton* button);

#endif

