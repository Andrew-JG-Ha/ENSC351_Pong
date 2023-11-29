#include <stdlib.h>
#include "utility.h"
#include "sensor.h"

static double readRaw(char* filepath) {
    char output[10];
    char *eptr;
    double ADCValue;
    readFromFile(filepath, output, 10);
    ADCValue = strtod(output, &eptr);
    return ADCValue;
}

static double convertToVoltage(double raw) {
    return raw*1.8/4095;
}

double readVoltage(char* filepath) {
    double raw = readRaw(filepath);
    return convertToVoltage(raw);
}