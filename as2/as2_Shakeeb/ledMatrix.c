#include "utility.h"
#include "ledMatrix.h"
#include "i2c.h"
#include <math.h>

#define REG_DIRA 0x00
#define bitsInChar 8

static display baseDigit [12];

static void initializebaseDigit() {
    //Initialize 0
    baseDigit[0].row[7] = 0b10000011;
    baseDigit[0].row[6] = 0b10000010;
    baseDigit[0].row[5] = 0b10000010;
    baseDigit[0].row[4] = 0b10000010;
    baseDigit[0].row[3] = 0b10000010;
    baseDigit[0].row[2] = 0b10000010;
    baseDigit[0].row[1] = 0b10000011;
    baseDigit[0].row[0] = 0b00000000;

    //Initialize 1
    baseDigit[1].row[7] = 0b00000001;
    baseDigit[1].row[6] = 0b00000011;
    baseDigit[1].row[5] = 0b00000001;
    baseDigit[1].row[4] = 0b00000001;
    baseDigit[1].row[3] = 0b00000001;
    baseDigit[1].row[2] = 0b00000001;
    baseDigit[1].row[1] = 0b10000011;
    baseDigit[1].row[0] = 0b00000000;

    //Initialize 2
    baseDigit[2].row[7] = 0b10000011;
    baseDigit[2].row[6] = 0b10000000;
    baseDigit[2].row[5] = 0b10000000;
    baseDigit[2].row[4] = 0b10000011;
    baseDigit[2].row[3] = 0b00000010;
    baseDigit[2].row[2] = 0b00000010;
    baseDigit[2].row[1] = 0b10000011;
    baseDigit[2].row[0] = 0b00000000;

    //Initialize 3
    baseDigit[3].row[7] = 0b10000011;
    baseDigit[3].row[6] = 0b10000000;
    baseDigit[3].row[5] = 0b10000000;
    baseDigit[3].row[4] = 0b10000011;
    baseDigit[3].row[3] = 0b10000000;
    baseDigit[3].row[2] = 0b10000000;
    baseDigit[3].row[1] = 0b10000011;
    baseDigit[3].row[0] = 0b00000000;

    //Initialize 4
    baseDigit[4].row[7] = 0b10000010;
    baseDigit[4].row[6] = 0b10000010;
    baseDigit[4].row[5] = 0b10000010;
    baseDigit[4].row[4] = 0b10000011;
    baseDigit[4].row[3] = 0b10000000;
    baseDigit[4].row[2] = 0b10000000;
    baseDigit[4].row[1] = 0b10000000;
    baseDigit[4].row[0] = 0b00000000;

    //Initialize 5
    baseDigit[5].row[7] = 0b10000011;
    baseDigit[5].row[6] = 0b00000010;
    baseDigit[5].row[5] = 0b00000010;
    baseDigit[5].row[4] = 0b10000011;
    baseDigit[5].row[3] = 0b10000000;
    baseDigit[5].row[2] = 0b10000000;
    baseDigit[5].row[1] = 0b10000011;
    baseDigit[5].row[0] = 0b00000000;

    //Initialize 6
    baseDigit[6].row[7] = 0b10000011;
    baseDigit[6].row[6] = 0b00000010;
    baseDigit[6].row[5] = 0b00000010;
    baseDigit[6].row[4] = 0b10000011;
    baseDigit[6].row[3] = 0b10000010;
    baseDigit[6].row[2] = 0b10000010;
    baseDigit[6].row[1] = 0b10000011;
    baseDigit[6].row[0] = 0b00000000;

    //Initialize 7
    baseDigit[7].row[7] = 0b10000011;
    baseDigit[7].row[6] = 0b10000000;
    baseDigit[7].row[5] = 0b10000000;
    baseDigit[7].row[4] = 0b10000000;
    baseDigit[7].row[3] = 0b10000000;
    baseDigit[7].row[2] = 0b10000000;
    baseDigit[7].row[1] = 0b10000000;
    baseDigit[7].row[0] = 0b00000000;

    //Initialize 8
    baseDigit[8].row[7] = 0b10000011;
    baseDigit[8].row[6] = 0b10000010;
    baseDigit[8].row[5] = 0b10000010;
    baseDigit[8].row[4] = 0b10000011;
    baseDigit[8].row[3] = 0b10000010;
    baseDigit[8].row[2] = 0b10000010;
    baseDigit[8].row[1] = 0b10000011;
    baseDigit[8].row[0] = 0b00000000;

    //Initialize 9
    baseDigit[9].row[7] = 0b10000011;
    baseDigit[9].row[6] = 0b10000010;
    baseDigit[9].row[5] = 0b10000010;
    baseDigit[9].row[4] = 0b10000011;
    baseDigit[9].row[3] = 0b10000000;
    baseDigit[9].row[2] = 0b10000000;
    baseDigit[9].row[1] = 0b10000011;
    baseDigit[9].row[0] = 0b00000000;

    //Initialize .
    baseDigit[10].row[7] = 0b00000000;
    baseDigit[10].row[6] = 0b00000000;
    baseDigit[10].row[5] = 0b00000000;
    baseDigit[10].row[4] = 0b00000000;
    baseDigit[10].row[3] = 0b00000000;
    baseDigit[10].row[2] = 0b00000000;
    baseDigit[10].row[1] = 0b00000000;
    baseDigit[10].row[0] = 0b00001000;

    //Initialize blank
    baseDigit[10].row[7] = 0b00000000;
    baseDigit[10].row[6] = 0b00000000;
    baseDigit[10].row[5] = 0b00000000;
    baseDigit[10].row[4] = 0b00000000;
    baseDigit[10].row[3] = 0b00000000;
    baseDigit[10].row[2] = 0b00000000;
    baseDigit[10].row[1] = 0b00000000;
    baseDigit[10].row[0] = 0b00000000;
}

static display rotateLeft(int displayIndex, int rotationAmount) {
    display digit = baseDigit[displayIndex];
    unsigned char frontMask = (1U << rotationAmount) - 1;
    unsigned char endMask = ~frontMask;
    unsigned char frontPiece, endPiece;

    for (int i = 0; i < 8; i++) {
        frontPiece = frontMask & digit.row[i];
        endPiece = endMask & digit.row[i];
        digit.row[i] = ((frontPiece << (rotationAmount) | (endPiece >> (8 - rotationAmount))));
    }
    return digit;
}

static display combineDisplays(display d1, display d2) {
    display d3;
    for (int i = 0; i < 8; i++) {
        d3.row[i] = d1.row[i] | d2.row[i];
    }
    return d3;
}

void displayInt(int i2cFileDesc, int number) {
    number > 99 ? number = 99: number;
    number < 0  ? number = 0: number;
    int secondDigit = number % 10;
    int firstDigit = (number - secondDigit) / 10;
    display digit = rotateLeft(firstDigit, 5);
    display combinedDisplays = combineDisplays(digit, baseDigit[secondDigit]);
    writeI2cReg(i2cFileDesc, REG_DIRA, bitsInChar, &combinedDisplays.row[0]);
    // displayFirstDigit(i2cFileDesc,firstDigit);
    // displaySecondDigit(i2cFileDesc,secondDigit);
}

void displayFloat(int i2cFileDesc, float number) {
    number > 9.9 ? number = 9.9: number;
    number < 0.0 ? number = 0.0: number;
    number = number * 10;
    int secondDigit = (int) fmodf(number, 10.0);
    int firstDigit = (number - secondDigit) / 10;
    display digit = rotateLeft(firstDigit, 5);
    display combinedDisplays = combineDisplays(digit, baseDigit[secondDigit]);
    combinedDisplays = combineDisplays(combinedDisplays, baseDigit[10]);
    writeI2cReg(i2cFileDesc, REG_DIRA, bitsInChar, &combinedDisplays.row[0]);
}

void setupLEDMatrix(void) {
    runCommand("i2cset -y 1 0x70 0x21 0x00");
    runCommand("i2cset -y 1 0x70 0x81 0x00");
    initializebaseDigit();
}

void shutdownLEDMatrix(int* i2cFileDesc) {
    writeI2cReg(*i2cFileDesc, REG_DIRA, bitsInChar, &baseDigit[11].row[0]);
}