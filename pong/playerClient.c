#include "playerClient.h"
#include <pthread.h>
#include "utility/utils.h"

Player* generatePlayer(HardwareParams hardwareParams) {
    Player* newPlayer = malloc(sizeof(Player));
    newPlayer->buttonLed = generateLed(hardwareParams.buttonLed);
    newPlayer->joyStickLed = generateLed(hardwareParams.joyStickLed);
    newPlayer->upButton = generateButton(hardwareParams.upButton);
    newPlayer->downButton = generateButton(hardwareParams.downButton);
    newPlayer->profileSwitchButton = generateButton(hardwareParams.profileSwitchButton);
    newPlayer->joystick = Joystick_new(hardwareParams.joystick.xpin, hardwareParams.joystick.ypin);
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
        
        if (isButtonInput == true) {
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
    free(player->buttonLed);
    player->buttonLed = NULL;
    destroyLed(player->joyStickLed);
    free(player->joyStickLed);
    player->joyStickLed = NULL;
    destroyButton(player->profileSwitchButton);
    free(player->profileSwitchButton);
    player->profileSwitchButton = NULL;
    destroyButton(player->downButton);
    free(player->downButton);
    player->downButton = NULL;
    destroyButton(player->upButton);
    free(player->upButton);
    player->upButton = NULL;
    Joystick_destroy(player->joystick);
    free(player->joystick);
    player->joystick = NULL;
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
}

int main() {
    struct HardwareParams h;
    h.buttonLed.pin = "p8.13";
    h.buttonLed.gpioPin = "gpio23";
    h.buttonLed.pinNumber = "23";

    h.joyStickLed.pin = "p8.46";
    h.joyStickLed.gpioPin = "gpio71";
    h.joyStickLed.pinNumber = "71";

    h.upButton.pin = "p8.7";
    h.upButton.gpioPin = "gpio66";
    h.upButton.pinNumber = "66";

    h.downButton.pin = "p8.9";
    h.downButton.gpioPin = "gpio69";
    h.downButton.pinNumber = "69";

    h.profileSwitchButton.pin = "p8.8";
    h.profileSwitchButton.gpioPin = "gpio67";
    h.profileSwitchButton.pinNumber = "67";

    h.joystick.xpin = "in_voltage2_raw";
    h.joystick.ypin = "in_voltage3_raw";

    Player* play = generatePlayer(h);

    if (pthread_create(&play->tId, NULL, playerThread, play)) {
        perror("ERROR");
    }
    pthread_detach(play->tId);

    int currTime = 0;
    int startTime = getTimeInMilliS();

    while(true) {
        sleepForMs(100);
        pthread_mutex_lock(&play->mId);
        printf("Current Value: %d\n", play->currPlayerDir);
        pthread_mutex_unlock(&play->mId);
        currTime = getTimeInMilliS();
        if (currTime - startTime > 10000){
            pthread_cancel(play->tId);
            destroyPlayer(play);
            exit(1);
        }
    }
}


