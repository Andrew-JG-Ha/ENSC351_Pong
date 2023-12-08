#ifndef GAME_SERVER
#define GAME_SERVER

#include <pthread.h>
#include "hardware/lcd.h"
#include "gameWrite.h"
#include "playerClient.h"

#define BOARD_SIZE 16
#define PADDLE_SIZE 3

/**
 * Game server would read the playerClient desired move ie up/down/nothing and update game state
 * and update the display
*/
typedef struct OutputHardware {
    LcdHardware ledScreen;
    MatrixHardware matrix;
} OutputHardware;

typedef struct GameServer {
    pthread_t pId;
    int board[BOARD_SIZE][BOARD_SIZE];
    int scoreLeft;
    int scoreRight;
    int ballX;
    int ballY;
    Player* player1;
    Player* player2;
    LcdScreen* lcdScreen;
    MatrixHardware matrixHardware;  
} GameServer;

GameServer* generateGameServer(Player* player1, Player* player2, OutputHardware hw);
void destroyGameServer(GameServer* gameServer);
void runGameServer();
void stopGameServer();

#endif
