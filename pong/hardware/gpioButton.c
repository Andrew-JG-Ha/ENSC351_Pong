#include "gpioButton.h"
#include "../utility/a2d_utils.h"
#include <stdlib.h>

#define MAX_LEN 1024

#define BUT_PATH "/sys/class/gpio/"
#define VALUE_FILE "value"

void initButtonPin(GpioButton* button, char* pin, char* pinNumber);

GpioButton* generateButton(ButtonHardware buttonHardware) {
    GpioButton* newButton = malloc(sizeof(GpioButton));
    newButton->val = 0;
    newButton->filePath = calloc(MAX_LEN, sizeof(char));
    sprintf(newButton->filePath, "%s%s/", BUT_PATH, buttonHardware.gpioPin);
    initButtonPin(newButton, buttonHardware.pin, buttonHardware.pinNumber);
    return newButton;
}

void initButtonPin(GpioButton* button, char* pin, char* pinNumber) {
    char buffer[MAX_LEN];
    char configPinCmd[MAX_LEN];
    sprintf(configPinCmd, "config-pin %s gpio", pin); 
    runCommand(configPinCmd);
    if (readFile(button->filePath, "direction", buffer) == -1) {
        writeToFile(GPIO_FILE_DIR, "export", pinNumber);
    }
    writeToFile(button->filePath, "direction", "in");
    writeToFile(button->filePath, "active_low", "0");
    writeToFile(button->filePath, VALUE_FILE, "0");
}

int readButton(GpioButton* button) {
    int retVal = -1;
    char buffer[MAX_LEN];
    readFile(button->filePath, VALUE_FILE, buffer);
    retVal = atoi(buffer);
    return retVal;
}

void destroyButton(GpioButton* button) {
    free(button->filePath);
    button->filePath = NULL;
    free(button);
    button = NULL;
}
