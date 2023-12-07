#include "gameParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ENCODING 16

static void intArr2Binary_split(int* intArr, int columns, unsigned char* returnAddress1, unsigned char* returnAddress2);
static void orderEncodings(GameEncodings* gameEncodings);
static void rotateNBits(unsigned char charVal, unsigned char* returnBuffer, int N);

GameEncodings* generateGameEncodings() {
    GameEncodings* newGameEncodings = malloc(sizeof(GameEncodings));
    newGameEncodings->playerHalf1 = calloc(sizeof(unsigned char), MAX_ENCODING); // 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    newGameEncodings->playerHalf2 = calloc(sizeof(unsigned char), MAX_ENCODING);
    return newGameEncodings;
}

void destroyGameEncodings(GameEncodings* gameEncodings) {
    // for (int i = 0; i < MAX_ENCODING; i++) {
    //     gameEncodings->playerHalf1[i] = NULL;
    //     gameEncodings->playerHalf2[i] = NULL;
    // }
    free(gameEncodings->playerHalf1);
    free(gameEncodings->playerHalf2);
    free(gameEncodings);
}

void parseGameState(GameEncodings* gameEncodings, int boardSize, int board[][boardSize]) {
    int halfBoardSize = boardSize / 2;
    int positionInPH = 0;
    bool reseted = false;
    for (int row = 0; row < boardSize; row++) {

        if (row >= halfBoardSize  && reseted == false) {
            positionInPH = 1;
            reseted = true;
        }

        intArr2Binary_split(board[row], boardSize, &gameEncodings->playerHalf1[positionInPH], &gameEncodings->playerHalf2[positionInPH]);
        rotateNBits(gameEncodings->playerHalf1[positionInPH], &gameEncodings->playerHalf1[positionInPH], 1);
        rotateNBits(gameEncodings->playerHalf2[positionInPH], &gameEncodings->playerHalf2[positionInPH], 1);
        positionInPH = positionInPH + 2;
    }
}

static void intArr2Binary_split(int* intArr, int columns, unsigned char* returnAddress1, unsigned char* returnAddress2) {
    int halfMark = columns / 2;
    char temp1[halfMark];
    char temp2[halfMark];
    for (int col = 0; col < halfMark ; col++) {
        temp1[col] = ((intArr[col] == 0) ? '0' : '1');
        temp2[col] = ((intArr[col + halfMark] == 0) ? '0' : '1');
    }
    *returnAddress1 = (unsigned char) strtol(temp1, NULL, 2);
    *returnAddress2 = (unsigned char) strtol(temp2, NULL, 2);
}

static void rotateNBits(unsigned char charVal, unsigned char* returnBuffer, int N) { // if N > 0 then left rotate, if N < 0 then right rotate
    int moddedN = abs(N % __CHAR_BIT__);
    int bitmask;
    if (N > 0) { // ROL(N)
        bitmask = (~(-1 << moddedN) << (__CHAR_BIT__ - moddedN)) & charVal;
        *returnBuffer = (charVal << moddedN) | (bitmask >> (__CHAR_BIT__ - moddedN));
    }
    else { // ROR(N)
        bitmask = ~(-1 << moddedN) & charVal;
        *returnBuffer = (charVal >> moddedN) | (bitmask << (__CHAR_BIT__ - moddedN));
    }
}
