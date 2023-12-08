#include "playerClient.h"
#include "gameServer.h"

#define USER_BUTTON "/sys/class/gpio/gpio72/"
#define MAX_LENGTH 1024

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
    gameServer = generateGameServer(player1, player2, outHW);

    // Start threads
    runPlayerClient(player1);
    runPlayerClient(player2);    

    // waitForEdge();

    // Cleanup
    cleanup(player1, player2, gameServer);
}

void initGameServerHW(OutputHardware* outputHardware) {
    outputHardware->lcdScreen.dataPins[0].pin = "p8.27";
    outputHardware->lcdScreen.dataPins[0].gpioPin = "gpio86";
    outputHardware->lcdScreen.dataPins[0].pinNumber = "86";

    outputHardware->lcdScreen.dataPins[1].pin = "p8.28";
    outputHardware->lcdScreen.dataPins[1].gpioPin = "gpio88";
    outputHardware->lcdScreen.dataPins[1].pinNumber = "88";

    outputHardware->lcdScreen.dataPins[2].pin = "p8.29";
    outputHardware->lcdScreen.dataPins[2].gpioPin = "gpio87";
    outputHardware->lcdScreen.dataPins[2].pinNumber = "87";

    outputHardware->lcdScreen.dataPins[3].pin = "p8.30";
    outputHardware->lcdScreen.dataPins[3].gpioPin = "gpio89";
    outputHardware->lcdScreen.dataPins[3].pinNumber = "89";

    outputHardware->lcdScreen.ePin.pin = "p8.31";
    outputHardware->lcdScreen.ePin.gpioPin = "gpio10";
    outputHardware->lcdScreen.ePin.pinNumber = "10";

    outputHardware->lcdScreen.rsPin.pin = "p8.32";
    outputHardware->lcdScreen.rsPin.gpioPin = "gpio11";
    outputHardware->lcdScreen.rsPin.pinNumber = "11";

    outputHardware->matrix.Matrix1 = 0x70;
    outputHardware->matrix.Matrix2 = 0x72;
    outputHardware->matrix.Matrix3 = 0x71;
    outputHardware->matrix.Matrix4 = 0x73;
    
}

void initPlayer1HW(InputHardware* inputHardware) {
    inputHardware->buttonLed.pin = "p8.40";
    inputHardware->buttonLed.gpioPin = "gpio77";
    inputHardware->buttonLed.pinNumber = "77";

    inputHardware->joyStickLed.pin = "p8.39";
    inputHardware->joyStickLed.gpioPin = "gpio76";
    inputHardware->joyStickLed.pinNumber = "76";

    inputHardware->upButton.pin = "p8.33";
    inputHardware->upButton.gpioPin = "gpio9";
    inputHardware->upButton.pinNumber = "9";

    inputHardware->downButton.pin = "p8.34";
    inputHardware->downButton.gpioPin = "gpio81";
    inputHardware->downButton.pinNumber = "81";

    inputHardware->profileSwitchButton.pin = "p8.36";
    inputHardware->profileSwitchButton.gpioPin = "gpio80";
    inputHardware->profileSwitchButton.pinNumber = "80";

    inputHardware->joystick.xpin = "in_voltage2_raw";
    inputHardware->joystick.ypin = "in_voltage3_raw";
}

void initPlayer2HW(InputHardware* inputHardware) {
    inputHardware->buttonLed.pin = "p8.8";
    inputHardware->buttonLed.gpioPin = "gpio67";
    inputHardware->buttonLed.pinNumber = "67";

    inputHardware->joyStickLed.pin = "p8.7";
    inputHardware->joyStickLed.gpioPin = "gpio66";
    inputHardware->joyStickLed.pinNumber = "66";

    inputHardware->upButton.pin = "p8.12";
    inputHardware->upButton.gpioPin = "gpio44";
    inputHardware->upButton.pinNumber = "44";

    inputHardware->downButton.pin = "p8.13";
    inputHardware->downButton.gpioPin = "gpio23";
    inputHardware->downButton.pinNumber = "23";

    inputHardware->profileSwitchButton.pin = "p8.11";
    inputHardware->profileSwitchButton.gpioPin = "gpio45";
    inputHardware->profileSwitchButton.pinNumber = "45";

    inputHardware->joystick.xpin = "in_voltage4_raw";
    inputHardware->joystick.ypin = "in_voltage5_raw";
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
    destroyGameServer(server);
    destroyPlayer(player1);
    destroyPlayer(player2);
}
