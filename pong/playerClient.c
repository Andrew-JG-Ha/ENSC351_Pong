#include "playerClient.h"
#include <pthread.h>
#include "utility/utils.h"

Player* generatePlayer(InputHardware inputHardware) {
    Player* newPlayer = malloc(sizeof(Player));
    newPlayer->buttonLed = generateLed(inputHardware.buttonLed);
    newPlayer->joyStickLed = generateLed(inputHardware.joyStickLed);
    newPlayer->upButton = generateButton(inputHardware.upButton, false);
    newPlayer->downButton = generateButton(inputHardware.downButton, false);
    newPlayer->profileSwitchButton = generateButton(inputHardware.profileSwitchButton, false);
    newPlayer->joystick = Joystick_new(inputHardware.joystick.xpin, inputHardware.joystick.ypin);
    newPlayer->currPlayerDir = 0;
    pthread_mutex_init(&newPlayer->mId, NULL);
    return newPlayer;
}

static void* playerThread(void* playerObj) { // threaded
    Player* player = (Player*) playerObj;
    bool isButtonInput = false;
    int up = 0;
    int down = 0;
    int playerDir = 0;
    bool buttonPressed = false;
    while(true) {
        if (readButton(player->profileSwitchButton) == 1 && buttonPressed == false) {
            buttonPressed = true;
        } else if (readButton(player->profileSwitchButton) == 0 && buttonPressed == true) {
            buttonPressed = false;      // reset bool for next round; 
            if (isButtonInput == true) {
                isButtonInput = false;
            } else {
                isButtonInput = true;
            }
        }
        if (isButtonInput) {
            up = readButton(player->upButton);
            down = readButton(player->downButton);
            if (up == down) {
                playerDir = 0;
            } else if (up == 1) {
                playerDir = 1;
            } else {
                playerDir = -1;
            }
            turnLedOn(player->buttonLed);
            turnLedOff(player->joyStickLed);
        } else {    // JoyStick input
            playerDir = getUpDownDirection(player->joystick);
            turnLedOn(player->joyStickLed);
            turnLedOff(player->buttonLed);
        }
        pthread_mutex_lock(&player->mId);
        player->currPlayerDir = playerDir;
        // critical section
        //joystick->xReading = joystickReadX(A2D_FILE_DIR, joystick->xPin);
        //joystick->yReading = joystickReadY(A2D_FILE_DIR, joystick->yPin);
        //direction = encodeDirection(*joystick, direction);
        //decodeDirection(joystick, direction);
        // critical section end
        pthread_mutex_unlock(&player->mId);
    }
    return NULL;
}

void destroyPlayer(Player* player) {
    pthread_mutex_destroy(&player->mId);

    turnLedOff(player->buttonLed);
    turnLedOff(player->joyStickLed);
    destroyLed(player->buttonLed);
    destroyLed(player->joyStickLed);
    //stopButtonThread(player->profileSwitchButton);
    destroyButton(player->profileSwitchButton, false);
    destroyButton(player->downButton, false);
    destroyButton(player->upButton, false);
    Joystick_destroy(player->joystick);
    free(player);
    player = NULL;
}

void runPlayerClient(Player* player) {
    if (pthread_create(&player->tId, NULL, playerThread, player)) {
        perror("ERROR: couldn't initialize player thread");
    }
    pthread_detach(player->tId);
}

void stopPlayerClient(Player* player) {
    if (pthread_cancel(player->tId)) {
        perror("ERROR: couldn't stop player thread");
    }
    if (!pthread_join(player->tId, NULL)) {
        perror("Thread join failed. \n");
        exit(1);
    }
}