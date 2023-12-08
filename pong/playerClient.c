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
    newPlayer->joystick = generateJoystick(inputHardware.joystick.xpin, inputHardware.joystick.ypin, inputHardware.joystick.center, inputHardware.joystick.flip);
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
    destroyLed(player->joyStickLed);
    //stopButtonThread(player->profileSwitchButton);
    destroyButton(player->profileSwitchButton, false);
    destroyButton(player->downButton, false);
    destroyButton(player->upButton, false);
    destroyJoystick(player->joystick);
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
        perror("Player thread join failed. \n");
        exit(1);
    }
}

// void initPlayer1HW(InputHardware* inputHardware) {
//     inputHardware->buttonLed.pin = "p8.40";
//     inputHardware->buttonLed.gpioPin = "gpio77";
//     inputHardware->buttonLed.pinNumber = "77";

//     inputHardware->joyStickLed.pin = "p8.39";
//     inputHardware->joyStickLed.gpioPin = "gpio76";
//     inputHardware->joyStickLed.pinNumber = "76";

//     inputHardware->upButton.pin = "p8.33";
//     inputHardware->upButton.gpioPin = "gpio9";
//     inputHardware->upButton.pinNumber = "9";

//     inputHardware->downButton.pin = "p8.34";
//     inputHardware->downButton.gpioPin = "gpio81";
//     inputHardware->downButton.pinNumber = "81";

//     inputHardware->profileSwitchButton.pin = "p8.36";
//     inputHardware->profileSwitchButton.gpioPin = "gpio80";
//     inputHardware->profileSwitchButton.pinNumber = "80";

//     inputHardware->joystick.center = 2045;
//     inputHardware->joystick.flip = 1;
//     inputHardware->joystick.xpin = "in_voltage2_raw";
//     inputHardware->joystick.ypin = "in_voltage3_raw";
// }

// void initPlayer2HW(InputHardware* inputHardware) {
//     inputHardware->buttonLed.pin = "p8.16";
//     inputHardware->buttonLed.gpioPin = "gpio46";
//     inputHardware->buttonLed.pinNumber = "46";

//     inputHardware->joyStickLed.pin = "p8.15";
//     inputHardware->joyStickLed.gpioPin = "gpio47";
//     inputHardware->joyStickLed.pinNumber = "47";

//     inputHardware->upButton.pin = "p8.12";
//     inputHardware->upButton.gpioPin = "gpio44";
//     inputHardware->upButton.pinNumber = "44";

//     inputHardware->downButton.pin = "p8.13";
//     inputHardware->downButton.gpioPin = "gpio23";
//     inputHardware->downButton.pinNumber = "23";

//     inputHardware->profileSwitchButton.pin = "p8.11";
//     inputHardware->profileSwitchButton.gpioPin = "gpio45";
//     inputHardware->profileSwitchButton.pinNumber = "45";

//     inputHardware->joystick.center = 1789;
//     inputHardware->joystick.flip = 1;
//     inputHardware->joystick.xpin = "in_voltage5_raw";
//     inputHardware->joystick.ypin = "in_voltage6_raw";
// }


// int main() {
//     InputHardware inHW_player1;
//     InputHardware inHW_player2;
    
//     // initializing and defining pins 
//     initPlayer1HW(&inHW_player1);
//     initPlayer2HW(&inHW_player2);

//     Player* player1;
//     Player* player2;
//     player1 = generatePlayer(inHW_player1);
//     player2 = generatePlayer(inHW_player2);
//     // Start threads
//     runPlayerClient(player1);
//     runPlayerClient(player2);  
//     // waitForEdge();
//     int currTime = 0;
//     int startTime = getTimeInMilliS();
//     int play1 = 0;
//     int play2 = 0;

//     while(true) {
//         sleepForMs(100);
//         pthread_mutex_lock(&player1->mId);
//         play1 = player1->currPlayerDir;
//         //printf("Current Value: %d\n", play->currPlayerDir);
//         pthread_mutex_unlock(&player1->mId);
//         pthread_mutex_lock(&player2->mId);
//         play2 = player2->currPlayerDir;
//         //printf("Current Value: %d\n", play->currPlayerDir);
//         pthread_mutex_unlock(&player2->mId);
//         printf("P1:%d  P2:%d\n",play1,play2);
//         currTime = getTimeInMilliS();
//         if (currTime - startTime > 30000){
//             break;
//         }
//     }

//     // Cleanup
//     destroyPlayer(player1);
//     destroyPlayer(player2);
//     exit(1);
// }
