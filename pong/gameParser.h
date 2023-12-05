#ifndef GAME_PARSER_H
#define GAME_PARSER_H

typedef struct GameEncodings {
    unsigned char* playerHalf1;
    unsigned char* playerHalf2;
} GameEncodings;

GameEncodings* generateGameEncodings();

void destroyGameEncodings(GameEncodings* gameEncodings);

void parseGameState(GameEncodings* gameEncodings, int boardSize, int board[][boardSize]);

#endif