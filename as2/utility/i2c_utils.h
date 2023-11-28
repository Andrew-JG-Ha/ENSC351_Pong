/**
 * i2c_utils.h
 * This file contains all basic utility functions to interact with I2C files, such as configuring pins and writing to I2C files
*/

#ifndef I2C_H
#define I2C_H

#include "utils.h"
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_FILE_DIR "/dev/"
#define I2C_BUS_0 "i2c-0"
#define I2C_BUS_1 "i2c-1"
#define I2C_BUS_2 "i2c-2"

// Linux commands - for debugging purposes
int configPins(char* bus);

// I2C writing to and reading from registers 
int initBus(char* bus, int address);

void writeToRegI2C_single(int i2cFileDesc, unsigned char regAddr, unsigned char value);

void writeToRegI2C_multi(int i2cFileDesc, unsigned char regAddr, unsigned char* valueArr, int numberOfElements);

unsigned char readRegI2C(int i2cFileDesc, unsigned char regAddr);

#endif
