#include "gameParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENCODING 16

static void intArr2Binary_split(int* intArr, int columns, unsigned long* returnAddress1, unsigned long* returnAddress2);
static void orderEncodings(GameEncodings* gameEncodings);
static void rotateNBits(unsigned char charVal, unsigned char* returnBuffer, int N);

GameEncodings* generateGameEncodings(int boardSize) {
    GameEncodings* newGameEncodings = malloc(sizeof(GameEncodings));
    newGameEncodings->boardSize = boardSize;
    newGameEncodings->playerHalf1 = calloc(sizeof(unsigned char), MAX_ENCODING); // 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    newGameEncodings->playerHalf2 = calloc(sizeof(unsigned char), MAX_ENCODING);
    return newGameEncodings;
}

void destroyGameEncodings(GameEncodings* gameEncodings) {
    for (int i = 0; i < MAX_ENCODING; i++) {
        gameEncodings->playerHalf1[i] = NULL;
        gameEncodings->playerHalf2[i] = NULL;
    }
    gameEncodings->boardSize = NULL;
    free(gameEncodings->playerHalf1);
    free(gameEncodings->playerHalf2);
    free(gameEncodings);
}

void parseGameState(GameEncodings* gameEncodings, int** board) {
    int halfBoardSize = gameEncodings->boardSize / 2;
    for (int row = 0; row < gameEncodings->boardSize; row++) {
        intArr2Binary_split(board[row], gameEncodings->boardSize, gameEncodings->playerHalf1[row], gameEncodings->playerHalf2[row]);
    }

}

static void orderEncodings(GameEncodings* gameEncodings) {

}

static void intArr2Binary_split(int* intArr, int columns, unsigned long* returnAddress1, unsigned long* returnAddress2) {
    int halfMark = columns / 2;
    char temp1[halfMark];
    char temp2[halfMark];
    for (int col = 0; col < halfMark ; col++) {
        temp1[col] = ((intArr[col] == 0) ? '0' : '1');
        temp2[col] = ((intArr[col + halfMark] == 0) ? '0' : '1');
    }
    *returnAddress1 = (unsigned long) strtol(temp1, NULL, 2);
    *returnAddress2 = (unsigned long) strtol(temp2, NULL, 2);
}

static void rotateNBits(unsigned char charVal, unsigned char* returnBuffer, int N) { // if N > 0 then left rotate, if N < 0 then right rotate
    int moddedN = N % __CHAR_BIT__;
    int bitmask;
    int temp;
    if (N > 0) { // ROL(N)
        bitmask = (~(-1 << moddedN) << (__CHAR_BIT__ - moddedN)) & charVal;
        temp = (charVal << moddedN) | (bitmask >> (__CHAR_BIT__ - moddedN));
    }
    else { // ROR(N)
        bitmask = ~(-1 << moddedN) & charVal;
        temp = (charVal >> moddedN) | (bitmask << (__CHAR_BIT__ - moddedN));
    }
}