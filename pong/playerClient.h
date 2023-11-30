#ifndef PLAYER_CLIENT_H
#define PLAYER_CLIENT_H

#include <pthread.h>

/**
 * Player client would use joystick or button packages to obtain current "action" from hardware
*/

typedef struct Player {
    pthread_t tId;
    pthread_mutex_t mId;
    int currPlayerDir;      

    /**
     * vars for pins etc, would be dependent on hardware
    */

} Player;

Player* generatePlayer();
void destroyPlayer(Player* player);
void startPlayer(Player* player);
void stopPlayer(Player* player);

#endif