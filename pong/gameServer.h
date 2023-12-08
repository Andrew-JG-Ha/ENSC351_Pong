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
typedef struct OutputHardware {
typedef struct OutputHardware {
    LcdHardware ledScreen;
    MatrixHardware matrix;
} OutputHardware;
} OutputHardware;


typedef struct GameServer {
    pthread_t pId;
    int board[BOARD_SIZE][BOARD_SIZE];
    int scoreLeft;
    int scoreRight;
    int directionX;
    int directionY;
    Player* player1;
    Player* player2;
    LcdScreen* lcdScreen;
    MatrixHardware matrixHardware;
    

} gameServer;

GameServer* generateGameServer();
void destroyGameServer(GameServer* gameServer);

#endif