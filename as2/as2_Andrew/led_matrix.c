#include "led_matrix.h"

#define I2C_DEVICE_ADDRESS 0x70
#define turnOnAddr 0x81
#define turnOffAddr 0x80
#define turnOnNBlinkAddr 0x83

#define DECIMAL 10
#define matrixSize 8
#define decimalPoint 0x08
#define encodingBitmask 0x80

static int matrixOn(char* bus);

static int matrixBlink(char* bus);

static void matrixOff(char* bus);

static void mapInt(LedMatrix* object, int value);

static void mapDouble(LedMatrix* object, double value);

static void writeToLedMatrix(LedMatrix* object);

static void digitMap(int value, unsigned char* returnBuffer);

LedMatrix* LedMatrix_new(char* bus, int config) {
    LedMatrix* newObject = malloc(sizeof(LedMatrix));
    newObject->matrixEncodings = calloc(sizeof(unsigned char), matrixSize);
    memset(newObject->matrixEncodings, 0, matrixSize);
    newObject->bus = bus;
    newObject->fileDesc = -1;
    updateConfig(newObject, config);
    return newObject;
}

void LedMatrix_destroy(LedMatrix* object) {
    matrixOff(object->bus);
    free(object->matrixEncodings);
    free(object);
    object = NULL;
}

void writeD(LedMatrix* object, double value) { 
    mapDouble(object, value);
    writeToLedMatrix(object);
}

void writeI(LedMatrix* object, int value) {
    mapInt(object, value);
    writeToLedMatrix(object);
}

void writeE(LedMatrix* object, unsigned char* encoding) {
    for (int i = 0; i < matrixSize; i++) {
        object->matrixEncodings[i] = encoding[i];
    }
    writeToLedMatrix(object);
}

void updateConfig(LedMatrix* object, int newConfig) {
    switch (newConfig) {
    case 1:
        object->fileDesc = matrixOn(object->bus);
        break;
    case 2:
        object->fileDesc = matrixBlink(object->bus);
        break;
    default:
        object->fileDesc = -1;
        matrixOff(object->bus);
        break;
    }
}

static int matrixOn(char* bus) {
    configPins(bus);
    int i2cFileDesc = initBus(bus, I2C_DEVICE_ADDRESS);
    writeToRegI2C_single(i2cFileDesc, turnOnAddr, 0x00);
    runCommand("i2cset -y 1 0x70 0x81 0x01");
    return i2cFileDesc;
}

static int matrixBlink(char* bus) {
    configPins(bus);
    int i2cFileDesc = initBus(bus, I2C_DEVICE_ADDRESS);
    writeToRegI2C_single(i2cFileDesc, turnOnNBlinkAddr, 0x00);
    runCommand("i2cset -y 1 0x70 0x83 0x01");
    return i2cFileDesc;
}

static void matrixOff(char* bus) {
    configPins(bus);
    int i2cFileDesc = initBus(bus, I2C_DEVICE_ADDRESS);
    writeToRegI2C_single(i2cFileDesc, turnOffAddr, 0x00);
}

static void mapInt(LedMatrix* object, int value) {
    int digit2 = (int) value / DECIMAL;
    digit2 = digit2 % DECIMAL;
    int digit1 = (int) value % DECIMAL;
    unsigned char digit2_encoding[matrixSize];
    unsigned char digit1_encoding[matrixSize];

    digitMap(digit2, digit2_encoding);
    digitMap(digit1, digit1_encoding);
    for (int i = 0; i < matrixSize; i++) {
        if (digit2 > 0) {
            int temp = encodingBitmask & digit2_encoding[i];
            object->matrixEncodings[i] = ((digit2_encoding[i] & ~encodingBitmask) << 5) | temp >> 3 | digit1_encoding[i];
        }
        else {
            object->matrixEncodings[i] = digit1_encoding[i];
        }
    }
}

static void mapDouble(LedMatrix* object, double value) {
    int temp = value * DECIMAL;
    int digit2 = (int) temp / DECIMAL;
    int digit1 = (int) temp % DECIMAL;
    digit2 = digit2 % DECIMAL;
    unsigned char digit2_encoding[matrixSize];
    unsigned char digit1_encoding[matrixSize];
    digitMap(digit2, digit2_encoding);
    digitMap(digit1, digit1_encoding);
    for (int i = 0; i < matrixSize; i++) {
        int temp = encodingBitmask & digit2_encoding[i]; 
        object->matrixEncodings[i] = ((digit2_encoding[i] & ~encodingBitmask) << 5) | temp >> 3 | digit1_encoding[i];
    }
    object->matrixEncodings[0] = object->matrixEncodings[0] | decimalPoint;
}

static void writeToLedMatrix(LedMatrix* object) {
    if (object->fileDesc == -1) {
        printf("Error: Invalid file descriptor (%d) for bus (%s)\n", object->fileDesc, object->bus);
        return;
    }
    for (int i = 0; i < matrixSize; i++) {
        writeToRegI2C_single(object->fileDesc, (unsigned char) 2*i, object->matrixEncodings[i]);
    }
}

static void digitMap(int value, unsigned char* returnBuffer) {
    switch (value) {
    case 0: // I2C seems to be shifted over 1 
        returnBuffer[7] = 0x83; // 1000 0011
        returnBuffer[6] = 0x82; // 1000 0010
        returnBuffer[5] = 0x82; // 1000 0010
        returnBuffer[4] = 0x82; // 1000 0010
        returnBuffer[3] = 0x82; // 1000 0010
        returnBuffer[2] = 0x82; // 1000 0010
        returnBuffer[1] = 0x83; // 1000 0011
        returnBuffer[0] = 0x00; // 0000 0000
        break;
    case 1:
        returnBuffer[7] = 0x01; // 0000 0001
        returnBuffer[6] = 0x03; // 0000 0011
        returnBuffer[5] = 0x01; // 0000 0001
        returnBuffer[4] = 0x01; // 0000 0001
        returnBuffer[3] = 0x01; // 0000 0001
        returnBuffer[2] = 0x01; // 0000 0001
        returnBuffer[1] = 0x83; // 1000 0011
        returnBuffer[0] = 0x00; // 0000 0000
        break;
    case 2:
        returnBuffer[7] = 0x83;
        returnBuffer[6] = 0x80;
        returnBuffer[5] = 0x80;
        returnBuffer[4] = 0x83;
        returnBuffer[3] = 0x02;
        returnBuffer[2] = 0x02;
        returnBuffer[1] = 0x83;
        returnBuffer[0] = 0x00; 
        break;
    case 3:
        returnBuffer[7] = 0x83;
        returnBuffer[6] = 0x80;
        returnBuffer[5] = 0x80;
        returnBuffer[4] = 0x83;
        returnBuffer[3] = 0x80;
        returnBuffer[2] = 0x80;
        returnBuffer[1] = 0x83;
        returnBuffer[0] = 0x00; 
        break;
    case 4:
        returnBuffer[7] = 0x82;
        returnBuffer[6] = 0x82;
        returnBuffer[5] = 0x82;
        returnBuffer[4] = 0x83;
        returnBuffer[3] = 0x80;
        returnBuffer[2] = 0x80;
        returnBuffer[1] = 0x80;
        returnBuffer[0] = 0x00; 
        break;
    case 5:
        returnBuffer[7] = 0x83;
        returnBuffer[6] = 0x02;
        returnBuffer[5] = 0x02;
        returnBuffer[4] = 0x83;
        returnBuffer[3] = 0x80;
        returnBuffer[2] = 0x80;
        returnBuffer[1] = 0x83;
        returnBuffer[0] = 0x00; 
        break;
    case 6:
        returnBuffer[7] = 0x83;
        returnBuffer[6] = 0x02;
        returnBuffer[5] = 0x02;
        returnBuffer[4] = 0x83;
        returnBuffer[3] = 0x82;
        returnBuffer[2] = 0x82;
        returnBuffer[1] = 0x83;
        returnBuffer[0] = 0x00; 
        break;
    case 7:
        returnBuffer[7] = 0x83;
        returnBuffer[6] = 0x80;
        returnBuffer[5] = 0x80;
        returnBuffer[4] = 0x80;
        returnBuffer[3] = 0x80;
        returnBuffer[2] = 0x80;
        returnBuffer[1] = 0x80;
        returnBuffer[0] = 0x00; 
        break;
    case 8:
        returnBuffer[7] = 0x83;
        returnBuffer[6] = 0x82;
        returnBuffer[5] = 0x82;
        returnBuffer[4] = 0x83;
        returnBuffer[3] = 0x82;
        returnBuffer[2] = 0x82;
        returnBuffer[1] = 0x83;
        returnBuffer[0] = 0x00; 
        break;
    case 9:
        returnBuffer[7] = 0x83;
        returnBuffer[6] = 0x82;
        returnBuffer[5] = 0x82;
        returnBuffer[4] = 0x83;
        returnBuffer[3] = 0x80;
        returnBuffer[2] = 0x80;
        returnBuffer[1] = 0x83;
        returnBuffer[0] = 0x00; 
        break;
    default:
        returnBuffer[7] = 0x00;
        returnBuffer[6] = 0x00;
        returnBuffer[5] = 0x00;
        returnBuffer[4] = 0x00;
        returnBuffer[3] = 0x00;
        returnBuffer[2] = 0x00;
        returnBuffer[1] = 0x00;
        returnBuffer[0] = 0x00; 
    break;
    }

}