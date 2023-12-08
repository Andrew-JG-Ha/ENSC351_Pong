#ifndef GAMEWRITE_H
#define GAMEWRITE_H

#define BYTES_TO_WRITE 8

typedef struct MatrixHardware {
    unsigned char Matrix1;
    unsigned char Matrix2;
    unsigned char Matrix3;
    unsigned char Matrix4;
} MatrixHardware;

void writeData(int i2cFileDesc, int boardSize, MatrixHardware* mat, GameEncodings* encodings);

#endif