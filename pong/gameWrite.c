#include "gameWrite.h"
#include "utility/i2c_utils.h"

// Take in two addresses and write data to both
void writeData(int i2cFileDesc, int boardSize, MatrixHardware mat, GameEncodings* encodings) {
    //We have 4 matrices, and 2 addresses
    for (int i = 0; i < boardSize; i++) {
        if (i % 2) {
            writeToRegI2C(i2cFileDesc, mat.Matrix2, encodings->playerHalf1[i], BYTES_TO_WRITE);
            writeToRegI2C(i2cFileDesc, mat.Matrix4, encodings->playerHalf2[i], BYTES_TO_WRITE);
        }
        else {
            writeToRegI2C(i2cFileDesc, mat.Matrix1, encodings->playerHalf1[i], BYTES_TO_WRITE);
            writeToRegI2C(i2cFileDesc, mat.Matrix3, encodings->playerHalf2[i], BYTES_TO_WRITE);
        }
    }
}

int enableMatrix(char* bus, MatrixHardware m) {

}
