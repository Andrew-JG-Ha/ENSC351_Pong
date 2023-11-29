// analysis.h
// Module to analyze light level data in the background (thread).
// It calculates various statistics, bundles them for use in other modules
// and prints them to terminal.
#ifndef ANALYSIS_H
#define ANALYSIS_H

typedef struct {
 double minInterval;
 double maxInterval;
 double avgInterval;
 double minV;
 double maxV;
 double avgV;
 int numDips;
} dataPackage;

// Begin/end the background thread which analyzes data.
void startAnalyzing(void);
void stopAnalyzing(void);

dataPackage getData(void);

#endif