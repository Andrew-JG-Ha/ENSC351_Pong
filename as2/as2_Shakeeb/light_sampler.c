#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "sensor.h"
#include "sampler.h"
#include "analysis.h"
#include "i2c.h"
#include "ledMatrix.h"
#include "joystick.h"
#include <unistd.h>
#include <string.h>

#define LEDMATRIXADDRESS 0x70
#define USERBUTTONVALUE "/sys/class/gpio/gpio72/value"
#define BUFFLENGTH 32

int main() {

    //Execute config-pin in linux to force the USER button to be treated as GPIO
    runCommand("config-pin p8.43 gpio");

    //Create 32 character buffer to read input
    char buff[BUFFLENGTH];
    int res;

    //Checking if gpio72 exists or if it needs to be created.
    res = readFromFile("/sys/class/gpio/gpio72/value", buff, BUFFLENGTH);
    if (res == -1) {
        runCommand("echo 72 > /sys/class/gpio/export");
    }
    //Configure USER Button to be input
    runCommand("echo in > /sys/class/gpio/gpio72/direction");

    //Set USER button to be active high
    runCommand("echo 1 > /sys/class/gpio/gpio72/active_low");

    int i2cFileDesc = enable_I2C_Bus1(LEDMATRIXADDRESS);
    setupLEDMatrix();

    printf("Starting to sample data...\n");
    Sampler_startSampling();
    startAnalyzing();
    Joystick_startReading(&i2cFileDesc);
    while(1) {
        sleepForMs(100);
        char userBuff[BUFFLENGTH];
        readFromFile(USERBUTTONVALUE, userBuff, BUFFLENGTH);
        if ((int) (userBuff[0] - '0')) {
            break;
        }
    }
    Joystick_stopReading(&i2cFileDesc);
    stopAnalyzing();
    Sampler_stopSampling();
    close(i2cFileDesc);
    return 0;
}