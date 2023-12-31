#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "gameServer.h"
#include "utility/i2c_utils.h"
#include "hardware/audio.h"

#define EMPTY 0
#define PADDLE_BALL 1

static void* serverThread(void* serverObj);
static void initializeGame(GameServer *game);

GameServer* generateGameServer(Player* player1, Player* player2, OutputHardware hw) {
    GameServer* newGame = (GameServer*)malloc(sizeof(GameServer));
    memset(newGame -> board, 0, sizeof(newGame->board));
    newGame->scoreLeft = 0;
    newGame->scoreRight = 0;
    newGame->ballRowVelo = 0;
    newGame->ballColVelo = 0;
    newGame->ballRowPos = 0;
    newGame->ballColPos = 0;
    newGame->player1 = player1;
    newGame->player2 = player2; 
    newGame->matrix = initMatrix(I2C_BUS_1, hw.matrix);
    // newGame->lcdScreen = generateLcd(hw.lcdScreen);
    newGame->gameEncodings = generateGameEncodings();

    return newGame;
}

void destroyGameServer(GameServer* gameServer) {
    // destroyLcd(gameServer->lcdScreen);
    // gameServer->lcdScreen = NULL;
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

    while (true) {
        parseGameState(gameServer->gameEncodings, BOARD_SIZE, gameServer->board);
        writeData(BOARD_SIZE, gameServer->matrix, gameServer->gameEncodings);
        updateGame(gameServer);
        sleepForMs(1000);
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
void initializeGame(GameServer *game) {
    
    bootUp();

    
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < PADDLE_SIZE; i++) {
            game->board[BOARD_SIZE/2 + i][0] = PADDLE_BALL;
            game->board[BOARD_SIZE/2 + i][BOARD_SIZE - 1] = PADDLE_BALL;
    }
    game->ballRowPos = (int)ceil((double)BOARD_SIZE/2);
    game->ballColPos = (int)ceil((double)BOARD_SIZE/2);

    int ballRowPos = game->ballRowPos;
    int ballColPos = game->ballColPos;
    game->board[ballRowPos][ballColPos] = PADDLE_BALL;

    game->ballRowVelo = 1;
    game->ballColVelo = 1;
}


void moveColumnDown(GameServer* game, int j) {
    int i = 0;

    while (i < BOARD_SIZE && game->board[i][j] != PADDLE_BALL) {
        i++;
    }

    if (i < BOARD_SIZE - PADDLE_SIZE) {
        game->board[i][j] = EMPTY;

        for (int k = 1; k <= 4; k++) {
            game->board[i + k][j] = PADDLE_BALL;
        }
    }
}

void moveColumnUp(GameServer* game, int j) {
    int i = BOARD_SIZE - 1;

    while (i > 0 && game->board[i][j] != PADDLE_BALL) {
        i--;
    }

    if (i >= PADDLE_SIZE) {
        game->board[i][j] = EMPTY;

        for (int k = 1; k <= 4; k++) {
            game->board[i - k][j] = PADDLE_BALL;
        }
    }
}


void updateGame(GameServer *game) {
    // player 1 (right) paddles
    if (game->player1->currPlayerDir == 1) {
        // move up
        moveColumnUp(game, BOARD_SIZE - 1);
    } else if (game->player1->currPlayerDir == -1) {
        // move down
        moveColumnDown(game, BOARD_SIZE - 1);
    }

    // player 2 (left) paddles
    if (game->player2->currPlayerDir == 1) {
        // move up
        moveColumnUp(game, 0);
    } else if (game->player2->currPlayerDir == -1) {
        // move down
        moveColumnDown(game, 0);
    }

    // find ball and move according to ballrowvballRowVelo/col
    // currentState
    game->board[game->ballRowPos][game->ballColPos] = EMPTY;

    //nextState
    int nextStateCol = game->ballColPos + game->ballColVelo;
    int nextStateRow = game->ballRowPos + game->ballRowVelo;
    
    // paddle collision (move across coloumns)
    if (game->board[nextStateRow][nextStateCol] == PADDLE_BALL) {
        ping();
        game->ballColVelo = -game->ballColVelo;
        nextStateCol = game->ballColPos;
    } 

    // top/bot collision (move across rows)
    if (nextStateRow < 0 || nextStateRow > BOARD_SIZE - 1) {
        game->ballRowVelo = -game->ballRowVelo;
        nextStateRow = game->ballRowPos;
    }

    if(nextStateCol < 0) {
        nextStateCol = 8;
        nextStateRow = 8;
        game->scoreLeft++;
        game->ballColVelo = 1;
        game->ballRowVelo = 1;
        printf("scored on left player\n");


    } else if (nextStateCol > BOARD_SIZE - 1) {
        nextStateCol = 8;
        nextStateRow = 8;
        game->scoreRight++;
        game->ballColVelo = 1;
        game->ballRowVelo = 1;
        printf(" scored on right player\n");

    }

    // printf("left score is: %d, right score is %d\n", game->scoreLeft, game->scoreRight);
    // check scores and determine if match won
    if (game->scoreLeft == 3 ) {
        speak("game won by right player");
        printf("game won by right player\n");
        pthread_exit(NULL);
        // SOUND/LCD =======================

    }
    
    if (game->scoreRight == 3) {
        speak("game won by right player");
        printf("game won by right player\n");
        pthread_exit(NULL);
        // SOUND/LCD =======================
    }

    game->ballColPos = nextStateCol;
    game->ballRowPos = nextStateRow;
    // set new position 
    game->board[game->ballRowPos][game->ballColPos] = PADDLE_BALL;
    
}