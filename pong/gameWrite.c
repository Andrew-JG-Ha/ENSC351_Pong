#include "gameWrite.h"
#include "gameParser.h"
#include "../utility/i2c_utils.h"

// Take in two addresses and write data to both
void writeData(int i2cFileDesc, int boardSize, MatrixHardware* mat, GameEncodings* encodings) {
    //We have 4 matrices, and 2 addresses
    for (int i = 0; i < boardSize; i++) {
        if (i % 2) {
            writeToRegI2C(i2cFileDesc, mat->Matrix2, encodings->playerHalf1[i], BYTES_TO_WRITE);
            writeToRegI2C(i2cFileDesc, mat->Matrix4, encodings->playerHalf2[i], BYTES_TO_WRITE);
        }
        else {
            writeToRegI2C(i2cFileDesc, mat->Matrix1, encodings->playerHalf1[i], BYTES_TO_WRITE);
            writeToRegI2C(i2cFileDesc, mat->Matrix3, encodings->playerHalf2[i], BYTES_TO_WRITE);
        }
    }
}

MatrixHardware* generate_Matrix(unsigned char m1, unsigned char m2, unsigned char m3, unsigned char m4) {
    MatrixHardware* newMatrix = calloc(sizeof(MatrixHardware));
    newMatrix->Matrix1 = m1;
    newMatrix->Matrix1 = m2;
    newMatrix->Matrix1 = m3;
    newMatrix->Matrix1 = m4;
    return newMatrix;
}

void destroy_Matrix(MatrixHardware* m) {
    free(m);
    m = NULL;
}