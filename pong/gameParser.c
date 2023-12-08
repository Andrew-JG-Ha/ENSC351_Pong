#include "gameParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ENCODING 16

static void intArr2Binary_split(int* intArr, int columns, unsigned char* returnAddress1, unsigned char* returnAddress2);
static void rotateNBits(unsigned char charVal, unsigned char* returnBuffer, int N);

GameEncodings* generateGameEncodings() {
    GameEncodings* newGameEncodings = malloc(sizeof(GameEncodings));
    newGameEncodings->playerHalf1 = calloc(sizeof(unsigned char), MAX_ENCODING); // 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    newGameEncodings->playerHalf2 = calloc(sizeof(unsigned char), MAX_ENCODING);
    return newGameEncodings;
}

void destroyGameEncodings(GameEncodings* gameEncodings) {
    free(gameEncodings->playerHalf1);
    free(gameEncodings->playerHalf2);
    gameEncodings->playerHalf1 = NULL;
    gameEncodings->playerHalf2 = NULL;
    free(gameEncodings);
    gameEncodings = NULL;
}

static void rotateColumnsVertically(int rotateAmount, unsigned char input[16][8], unsigned char* output[16][8]) {
    for (int col = 0; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
            int rotatedRow = (row + rotateAmount) % 8;
            output[row][col] = *input[rotatedRow][col];
        }
    }
}

void parseGameState(GameEncodings* gameEncodings, int boardSize, int board[][boardSize]) {
    // int halfBoardSize = boardSize / 2;
    //int positionInPH = 0;
    // bool reseted = false;
    for (int row = 0; row < boardSize; row++) {

        // if (row >= halfBoardSize  && reseted == false) {
        //     positionInPH = 1;
        //     reseted = true;
        // }

        intArr2Binary_split(board[row], boardSize, &gameEncodings->playerHalf1[row], &gameEncodings->playerHalf2[row]);
        printf("PH1: %d PH2: %d\n", gameEncodings->playerHalf1[row], gameEncodings->playerHalf2[row]);
        rotateNBits(gameEncodings->playerHalf1[row], &gameEncodings->playerHalf1[row], 1);
        rotateNBits(gameEncodings->playerHalf2[row], &gameEncodings->playerHalf2[row], 5);
        printf("PH1: %d PH2: %d\n", gameEncodings->playerHalf1[row], gameEncodings->playerHalf2[row]);
        // positionInPH = positionInPH + 1;
    }
    rotateColumnsVertically(6, gameEncodings->playerHalf1, &gameEncodings->playerHalf2[0]);
}


static void intArr2Binary_split(int* intArr, int columns, unsigned char* returnAddress1, unsigned char* returnAddress2) {
    int halfMark = columns / 2;
    char temp1[halfMark + 1];
    char temp2[halfMark + 1];
    for (int col = 0; col < halfMark ; col++) {
        temp1[col] = ((intArr[col] == 0) ? '0' : '1');
        temp2[col] = ((intArr[columns - col - 1] == 0) ? '0' : '1');
    }

    temp1[halfMark] = '\0';  // Null-terminate the string
    temp2[halfMark] = '\0';  // Null-terminate the string

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

// int main() {
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
// }