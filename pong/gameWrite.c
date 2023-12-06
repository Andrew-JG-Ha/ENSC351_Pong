#include "gameWrite.h"
#include "gameParser.h"
#include "utility/i2c_utils.h"

// Take in two addresses and write data to both
void writeData(int i2cFileDesc, int boardSize, GameEncodings* encodings) {
//We have 4 matrices, and 2 addresses, 
    for (int i = 0; i < boardSize/2; i++) {
        //Write to MATRIX1
        //Write to MATRIX2
        writeToRegI2C(i2cFileDesc, MATRIX1, encodings.playerHalf1, BITS_TO_WRITE);
        writeToRegI2C(i2cFileDesc, MATRIX2, encodings.playerHalf2, BITS_TO_WRITE);
    }
    for (int i = boardSize/2; i < boardSize; i++) {
        //Write to MATRIX3
        //Write to MATRIX4
        writeToRegI2C(i2cFileDesc, MATRIX3, encodings.playerHalf1,BITS_TO_WRITE);
        writeToRegI2C(i2cFileDesc, MATRIX4, encodings.playerHalf2,BITS_TO_WRITE);
    }
}