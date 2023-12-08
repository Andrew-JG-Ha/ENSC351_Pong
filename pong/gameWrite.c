#include "gameWrite.h"
#include "utility/i2c_utils.h"
#include "utility/utils.h"
#include <string.h>
#include <stdio.h>

#include <stdlib.h>

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define BYTES_TO_WRITE 8
#define MAX_LENGTH 1024


// static void invertBits(unsigned char input, unsigned char* returnAddress1) {
//     // Convert the integer to an 8-bit binary string
//     unsigned char binaryString[9];  // 8 bits + null terminator

//     // Invert each bit in the binary string
//     for (int i = 0; i < 8; i++) {
//         binaryString[i] = (returnAddress1[i] == '0') ? '1' : '0';
//     }
//     binaryString[8] = '\0';

//     // Convert the inverted binary string back to an integer
//     *returnAddress1 = (unsigned char) strtol(binaryString, NULL, 2);
// }

// Take in two addresses and write data to both
void writeData(int boardSize, Matrix* mat, GameEncodings* encodings) {
    //We have 4 matrices, and 2 addresses
    // for (int i = 0; i < boardSize; i++) {
    //     if (i % 2) {
    //         printf("encodings: %s, i = %d\n", &encodings->playerHalf1[i], i);
    //         writeI2CReg(mat->i2cFileDesc[1], 0x00, BYTES_TO_WRITE, &encodings->playerHalf1[i]);
    //         writeI2CReg(mat->i2cFileDesc[3], 0x00, BYTES_TO_WRITE, &encodings->playerHalf2[i]);
    //     }
    //     else {
    //         writeI2CReg(mat->i2cFileDesc[0], 0x00, BYTES_TO_WRITE, &encodings->playerHalf1[i]);
    //         writeI2CReg(mat->i2cFileDesc[2], 0x00, BYTES_TO_WRITE, &encodings->playerHalf2[i]);
    //     }
    // }

    // for (int i = 0; i < 8; i++) {
    //     invertBits(encodings->playerHalf1[i], &encodings->playerHalf1[i]);
    // }

    writeI2CReg(mat->i2cFileDesc[0], 0x00, BYTES_TO_WRITE, &encodings->playerHalf1[0]); // 0
    writeI2CReg(mat->i2cFileDesc[1], 0x00, BYTES_TO_WRITE, &encodings->playerHalf1[8]); // 8
    writeI2CRegReverse(mat->i2cFileDesc[2], 0x00, BYTES_TO_WRITE, &encodings->playerHalf2[8]); // 7
    writeI2CRegReverse(mat->i2cFileDesc[3], 0x00, BYTES_TO_WRITE, &encodings->playerHalf2[16]); // 15
}

Matrix* initMatrix(char* bus, MatrixHardware m) {
    Matrix* newMatrix = malloc(sizeof(Matrix));
    configPins(bus);
    char command[MAX_LENGTH];
    for (int i = 0; i < 4; i++) {
        newMatrix->i2cFileDesc[i] = initBus(bus, m.deviceAddress[i]);
        newMatrix->deviceAddress[i] = m.deviceAddress[i];


        sprintf(command, "%s%d%s", "i2cset -y 1 ", newMatrix->deviceAddress[i], " 0x21 0x00");
        runCommand(command);
        memset(command, '\0', sizeof(command));
        sprintf(command, "%s%d%s", "i2cset -y 1 ", newMatrix->deviceAddress[i], " 0x81 0x00");
        runCommand(command);
        // writeI2CReg(newMatrix->i2cFileDesc[i], 0x21, 1, 0x00);
        // writeI2CReg(newMatrix->i2cFileDesc[i], 0x81, 1, 0x00);
    }
    return newMatrix;
}

void destroyMatrix(Matrix* m) {
    free(m);
    m = NULL;
}

// int main() {
//     // MatrixHardware m;
//     // m.deviceAddress[0] = 0x70;
//     // m.deviceAddress[1] = 0x71;
//     // m.deviceAddress[2] = 0x72;
//     // m.deviceAddress[3] = 0x73;
//     // Matrix* mat = initMatrix(I2C_BUS_1, m);
//     int board[16][16];
//     for (int i = 0; i < 16; i++) {
//         for (int j = 0; j < 16; j++) {
//             board[i][j] = 0;
//         }
//     }
//     board[0][0] = 1;
//     board[0][5] = 1;
//     board[1][0] = 1;
//     board[2][0] = 1;
//     board[7][15] = 1;
//     board[8][15] = 1;
//     board[9][15] = 1;

//     printf("initial setup: \n");
//     for (int i = 0; i< 16; i++) {
//         for (int j = 0; j < 16; j++) {
//             printf("%d", board[i][j]);
//         }
//         printf("\n");
//     }
//     printf("\n");
//     GameEncodings* test = generateGameEncodings();
//     parseGameState(test, 16, board);
//     printf("post parsing: \n");
//     for (int i = 0; i < 16; i++) {
//         printf("row: %d, PH1: %d, PH2: %d\n", i, test->playerHalf1[i], test->playerHalf2[i]);
//     }
//     // GameEncodings* test2 = malloc(sizeof(GameEncodings));
//     // unsigned char tests [64];
//     // for (int i = 0; i < 64; i++) {
//     //     tests[i] = 0x00;
//     // }
//     // tests[0] = 0xff;
//     // test2->playerHalf1 = tests;
//     // test2->playerHalf2 = tests;
//     // //writeData(16, mat, test);
//     // free(test2);
// }