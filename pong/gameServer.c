#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utility/i2c_utils.h"
#include "gameServer.h"

#define EMPTY 0
#define PADDLE_BALL 1

static void* serverThread(void* serverObj);
static void initializeGame(GameServer *game);

GameServer* generateGameServer(Player* player1, Player* player2, OutputHardware hw) {
    GameServer* newGame = (GameServer*)malloc(sizeof(GameServer));
    memset(newGame -> board, 0, sizeof(newGame->board));
    newGame->scoreLeft = 0;
    newGame->scoreRight = 0;
    newGame->ballX = 0;
    newGame->ballY = 0;
    newGame->player1 = player1;
    newGame->player2 = player2; 
    newGame->matrixHardware = hw.matrix;
    newGame->lcdScreen = generateLcd(hw.lcdScreen);
    newGame->gameEncodings = generateGameEncodings();
    return newGame;
}

void destroyGameServer(GameServer* gameServer) {
    destroyLcd(gameServer->lcdScreen);
    gameServer->lcdScreen = NULL;
    destroyGameEncodings(gameServer->gameEncodings);
    gameServer->gameEncodings = NULL;
    free(gameServer);
    gameServer = NULL;
}

void runGameServer(GameServer* gameServer) {
    if (pthread_create(&gameServer->tId, NULL, serverThread, gameServer)) {
        perror("ERROR: couldn't initialize server thread");
    }
    pthread_detach(gameServer->tId);
}

static void* serverThread(void* serverObj) {
    GameServer* gameServer = (GameServer*) serverObj;
    initializeGame(gameServer);
    int fileDesc1; 
    // int fileDesc2 = 
    // int fileDesc3 = 
    // int fileDesc4 = 

    while (true) {
        parseGameState(gameServer->gameEncodings, BOARD_SIZE, gameServer->board);
        writeData(fileDesc1, BOARD_SIZE, gameServer->matrixHardware, gameServer->gameEncodings);
    }
    return NULL;
} 

void stopGameServer(GameServer* gameServer) {
    if (pthread_cancel(gameServer->tId)) {
        perror("ERROR: couldn't stop player thread");
    }
    if (!pthread_join(gameServer->tId, NULL)) {
        perror("Server thread join failed. \n");
        exit(1);
    }
}

// initialize game, paddles/ball represented by 1
static void initializeGame(GameServer *game) {
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

    game->ballX = 1;
    game->ballY = 1;
}

void updateGame(GameServer *game) {
    // player 1 (right) paddles, placeholder for inputs later
    // up
    if (game->player1->currPlayerDir == 1) {
        // move up
        for (int i = 1; i < BOARD_SIZE; i++) {
            if (game->board[i][0] == PADDLE_BALL) {
                if (i - 1 >= 0) {
                    game->board[i][0] = EMPTY;
                    game->board[i-1][0] = PADDLE_BALL;
                }
            }
        }
    } else if (game->player1->currPlayerDir == -1) {
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
    if (game->player2->currPlayerDir == 1) {
        // move up
        for (int i = 1; i < BOARD_SIZE; i++) {
            if (game->board[i][BOARD_SIZE - 1] == PADDLE_BALL) {
                if (i - 1 >= 0) {
                    game->board[i][BOARD_SIZE - 1] = EMPTY;
                    game->board[i-1][BOARD_SIZE - 1] = PADDLE_BALL;
                }
            }
        }
    } else if (game->player2->currPlayerDir == -1) {
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

                int newI = i + game->ballY;
                int newJ = j + game->ballX;

                // bounce off wall (top/bottom)
                if (newI >= BOARD_SIZE) {
                    newI = i - 1;
                    game->ballY = -1;
                } else if (i == 0) {
                    newI = i + 1;
                    game->ballY = 1;
                }

                // right paddle collision
                if (newJ == BOARD_SIZE - 1 && game->board[newI][newJ] == PADDLE_BALL) {
                    newJ = j - 1;
                    game->ballX = -1;
                }

                // left paddle collision
                if (newJ == 0 && game->board[newI][newJ] == PADDLE_BALL) {
                    newJ = j + 1;  
                    game->ballX = 1;
                }

                game->board[newI][newJ] = PADDLE_BALL;

                if (j == 0 || j == BOARD_SIZE - 1) {
                    // SCORE =============================================================
                    if (j == 0) {
                        game->scoreRight++;
                    } else if (j == BOARD_SIZE - 1) {
                        game->scoreLeft++;
                    }

                    if (game->scoreLeft == 3) {
                        // left win =================================
                    } else if (game->scoreRight == 3) {
                        // right win ==================================
                    } else {
                        // restart
                        game->board[i][j] = EMPTY;

                        int ballX = (int)ceil((double)BOARD_SIZE/2);
                        int ballY = (int)ceil((double)BOARD_SIZE/2);
                        game->board[ballX][ballY] = PADDLE_BALL;

                        game->ballX = 1;

                        game->ballY = 1;
                    }
                }    
            }
        }
    }
}
