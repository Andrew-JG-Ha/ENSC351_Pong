#include "gameWrite.h"
#include "utility/i2c_utils.h"
#include "utility/utils.h"

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define BYTES_TO_WRITE 8

// Take in two addresses and write data to both
void writeData(int boardSize, Matrix* mat, GameEncodings* encodings) {
    //We have 4 matrices, and 2 addresses
    for (int i = 0; i < boardSize; i++) {
        if (i % 2) {
            writeI2CReg(mat->i2cFileDesc[1], mat->deviceAddress[1], BYTES_TO_WRITE, encodings->playerHalf1 + i);
            writeI2CReg(mat->i2cFileDesc[3], mat->deviceAddress[3], BYTES_TO_WRITE, encodings->playerHalf2 + i);
        }
        else {
            writeI2CReg(mat->i2cFileDesc[0], mat->deviceAddress[0], BYTES_TO_WRITE, encodings->playerHalf1 + i);
            writeI2CReg(mat->i2cFileDesc[2], mat->deviceAddress[2], BYTES_TO_WRITE, encodings->playerHalf2 + i);
        }
    }
}

Matrix* initMatrix(char* bus, MatrixHardware m) {
    Matrix* newMatrix = malloc(sizeof(Matrix));
    configPins(bus);
    for (int i = 0; i < 4; i++) {
        newMatrix->i2cFileDesc[i] = initBus(bus, m.deviceAddress[i]);
        newMatrix->deviceAddress[i] = m.deviceAddress[i];
        writeI2CReg(newMatrix->i2cFileDesc[i], 0x21, 1, 0x00);
        writeI2CReg(newMatrix->i2cFileDesc[i], 0x81, 1, 0x00);
    }
    return newMatrix;
}

void destroyMatrix(Matrix* m) {
    free(m);
    m = NULL;
}

int main() {
    MatrixHardware m;
    m.deviceAddress[0] = 0x70;
    m.deviceAddress[1] = 0x72;
    m.deviceAddress[2] = 0x71;
    m.deviceAddress[3] = 0x73;
    Matrix* mat = initMatrix(I2C_BUS_1, m);
    int board[16][16];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            board[i][j] = 0;
        }
    }
    board[0][0] = 1;
    board[0][5] = 1;
    board[1][0] = 1;
    board[2][0] = 1;
    board[7][15] = 1;
    board[8][15] = 1;
    board[9][15] = 1;

    printf("initial setup: \n");
    for (int i = 0; i< 16; i++) {
        for (int j = 0; j < 16; j++) {
            printf("%d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    GameEncodings* test = generateGameEncodings();
    parseGameState(test, 16, board);
    printf("post parsing: \n");
    for (int i = 0; i < 16; i++) {
        printf("row: %d, PH1: %d, PH2: %d\n", i, test->playerHalf1[i], test->playerHalf2[i]);
    }
    writeData(16, mat, test);
}