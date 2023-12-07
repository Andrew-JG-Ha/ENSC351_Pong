#include "gpioButton.h"
#include "../utility/a2d_utils.h"
#include <stdlib.h>

#define MAX_LEN 1024

#define BUT_PATH "/sys/class/gpio/"
#define VALUE_FILE "value"

void initButtonPin(GpioButton* button, char* pin, char* pinNumber);

GpioButton* generateButton(ButtonHardware buttonHardware, bool threaded) {
    GpioButton* newButton = malloc(sizeof(GpioButton));
    newButton->val = 0;
    newButton->filePath = calloc(MAX_LEN, sizeof(char));
    sprintf(newButton->filePath, "%s%s/", BUT_PATH, buttonHardware.gpioPin);
    initButtonPin(newButton, buttonHardware.pin, buttonHardware.pinNumber);
    if (threaded) {
        pthread_mutex_init(&newButton->mId, NULL);
    }
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

void destroyButton(GpioButton* button, bool threaded) {
    if (threaded) {
        pthread_mutex_destroy(&button->mId);
    }
    free(button->filePath);
    button->filePath = NULL;
    free(button);
    button = NULL;
    
}

static void* buttonThread(void* but) {
    GpioButton* button = (GpioButton*) but;
    char path [MAX_LENGTH];
    //printf("Before sprintf: %s\n", path);
    sprintf(path,"%s%s" ,button->filePath, VALUE_FILE);
    while (true) {
        pthread_mutex_lock(&button->mId);
        waitForEdge(path, -1);
        button->val = !button->val;
        printf("Button val: %d", button->val);
        pthread_mutex_unlock(&button->mId);
    }
    return NULL;
}

void runButtonThread(GpioButton* button) {
    if (pthread_create(&button->tId, NULL, buttonThread, button)) {
        perror("ERROR: couldn't initialize button thread");
    }
    pthread_detach(button->tId);
}

void stopButtonThread(GpioButton* button) {
    if (pthread_cancel(button->tId)) {
        perror("ERROR: couldn't stop button thread");
    }
    if (!pthread_join(button->tId, NULL)) {
        perror("Thread join failed. \n");
        exit(1);
    }
}
