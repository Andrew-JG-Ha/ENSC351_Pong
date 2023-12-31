#ifndef GAME_SERVER
#define GAME_SERVER

#include <pthread.h>
#include "hardware/lcd.h"
#include "gameWrite.h"
#include "playerClient.h"
#include "gameParser.h"


#define BOARD_SIZE 16
#define PADDLE_SIZE 4

/**
 * Game server would read the playerClient desired move ie up/down/nothing and update game state
 * and update the display
*/

typedef struct OutputHardware {
    LcdHardware lcdScreen;
    MatrixHardware matrix;
} OutputHardware;

typedef struct GameServer {
    pthread_t tId;
    int board[BOARD_SIZE][BOARD_SIZE];
    int scoreLeft;
    int scoreRight;
    int ballX;
    int ballY;
    int ballRowVelo;
    int ballColVelo;
    int ballRowPos;
    int ballColPos;
    Player* player1;
    Player* player2;
    // LcdScreen* lcdScreen;
    Matrix* matrix;
    GameEncodings* gameEncodings;
} GameServer;


GameServer* generateGameServer(Player* player1, Player* player2, OutputHardware hw);
void destroyGameServer(GameServer* gameServer);
void runGameServer(GameServer* gameServer);
void stopGameServer(GameServer* gameServer);
void updateGame(GameServer *game);
void moveColumnDown(GameServer* game, int j);
void moveColumnUp(GameServer* game, int j);

#endif
