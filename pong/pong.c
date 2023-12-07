#include "playerClient.h"
#include "gameServer.h"


void configureLCDHW () {
    LcdHardware LH;

    Pin dataPins[4];
    Pin ePin;
    Pin rsPin;

    dataPins[0].pin = "p8.27";
    dataPins[0].gpioPin = "gpio86";
    dataPins[0].pinNumber = "86";

    dataPins[1].pin = "p8.28";
    dataPins[1].gpioPin = "gpio88";
    dataPins[1].pinNumber = "88";

    dataPins[2].pin = "p8.29";
    dataPins[2].gpioPin = "gpio87";
    dataPins[2].pinNumber = "87";

    dataPins[3].pin = "p8.30";
    dataPins[3].gpioPin = "gpio89";
    dataPins[3].pinNumber = "89";

    LH.dataPins = dataPins;

    ePin.pin = "p8.31";
    ePin.gpioPin = "gpio10";
    ePin.pinNumber = "10";

    rsPin.pin = "p8.32";
    rsPin.gpioPin = "gpio11";
    rsPin.pinNumber = "11";

    LH.ePin = ePin;
    LH.rsPin = rsPin;

    LcdScreen* newLcd = generateLcd(LH, 4);
    writeMessageToLcd(newLcd, "hello world!");
    sleepForMs(10000);
    destroyLcd(newLcd);
}



int main () {

}