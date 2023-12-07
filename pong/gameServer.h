#ifndef GAME_SERVER
#define GAME_SERVER

#include <pthread.h>
#include "hardware/lcd.h"
#include "playerClient.h"

#define BOARD_SIZE 16
#define PADDLE_SIZE 3

/**
 * Game server would read the playerClient desired move ie up/down/nothing and update game state
 * and update the display
*/
typedef struct outputHardware {
    LcdHardware ledScreen;
    MatrixHardware matrix;
} outputHardware;


typedef struct gameServer {
    pthread_t pId;
    int board[BOARD_SIZE][BOARD_SIZE];
    int scoreLeft;
    int scoreRight;
    int directionX;
    int directionY;
    Player* player1;
    Player* player2;
} gameServer;

gameServer* generateGameServer();
void destroyGameServer(gameServer* gameServer);

#endif