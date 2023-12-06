#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "gameServer.h"
#define EMPTY 0
#define PADDLE_BALL 1

// initialize game, paddles/ball represented by 1
void initializeGame(gameServer *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < PADDLE_SIZE; i++) {
        game->board[i][0] = PADDLE_BALL;
        game->board[BOARD_SIZE - 1 - i][0] = PADDLE_BALL;
    }

    int ballX = (int)ceil((double)BOARD_SIZE/2);
    int ballY = (int)ceil((double)BOARD_SIZE/2);
    game->board[ballX][ballY] = PADDLE_BALL;

    game->scoreLeft = 0;
    game->scoreRight = 0;
}

void updateGame(gameServer *game) {
    int placeholder = 0;
    // player 1 (right) paddles, placeholder for inputs later
    // up
    if (placeholder == NULL) {
        // move up
        for (int i = 1; i < BOARD_SIZE; i++) {
            if (game->board[i][0] == PADDLE_BALL) {
                if (i - 1 >= 0) {
                    game->board[i][0] = EMPTY;
                    game->board[i-1][0] = PADDLE_BALL;
                }
            }
        }
    } else if (placeholder == NULL) {
        // move down
        for (int i = BOARD_SIZE - 2; i >= 0; i--) {
            if (game->board[i][0] == PADDLE_BALL) {
                if (i + 1 < BOARD_SIZE) {
                    game->board[i][0] = EMPTY;
                    game->board[i + 1][0] = PADDLE_BALL;
                }
            }
        }
    }

    // player 2 (left) paddles
    if (placeholder == NULL) {
        // move up
        for (int i = 1; i < BOARD_SIZE; i++) {
            if (game->board[i][BOARD_SIZE - 1] == PADDLE_BALL) {
                if (i - 1 >= 0) {
                    game->board[i][BOARD_SIZE - 1] = EMPTY;
                    game->board[i-1][BOARD_SIZE - 1] = PADDLE_BALL;
                }
            }
        }
    } else if (placeholder == NULL) {
        // move down
        for (int i = BOARD_SIZE - 2; i >= 0; i--) {
            if (game->board[i][BOARD_SIZE - 1] == PADDLE_BALL) {
                if (i + 1 < BOARD_SIZE) {
                    game->board[i][BOARD_SIZE - 1] = EMPTY;
                    game->board[i + 1][BOARD_SIZE - 1] = PADDLE_BALL;
                }
            }
        }
    }
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] == PADDLE_BALL) {
                game->board[i][j] = EMPTY;  
               
                i++; 
                j++; 

                // bounce walls
                if (i >= BOARD_SIZE || j >= BOARD_SIZE) {
                    // Bounce off the walls by reversing the direction
                    i = BOARD_SIZE - 2;
                    j = BOARD_SIZE - 2;

                    // Increment the score for the opposite side
                    if (j == BOARD_SIZE - 2) {
                        game->scoreLeft++;
                    } else {
                        game->scoreRight++;
                    }
                }

                // paddle bounce
                if (i < BOARD_SIZE && game->board[i][j] == PADDLE_BALL) {
                    i -= 2;
                }

                game->board[i][j] = PADDLE_BALL;

                if (j == 0 || j == BOARD_SIZE - 1) {
                    // GAMEOVER
            }
        }
    }

}