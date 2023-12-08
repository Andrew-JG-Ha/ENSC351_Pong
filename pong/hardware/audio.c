#include "audio.h"
#include "../utility/utils.h"
#include <dirent.h>
#include <sys/types.h>

#define PLAYAUDIO "aplay audioFiles/message.wav"
#define MAX_LENGTH 1024
#define NUM_PING_FILES 33
#define PING_FILES "audioFiles/pings"

//Speak function that takes in string
void speak(char* msg) {
    char command [MAX_LENGTH];
    sprintf(command, "%s '%s'", "pico2wave -w audioFiles/message.wav", msg);
    runCommand(command);
    runCommand(PLAYAUDIO);
}

void ping(void) {
    char command [MAX_LENGTH];
    srand(getTimeInNanoS());
    int randIndex = (long long int) (rand()%NUM_PING_FILES);

    DIR* dir = opendir(PING_FILES);
    struct dirent *entry;

    if (dir == NULL) {
        perror("Failed to open file directory.\n");
        exit(EXIT_FAILURE);
    }

    while (randIndex >= 0) {
        entry = readdir(dir);
        if (entry == NULL) {
            break;
        }
        if (entry->d_type == 8) {
            if (randIndex == 0) {
                sprintf(command, "aplay audioFiles/pings/%s", entry->d_name);
                runCommand(command);
                break;
            }
            randIndex--;
        }
    }
    closedir(dir);
    dir = NULL;
}

void gameEnd(void) {
    char command[MAX_LENGTH];
    sprintf(command, "aplay audioFiles/%s", "gameEnd.wav");
    runCommand(command);
}

void bootUp(void) {
    char command[MAX_LENGTH];
    sprintf(command, "aplay audioFiles/%s", "gameStart.wav");
    runCommand(command);
}