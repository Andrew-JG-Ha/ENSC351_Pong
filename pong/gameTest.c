#include <stdio.h>
#include <stdlib.h>

#include "gameServer.h"
#include "gameLogic.h"


void printGame(const gameServer *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%d ", game->board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    gameServer pongGame;
    initializeGame(&pongGame);

    for (int i = 0; i < 5; i++) {
        updateGame(&pongGame);
        printGame(&pongGame);
    }

    return 0;
}
