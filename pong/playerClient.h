#ifndef PLAYER_CLIENT_H
#define PLAYER_CLIENT_H

#include <pthread.h>
#include "hardware/led.h"
#include "hardware/gpioButton.h"
#include "hardware/joystick.h"

/**
 * Player client would use joystick or button packages to obtain current "action" from hardware
*/
typedef struct HardwareParams {
    LedHardware joyStickLed;
    LedHardware buttonLed;
    ButtonHardware profileSwitchButton;
    ButtonHardware upButton;
    ButtonHardware downButton;
    JoystickHardware joystick;
} HardwareParams;

typedef struct Player {
    pthread_t tId;
    pthread_mutex_t mId;
    int currPlayerDir;
    Led* joyStickLed;
    Led* buttonLed;
    GpioButton* profileSwitchButton;
    GpioButton* upButton;
    GpioButton* downButton;
    Joystick* joystick;
} Player;

Player* generatePlayer(HardwareParams hardwareParams);
void destroyPlayer(Player* player);
void runPlayerClient(Player* player);
void stopPlayerClient(Player* player);

#endif