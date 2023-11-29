// i2c.h
// Module to initialize i2c busses and write to i2c registers.
// Contains i2c utility functions.
#ifndef I2C_H
#define I2C_H

int enable_I2C_Bus1(int deviceAddress);
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, int bytes, unsigned char* dataAddress);
#endif