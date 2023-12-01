#ifndef GAME_SERVER
#define GAME_SERVER

#include <pthread.h>
#define BOARD_SIZE 8

/**
 * Game server would read the playerClient desired move ie up/down/nothing and update game state
 * and update the display
*/

typedef struct gameServer {
    pthread_t pId;
    int board[BOARD_SIZE][BOARD_SIZE];
} gameServer;

gameServer* generateGameServer();
void destroyGameServer(gameServer* gameServer);