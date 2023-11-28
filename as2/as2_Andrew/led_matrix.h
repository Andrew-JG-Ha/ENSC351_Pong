/**
 * led_matrix.h
 * This file contains a constructor, destructor (for LedMatrix) and methods to interact with the led matrix 
*/

#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "utility/i2c_utils.h"

typedef struct LedMatrix {
    unsigned char* matrixEncodings;
    char* bus;
    int fileDesc;
}LedMatrix;

LedMatrix* LedMatrix_new(char* bus, int config);

void LedMatrix_destroy(LedMatrix* object);

void writeD(LedMatrix* object, double value);

void writeI(LedMatrix* object, int value);

void writeE(LedMatrix* object, unsigned char* encoding);

void updateConfig(LedMatrix* object, int newConfig);

#endif
