#ifndef GAME_PARSER_H
#define GAME_PARSER_H

typedef struct GameEncodings {
    unsigned char* playerHalf1;
    unsigned char* playerHalf2;
    int boardSize;
} GameEncodings;

GameEncodings* generateGameEncodings(int boardSize);

void destroyGameEncodings(GameEncodings* gameEncodings);

void parseGameState(GameEncodings* gameEncodings, int** board);

#endif