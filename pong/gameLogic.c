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

    game->directionX = 1;
    game->directionY = 1;

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

    for (int i = 1; i < BOARD_SIZE - 2; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] == PADDLE_BALL) {
                game->board[i][j] = EMPTY;

                int newI = i + game->directionY;
                int newJ = j + game->directionX;

                // bounce off wall (top/bottom)
                if (newI >= BOARD_SIZE) {
                    newI = i - 1;
                    game->directionY = -1;
                } else if (newI == 0) {
                    newI = i + 1;
                    game->directionY = 1;
                }

                // right paddle collision
                if (newI == BOARD_SIZE - 1 && game->board[newI][newJ] == PADDLE_BALL) {
                    newJ = j - 1;
                    game->directionX = -1;

                    srand(time(NULL));
                    int randomNumber = rand() % 2 + 1;

                    if (randomNumber == 0 && newI != BOARD_SIZE - 1) {
                        newI -= 1;
                        game->directionY = -1;
                    } else if (randomNumber == 1 && BOARD_SIZE != 0)
                        newI += 1;
                        game->directionY = 1;
                    }

                // left paddle collision
                if (newI == 0 && game->board[newI][newJ] == PADDLE_BALL) {
                    newJ = j + 1;  

                    srand(time(NULL));
                    int randomNumber = rand() % 2 + 1;

                    if (randomNumber == 0 && newI > 0) {
                        newI -= 1;
                        game->directionY = -1;
                    } else if (randomNumber == 1 && newI != BOARD_SIZE - 1) {
                        newI += 1;
                        game->directionY = 1;
                    }
                }

                game->board[newI][newJ] = PADDLE_BALL;

                if (j == 0 || j == BOARD_SIZE - 1) {
                    if (j == 0) {
                        game->scoreLeft++;
                    } else if (j == BOARD_SIZE - 1) {
                        game->scoreRight++;
                    }

                    if (game->scoreLeft == 3) {
                        // left win
                    } else if (game->scoreRight == 3) {
                        // right win
                    } else {
                        // restart
                        game->board[i][j] == EMPTY;

                        int ballX = (int)ceil((double)BOARD_SIZE/2);
                        int ballY = (int)ceil((double)BOARD_SIZE/2);
                        game->board[ballX][ballY] = PADDLE_BALL;

                        int randomNumber = rand() % 2 + 1;

                        if (randomNumber == 0) {
                            game->directionX = 1;
                        } else {
                            game->directionY = 1;
                        }

                    }
                }    
        }
    }

}