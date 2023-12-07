/**
 * a2d_utils.h
 * This file contains all basic utility functions to interact with analog files to either obtain raw readings or normalized readings to 1.8V
*/

#ifndef A2D_H
#define A2D_H

#include "utils.h"

#define A2D_FILE_DIR "/sys/bus/iio/devices/iio:device0/"
#define ANALOG_1 "in_voltage1_raw"
#define ANALOG_2 "in_voltage2_raw"
#define ANALOG_3 "in_voltage3_raw"
#define ANALOG_4 "in_voltage4_raw"
#define ANALOG_5 "in_voltage5_raw"
#define ANALOG_6 "in_voltage6_raw"
#define ANALOG_7 "in_voltage7_raw"

#define A2D_VOLTAGE_REF 1.8
#define A2D_MAX_READING 4095

#define MAX_LENGTH 1024

int getRawReadings(char* fileDirectory, char* fileName);

double getConvertedReadings(char* fileDirectory, char* fileName);

#endif
