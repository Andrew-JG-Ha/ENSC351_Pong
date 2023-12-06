#include <stdio.h>
#include "gameServer.h"

void printBoard(gameServer *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%d ", game->board[i][j]);
        }
        printf("\n");
    }
    printf("Score Left: %d\n", game->scoreLeft);
    printf("Score Right: %d\n", game->scoreRight);
    printf("\n");
}

int main() {
    gameServer game;
    initializeGame(&game);

    printf("Initial State:\n");
    printBoard(&game);

    for (int step = 0; step < 5; step++) {
        updateGame(&game);

        printf("After Step %d:\n", step + 1);
        printBoard(&game);
    }

    return 0;
}
