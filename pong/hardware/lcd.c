#include "lcd.h"
#include "../utility/utils.h"
#include <string.h>

#define MAX_LEN 1024

#define VALUE_FILE "value"

static void initPin(char* fileDir, char* pin, char* pinNumber);

static void enableWrite(LcdScreen screen);

static void write4BitsToLcd(LcdScreen* screen, __uint8_t value);

LcdScreen* generateLcd(LcdHardware lcdHardware) {
    LcdScreen* newLcdScreen = malloc(sizeof(LcdScreen));
    newLcdScreen->numDataPins = lcdHardware.numDataPins;
    newLcdScreen->dataPinsFilePaths = calloc(lcdHardware.numDataPins, sizeof(char*));
    newLcdScreen->ePinFilePath = calloc(1, sizeof(char) * MAX_LEN);
    newLcdScreen->rsPinFilePath = calloc(1, sizeof(char) * MAX_LEN);
    sprintf(newLcdScreen->ePinFilePath, "%s%s/", GPIO_FILE_DIR, lcdHardware.ePin.gpioPin);
    initPin(newLcdScreen->ePinFilePath, lcdHardware.ePin.pin, lcdHardware.ePin.pinNumber);
    
    for (int i = 0; i < lcdHardware.numDataPins; i++) {
        newLcdScreen->dataPinsFilePaths[i] = calloc(1, sizeof(char)*MAX_LEN);
        sprintf(newLcdScreen->dataPinsFilePaths[i], "%s%s/", GPIO_FILE_DIR, lcdHardware.dataPins[i].gpioPin);
        initPin(newLcdScreen->dataPinsFilePaths[i], lcdHardware.dataPins[i].pin, lcdHardware.dataPins[i].pinNumber);
    }

    sprintf(newLcdScreen->rsPinFilePath, "%s%s/", GPIO_FILE_DIR, lcdHardware.rsPin.gpioPin);
    initPin(newLcdScreen->rsPinFilePath, lcdHardware.rsPin.pin, lcdHardware.rsPin.pinNumber);
    
    /**
     * The following segment of code is adapted from 
     * https://opencoursehub.cs.sfu.ca/mba200/grav-cms/ensc351/links/files/2022-student-howtos/16x2CharacterLCDThroughGPIO.pdf
    */

    // Special Function Set 1.
	write4BitsToLcd(newLcdScreen, 0x03); // 0011
    sleepForNs(5000000); // 5 ms

	// Special Function Set 2.
	write4BitsToLcd(newLcdScreen, 0x03); // 0011
    sleepForNs(128000); // 128 us
    
	// Special Function Set 3.
	write4BitsToLcd(newLcdScreen, 0x03); // 0011
    sleepForNs(128000); // 128 us

	// Sets to 4-bit operation.
	write4BitsToLcd(newLcdScreen, 0x2); /* 0010 */
	sleepForNs(1000000); // 1 ms

	// Sets to 4-bit operation. Sets 2-line display. Selects 5x8 dot character font.
	write4BitsToLcd(newLcdScreen, 0x2); /* 0010 - We can alternatively write 0000 here for 8-bit operation. */
	write4BitsToLcd(newLcdScreen, 0x8); /* 1000 - We can alternatively write 1100 here for 5x10 dot font. */
    sleepForNs(128000); // 128 us

	// Display ON/OFF control.
	write4BitsToLcd(newLcdScreen, 0x0); /* 0000 */
	write4BitsToLcd(newLcdScreen, 0x8); /* 1000 */
	sleepForNs(128000); // 128 us

    // Clear the display.
	write4BitsToLcd(newLcdScreen, 0x0); /* 0000 */
	write4BitsToLcd(newLcdScreen, 0x1); /* 0001 */
	sleepForNs(64000); // 64 us

	// Sets mode to increment cursor position by 1 and shift right when writing to display.
	write4BitsToLcd(newLcdScreen, 0x0); /* 0000 */
	write4BitsToLcd(newLcdScreen, 0x6); /* 0110 */
	sleepForNs(128000); // 128 us

	// Turns on display. This corresponds to the instruction 0000 1100 in binary.
	// To be able to see the cursor, use 0000 1110.
	// To enable cursor blinking, use 0000 1111.
	write4BitsToLcd(newLcdScreen, 0x0); /* 0000 */
	write4BitsToLcd(newLcdScreen, 0xF); /* 1111 */
	sleepForNs(64000); // 64 us

    writeToFile(newLcdScreen->rsPinFilePath, VALUE_FILE, "1");

    return newLcdScreen;
}

void destroyLcd(LcdScreen* screen) {
    for (int i = 0; i < screen->numDataPins; i++) {
        free(screen->dataPinsFilePaths[i]);
        screen->dataPinsFilePaths[i] = NULL;
    }
    free(screen->dataPinsFilePaths);
    screen->dataPinsFilePaths = NULL;
    free(screen->ePinFilePath);
    screen->ePinFilePath = NULL;
    free(screen->rsPinFilePath);
    screen->rsPinFilePath = NULL;
    free(screen);
    screen = NULL;
}

void writeMessageToLcd(LcdScreen* screen, char* text) {
    printf("Writing \"%s\" to LCD...\n", text);
    for (int i = 0; i < strlen(text); i++) {
        writeCharToLcd(screen, text[i]);
    }
}

void writeCharToLcd(LcdScreen* screen, char character) {
    unsigned int upperBits = (character >> 4);
    unsigned int lowerBits = (character & 0xF);
    write4BitsToLcd(screen, upperBits);
    write4BitsToLcd(screen, lowerBits);
}

static void write4BitsToLcd(LcdScreen* screen, __uint8_t value) {
    char bits[2];
    bits[1] = '\0';
    for (int i = 0; i < screen->numDataPins; i++) {
        bits[0] = ((value >> i) & 0x01 ? 1 : 0) + '0';
        writeToFile(screen->dataPinsFilePaths[i], VALUE_FILE, bits);
    }
    enableWrite(*screen);
}

static void initPin(char* fileDir, char* pin, char* pinNumber) {
    char buffer[MAX_LEN];
    char configPinCmd[MAX_LEN];
    printf("fileDIr is: %s\n\n", fileDir);

    sprintf(configPinCmd, "config-pin %s gpio", pin); 
    runCommand(configPinCmd);
    if (readFile(fileDir, "direction", buffer) == -1) {
        writeToFile(GPIO_FILE_DIR, "export", pinNumber);
    }
    writeToFile(fileDir, "direction", "out");
    writeToFile(fileDir, "active_low", "0");
    writeToFile(fileDir, VALUE_FILE, "0");
}

static void enableWrite(LcdScreen screen) {
    writeToFile(screen.ePinFilePath, VALUE_FILE, "1");
    sleepForMs(10);
    writeToFile(screen.ePinFilePath, VALUE_FILE, "0");
    sleepForMs(10);
}