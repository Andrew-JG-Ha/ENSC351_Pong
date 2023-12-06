#ifndef GAMEWRITE_H
#define GAMEWRITE_H

#define MATRIX1 0x70
#define MATRIX2 0x71
#define MATRIX3 0x72
#define MATRIX4 0x73
#define BITS_TO_WRITE

void writeData(int i2cFileDesc, int boardSize, GameEncodings* encodings);

#endif