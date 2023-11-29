#include "joystick.h"

static int joystickReadX(char* fileDirectory, char* xPin);

static int joystickReadY(char* fileDirectory, char* yPin);

static void* joystickRead_thread(void* joystickObject);

Joystick* Joystick_new(char* xPin_file, char* yPin_file) {
    Joystick* newJoystick = malloc(sizeof(Joystick));
    sprintf(newJoystick->xPin, "%s", xPin_file);
    sprintf(newJoystick->yPin, "%s", yPin_file);
    newJoystick->xReading = A2D_MAX_READING / 2;
    newJoystick->yReading = A2D_MAX_READING / 2;
    pthread_mutex_init(&newJoystick->mID, NULL);
    return newJoystick;
}

void Joystick_destroy(Joystick* joystick) {
    pthread_mutex_destroy(&joystick->mID);
    free(joystick);
    joystick = NULL;
}

void startReading_JS(Joystick* joystick) {
    if (pthread_create(&joystick->tID, NULL, joystickRead_thread, joystick)) {
        perror("Error: pthread_create in 'startReading_JS' failed.\n");
    }
    pthread_detach(joystick->tID);
}

void stopReading_JS(Joystick* joystick) {
    if (pthread_cancel(joystick->tID)) {
        perror("Error: pthread_cancel in 'stopReading_JS' failed.\n");
    }
}

static void* joystickRead_thread(void* joystickObject) { // constantly running and getting data
    Joystick* joystick = (Joystick*) joystickObject;
    while(true) {
        pthread_mutex_lock(&joystick->mID);
        // critical section
        joystick->xReading = joystickReadX(A2D_FILE_DIR, joystick->xPin);
        joystick->yReading = joystickReadY(A2D_FILE_DIR, joystick->yPin);
        // critical section end
        pthread_mutex_unlock(&joystick->mID);
    }
    return NULL;
}

static int joystickReadX(char* fileDirectory, char* xPin) {
    int xVal = getRawReadings(fileDirectory, xPin);
    return xVal;
}

static int joystickReadY(char* fileDirectory, char* yPin) {
    int yVal = getRawReadings(fileDirectory, yPin);
    return yVal;
}