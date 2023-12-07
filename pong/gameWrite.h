#ifndef GAMEWRITE_H
#define GAMEWRITE_H

#define MATRIX1 0x70
#define MATRIX2 0x71
#define MATRIX3 0x72
#define MATRIX4 0x73
#define BYTES_TO_WRITE 8

typedef struct MatrixHardware {
    unsigned char Matrix1;
    unsigned char Matrix2;
    unsigned char Matrix3;
    unsigned char Matrix4;
} MatrixHardware;

void writeData(int i2cFileDesc, int boardSize, MatrixHardware* mat, GameEncodings* encodings);
void initMatrix(Matrix m, unsigned char m1, unsigned char m2, unsigned char m3, unsigned char m4);
void initializeI2C(void);

#endif