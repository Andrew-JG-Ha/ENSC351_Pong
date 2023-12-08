#ifndef GAMEWRITE_H
#define GAMEWRITE_H

#include "gameParser.h"

typedef struct MatrixHardware {
    int deviceAddress[4];
} MatrixHardware;

typedef struct Matrix {
    int i2cFileDesc[4];
    int deviceAddress[4];
} Matrix;

void writeData(int boardSize, Matrix* mat, GameEncodings* encodings);
Matrix* initMatrix(char* bus, MatrixHardware m);
void destroyMatrix(Matrix* m);
#endif