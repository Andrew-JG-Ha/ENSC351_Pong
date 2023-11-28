/**
 * Code By Andrew Ha
 * 301421903
 * Nov/27/2023
 * 
*/

#include "light_sensor.h"
#include "joystick.h"
#include "led_matrix.h"
#include "analysis.h"
#include <math.h>

#define USER_BUTTON "/sys/class/gpio/gpio72/"

static long long holdDuration = 25; // ms
static pthread_t userThreadID;

void setup();

void cleanup(PackageStats* statsPackage, LightS* sensor, Joystick* joystick, Package* package, LedMatrix* ledMatrix);

void startReading_User(bool* user_input);

void* user_thread(void* user_input);

int mapDirection(Joystick joystick, int previousDescision);

int normalizeAroundZero(int value);

int main() {
    bool userPressed = false;
    int descision = 0;
    PackageStats* statsPackage = PackageStat_new();
    LightS* lightSensor = LightS_new(A2D_FILE_DIR, ANALOG_1);
    Package* package = Package_new(lightSensor, statsPackage);
    Joystick* joystick = Joystick_new(ANALOG_2, ANALOG_3);
    LedMatrix* ledMatrix = LedMatrix_new(I2C_BUS_1, 1);
    setup();

    printf("Starting to sample data...\n");
    startSampling_LS(lightSensor);
    startAnalysis(package);
    startReading_JS(joystick);
    startReading_User(&userPressed);

    while(!userPressed) {
        /**
         * 0 = joystick centered (number of dips)
         * 1 = joystick up (max sample voltage) -X, Y don't care
         * 2 = joystick down (min sample voltage) +X, Y don't care
         * 3 = joystick left (min interval between samples) +Y, X don't care
         * 4 = joystick right (max interval between samples) -Y, X don't care
        */
        descision = mapDirection(*joystick, descision);
        switch (descision)
        {
        case 1:
            writeD(ledMatrix, statsPackage->maxReading);
            break;
        case 2:
            writeD(ledMatrix, statsPackage->minReading);
            break;
        case 3:
            writeD(ledMatrix, statsPackage->minInterval);
            break;
        case 4:
            writeD(ledMatrix, statsPackage->maxInterval);
            break; 
        default:
            // printf("numDips: %d\n", statsPackage->numDips);
            writeI(ledMatrix, statsPackage->numDips);
            break;
        }
    }

    printf("Shutting down...\n");
    stopAnalysis(package);// dips
    stopReading_JS(joystick);
    stopSampling_LS(lightSensor);
    cleanup(statsPackage, lightSensor, joystick, package, ledMatrix);
    printf("Done shutdown! Goodbye!\n");
}

void cleanup(PackageStats* statsPackage, LightS* sensor, Joystick* joystick, Package* package, LedMatrix* ledMatrix) {
    LightS_destroy(sensor);
    Joystick_destroy(joystick);
    PackageStat_destroy(statsPackage);
    Package_destroy(package);
    LedMatrix_destroy(ledMatrix);
}

void setup() {
    char buffer[MAX_LENGTH]; 
    runCommand("config-pin p8.43 gpio");
    if (readFile(USER_BUTTON, "direction", buffer) == -1) {
        writeToFile(GPIO_FILE_DIR, "export", "72");
    }
    runCommand("echo in > /sys/class/gpio/gpio72/direction");
    runCommand("echo 1 > /sys/class/gpio/gpio72/active_low");
    runCommand("echo rising > /sys/class/gpio/gpio72/edge");
    runCommand("i2cset -y 1 0x70 0x21 0x00");
    runCommand("i2cset -y 1 0x70 0x81 0x00");
}

void startReading_User(bool* user_input) {
    if (pthread_create(&userThreadID, NULL, user_thread, user_input)) {
        perror("Error: pthread_create in 'startReading_User' failed. \n");
    }
}

void* user_thread(void* user_input) {
    bool* userPressed = (bool*) user_input;
    char activeState[MAX_LENGTH];
    readFile(USER_BUTTON, "active_low", activeState);
    if (awaitChange(USER_BUTTON, "value", holdDuration, -1, activeState, NULL, NULL)) {
        *userPressed = true;
    }
    pthread_exit(NULL);
}

int mapDirection(Joystick joystick, int previousDescision) {
    /**
     * 0 = joystick centered (number of dips)
     * 1 = joystick up (max sample voltage) -X, Y don't care
     * 2 = joystick down (min sample voltage) +X, Y don't care
     * 3 = joystick left (min interval between samples) +Y, X don't care
     * 4 = joystick right (max interval between samples) -Y, X don't care
    */
    int middleValue = A2D_MAX_READING / 2;
    int joystickThreshold = middleValue * 0.5;
    int mappedX = normalizeAroundZero(joystick.xReading);
    int mappedY = normalizeAroundZero(joystick.yReading);
    // x == y is along the diagonal
    // printf("hypotenuse: %f, threshold: %f\n", pow(mappedX, 2) + pow(mappedY, 2), pow(joystickThreshold, 2));
    if (pow(mappedX, 2) + pow(mappedY, 2) <= pow(joystickThreshold, 2)) {
        return 0;
    }
    else {
        // printf("joystickX: %d, joystickY: %d, joystick reading: %d\n", mappedX, mappedY, joystickThreshold);   
        if (abs(mappedX) > abs(mappedY) && abs(mappedY) < joystickThreshold) {
            // printf("I am in the mappedX > abs(mappedY)\n");
            if (mappedX < abs(mappedY)) {
                return 1;
            }
            else {
                return 2;
            }
        }
        else if (abs(mappedX) < abs(mappedY) && abs(mappedX) < joystickThreshold) {
            // printf("I am in the mappedX > abs(mappedX)\n");
            if (mappedY < abs(mappedX)) {
                return 4;
            }
            else {
                return 3;
            }
        }
        else {
            return previousDescision;
        }
    }
}

int normalizeAroundZero(int value) {
    int result = (value - A2D_MAX_READING / 2);
    return result;
}