#include "a2d_utils.h"

int getRawReadings(char* fileDirectory, char* fileName) {
    char fullFileDir[MAX_LENGTH];
    sprintf(fullFileDir, "%s%s", fileDirectory, fileName);
    FILE *f = openFile(fullFileDir, "r");
    // Get reading
    int a2dReading = 0;
    int itemsRead = fscanf(f, "%d", &a2dReading);
    if (itemsRead <= 0) {
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }
    // Close file
    fclose(f);
    return a2dReading;
}

double getConvertedReadings(char* fileDirectory, char* fileName) {
    double result = (double) getRawReadings(fileDirectory, fileName);
    return (double) (A2D_VOLTAGE_REF * ((result) / (A2D_MAX_READING)));
}