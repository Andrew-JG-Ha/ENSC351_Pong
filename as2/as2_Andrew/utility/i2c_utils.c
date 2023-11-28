#include "i2c_utils.h"
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

int configPins(char* bus) {
    char command[MAX_LENGTH];
    int busNum = (int) (bus[strlen(bus) - 1] - '0');
    switch (busNum) {
    case 1:
        sprintf(command, "config-pin P9_17 i2c");
        runCommand(command);
        sprintf(command, "config-pin P9_18 i2c");
        runCommand(command);
        return 1;
        break;
    case 2:
        sprintf(command, "config-pin P9_19 i2c");
        runCommand(command);
        sprintf(command, "config-pin P9_20 i2c");
        runCommand(command);
        return 1;
    default:
        perror("ERROR: busline input is invalid\n");
        break;
    }
    return -1;
}

int initBus(char* bus, int address) {
    char fullFileDir[MAX_LENGTH];   
    sprintf(fullFileDir, "%s%s", I2C_FILE_DIR, bus);

    int i2cFileDesc = open(fullFileDir, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
    return i2cFileDesc;
}

void writeToRegI2C_multi(int i2cFileDesc, unsigned char regAddr, unsigned char* valueArr, int numberOfElements) {
    unsigned char buff[1 + numberOfElements];
    buff[0] = regAddr;
    for (int i = 0; i < numberOfElements; i++) {
        buff[i + 1] = valueArr[i];
    }
    int res = write(i2cFileDesc, buff, numberOfElements + 1);
    if (res != numberOfElements + 1) {
        perror("I2C: Unable to write i2c register.");
        exit(-1);
    }
}

void writeToRegI2C_single(int i2cFileDesc, unsigned char regAddr, unsigned char value) {
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(-1);
    }
}

unsigned char readRegI2C(int i2cFileDesc, unsigned char regAddr) {
    int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
    if (res != sizeof(regAddr)) {
        perror("I2C: Unable to write to i2c register.");
    exit(1);
    }
    char value = 0;
    res = read(i2cFileDesc, &value, sizeof(value));
    if (res != sizeof(value)) {
        perror("I2C: Unable to read from i2c register");
        exit(1);
    }
    return value;
}
