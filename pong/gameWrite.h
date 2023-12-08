#ifndef GAMEWRITE_H
#define GAMEWRITE_H

#include "gameParser.h"

#define BYTES_TO_WRITE 8

typedef struct MatrixHardware {
    Matrix Matrix1;
    Matrix Matrix2;
    Matrix Matrix3;
    Matrix Matrix4;
} MatrixHardware;

typedef struct Matrix {
    unsigned char deviceAddress;
    int i2cFileDesc;
} Matrix;

void writeData(int i2cFileDesc, int boardSize, MatrixHardware mat, GameEncodings* encodings);
int enableMatrix(char* bus, MatrixHardware m);

#endif