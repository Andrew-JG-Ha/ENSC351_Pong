// ledMatrix.h
// Module to write to the LED Matrix in the background (thread).
// It writes integers (0-99) and floats (0.0 - 9.9) to the LED Matrix.
#ifndef LEDMATRIX_H
#define LEDMATRIX_H

typedef struct {
    unsigned char row [8];
} display;

void displayInt(int i2cFileDesc, int number);
void displayFloat(int i2cFileDesc, float number);
void setupLEDMatrix(void);
void shutdownLEDMatrix(int* i2cFileDesc);

#endif