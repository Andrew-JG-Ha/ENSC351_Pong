#include "playerClient.h"
#include "gameServer.h"

#define USER_BUTTON "/sys/class/gpio/gpio72/"
#define MAX_LENGTH 1024

#define matrixAddress1 

GameServer* createGameServer(Player* player1, Player* player2);
void initPlayer2HW(InputHardware* inputHardware);
void initPlayer1HW(InputHardware* inputHardware);
void cleanup(Player* player1, Player* player2, GameServer* server);
void initBoardHW();

int main() {
    InputHardware inHW_player1;
    InputHardware inHW_player2;
    OutputHardware outHW;
    Pin dataPins[4];
    
    // initializing and defining pins 
    outHW.ledScreen.dataPins = dataPins;
    initPlayer1HW(&inHW_player1);
    initPlayer2HW(&inHW_player2);
    initGameServerHW(&outHW);
    initBoardHW();

    Player* player1;
    Player* player2;
    GameServer* gameServer;
    player1 = generatePlayer(inHW_player1);
    player2 = generatePlayer(inHW_player2);
    gameServer = generateGameServer();

    // Start threads
    runPlayerClient(player1);
    runPlayerClient(player2);    


    waitForEdge();

    // Cleanup
    cleanup(player1, player2, gameServer);
}

void initGameServerHW(OutputHardware* outputHardware) {
    outputHardware->ledScreen.dataPins[0].pin = "p8.27";
    outputHardware->ledScreen.dataPins[0].gpioPin = "gpio86";
    outputHardware->ledScreen.dataPins[0].pinNumber = "86";

    outputHardware->ledScreen.dataPins[1].pin = "p8.28";
    outputHardware->ledScreen.dataPins[1].gpioPin = "gpio88";
    outputHardware->ledScreen.dataPins[1].pinNumber = "88";

    outputHardware->ledScreen.dataPins[2].pin = "p8.29";
    outputHardware->ledScreen.dataPins[2].gpioPin = "gpio87";
    outputHardware->ledScreen.dataPins[2].pinNumber = "87";

    outputHardware->ledScreen.dataPins[3].pin = "p8.30";
    outputHardware->ledScreen.dataPins[3].gpioPin = "gpio89";
    outputHardware->ledScreen.dataPins[3].pinNumber = "89";

    outputHardware->ledScreen.ePin.pin = "p8.31";
    outputHardware->ledScreen.ePin.gpioPin = "gpio10";
    outputHardware->ledScreen.ePin.pinNumber = "10";

    outputHardware->ledScreen.rsPin.pin = "p8.32";
    outputHardware->ledScreen.rsPin.gpioPin = "gpio11";
    outputHardware->ledScreen.rsPin.pinNumber = "11";

}

void initPlayer1HW(InputHardware* inputHardware) {
    inputHardware->buttonLed.pin = "p8.13";
    inputHardware->buttonLed.gpioPin = "gpio23";
    inputHardware->buttonLed.pinNumber = "23";

    inputHardware->joyStickLed.pin = "p8.46";
    inputHardware->joyStickLed.gpioPin = "gpio71";
    inputHardware->joyStickLed.pinNumber = "71";

    inputHardware->upButton.pin = "p8.7";
    inputHardware->upButton.gpioPin = "gpio66";
    inputHardware->upButton.pinNumber = "66";

    inputHardware->downButton.pin = "p8.9";
    inputHardware->downButton.gpioPin = "gpio69";
    inputHardware->downButton.pinNumber = "69";

    inputHardware->profileSwitchButton.pin = "p8.8";
    inputHardware->profileSwitchButton.gpioPin = "gpio67";
    inputHardware->profileSwitchButton.pinNumber = "67";

    inputHardware->joystick.xpin = "in_voltage2_raw";
    inputHardware->joystick.ypin = "in_voltage3_raw";
}

void initPlayer2HW(InputHardware* inputHardware) {
    inputHardware->buttonLed.pin = "p8.13";
    inputHardware->buttonLed.gpioPin = "gpio23";
    inputHardware->buttonLed.pinNumber = "23";

    inputHardware->joyStickLed.pin = "p8.46";
    inputHardware->joyStickLed.gpioPin = "gpio71";
    inputHardware->joyStickLed.pinNumber = "71";

    inputHardware->upButton.pin = "p8.7";
    inputHardware->upButton.gpioPin = "gpio66";
    inputHardware->upButton.pinNumber = "66";

    inputHardware->downButton.pin = "p8.9";
    inputHardware->downButton.gpioPin = "gpio69";
    inputHardware->downButton.pinNumber = "69";

    inputHardware->profileSwitchButton.pin = "p8.8";
    inputHardware->profileSwitchButton.gpioPin = "gpio67";
    inputHardware->profileSwitchButton.pinNumber = "67";

    inputHardware->joystick.xpin = "in_voltage2_raw";
    inputHardware->joystick.ypin = "in_voltage3_raw";
}

void initBoardHW() {
    char buffer[MAX_LENGTH]; 
    runCommand("config-pin p8.43 gpio");
    if (readFile(USER_BUTTON, "direction", buffer) == -1) {
        writeToFile(GPIO_FILE_DIR, "export", "72");
    }
    runCommand("echo in > /sys/class/gpio/gpio72/direction");
    runCommand("echo 1 > /sys/class/gpio/gpio72/active_low");
    runCommand("echo rising > /sys/class/gpio/gpio72/edge");
    runCommand("i2cset -y 1 0x70 0x21 0x00");
    runCommand("i2cset -y 1 0x70 0x81 0x00");
    runCommand("i2cset -y 1 0x71 0x21 0x00");
    runCommand("i2cset -y 1 0x71 0x81 0x00");
    runCommand("i2cset -y 1 0x72 0x21 0x00");
    runCommand("i2cset -y 1 0x72 0x81 0x00");
    runCommand("i2cset -y 1 0x73 0x21 0x00");
    runCommand("i2cset -y 1 0x73 0x81 0x00");
}

void cleanup(Player* player1, Player* player2, GameServer* server) {

}
