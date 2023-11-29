// joystick.h
// Module to read joystick readings in the background
// and pass information on what statistic to display to the ledMatrix module(thread).
// It reads joystick input and determines which statistic is currently relevant.

#ifndef JOYSTICK_H
#define JOYSTICK_H

void Joystick_readXY(double* x, double* y);
void Joystick_startReading(int* i2cFileDesc);
void Joystick_stopReading(int* i2cFileDesc);

#endif