#ifndef GPIO_BUTTON_H
#define GPIO_BUTTON_H

#define MAX_LEN 64

typedef struct GpioButton {
    char pin[64];
} GpioButton;

static int readButton(GpioButton gpioButton);

GpioButton generateButton(char* pin);

