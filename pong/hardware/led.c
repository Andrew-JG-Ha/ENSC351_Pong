#include "led.h"
#include "../utility/a2d_utils.h"
#include <stdlib.h>

#define MAX_LEN 1024

#define FILE_PATH "/sys/class/gpio/"
#define VALUE_FILE "value"

void initLedPin(Led* led, char* pin, char* pinNumber);

Led* generateLed(LedHardware ledHardware) {
    Led* newLed = malloc(sizeof(Led));
    newLed->filePath = calloc(MAX_LEN, sizeof(char));
    sprintf(newLed->filePath, "%s%s/", FILE_PATH, ledHardware.gpioPin);
    initLedPin(newLed, ledHardware.pin, ledHardware.pinNumber);
    return newLed;
}

void initLedPin(Led* led, char* pin, char* pinNumber) {
    char buffer[MAX_LEN];
    char configPinCmd[MAX_LEN];
    sprintf(configPinCmd, "config-pin %s gpio", pin); 
    runCommand(configPinCmd);
    if (readFile(led->filePath, "direction", buffer) == -1) {
        writeToFile(GPIO_FILE_DIR, "export", pinNumber);
    }
    writeToFile(led->filePath, "direction", "out");
    writeToFile(led->filePath, VALUE_FILE, "0");

    //runCommand("echo in > /sys/class/gpio/gpio72/direction");
    //runCommand("echo 1 > /sys/class/gpio/gpio72/active_low");
    //runCommand("echo rising > /sys/class/gpio/gpio72/edge");
}

void turnLedOn(Led *led) {
    writeToFile(led->filePath, VALUE_FILE, "1");
}

void turnLedOff(Led *led) {
    writeToFile(led->filePath, VALUE_FILE, "0");
}

void destroyLed(Led* led) {
    free(led->filePath);
    led->filePath = NULL;
    free(led);
    led = NULL;
}
