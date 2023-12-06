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
    //writeToFile(button->filePath, "edge", "falling");

    //runCommand("echo in > /sys/class/gpio/gpio72/direction");
    //runCommand("echo 1 > /sys/class/gpio/gpio72/active_low");
    //runCommand("echo rising > /sys/class/gpio/gpio72/edge");
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

void* k() {
    printf("MOO");
    return NULL;
}

/**
int main() {
    struct ButtonHardware a;
    a.pin = "p8.8";
    a.gpioPin = "gpio67";
    a.pinNumber = "67";
    GpioButton* but = generateButton(a);
    int val = -1;

    long long startTime = getTimeInMilliS();
    long long currTime = 0.0;
    //int i = edgeTrigger(but->filePath, "edge", 500, k());
    //i += 1;
    while(true) {
        sleepForMs(100);
        val = readButton(but);
        printf("Current Value: %d\n", val);
        currTime = getTimeInMilliS();
        if (currTime - startTime > 10000){
            exit(1);
        }
    }
    destroyButton(but);
}
**/

