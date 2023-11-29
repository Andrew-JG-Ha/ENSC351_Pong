#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "utility.h"

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

static int initI2cBus(char* bus, int address) {
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
    return i2cFileDesc;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr,
int bytes, unsigned char* dataAddress) {
    unsigned char buff[2 * bytes + 1];
    buff[0] = regAddr;
    for (int i = 1; i < 2 * bytes + 1; i++) {
        if (i % 2) {
            buff[i] = *(dataAddress + (i/2));
        }
        else { 
            buff[i] = 0;
        }
    }
    int res = write(i2cFileDesc, buff, 2 * bytes + 1);
    if (res != (2 * bytes + 1)) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

int enable_I2C_Bus1(int deviceAddress) {
    configPin("p9_18", "i2c");
    configPin("p9_17", "i2c");
    return initI2cBus(I2CDRV_LINUX_BUS1, deviceAddress);
}
