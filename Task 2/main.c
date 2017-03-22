// compilation arguments: "./program sys shuffle datafile.txt 100 512"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include "fileInfo.h"
#include "inOutSystems.h"
#include "inOutLibrary.h"

void timeCheckpoint(const char *message) {
    // Getting time values
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    struct timeval user_time = usage.ru_utime;
    struct timeval system_time = usage.ru_stime;
    // Show info
    printf("user time: %ld | system time: %ld  ---  %s\n",
           user_time.tv_sec * 1000000 + user_time.tv_usec,
           system_time.tv_sec * 1000000 + system_time.tv_usec,
           message);
}

int main(int argc, char *argv[]) {

    FileInfo *fileInfo = enterFileInfo(argv);
    
    timeCheckpoint("Starting program\n");


    if (strcmp(fileInfo->mode, "sys") == 0) {

        printf("USING SYSTEM FUNCTIONS\n");

        if (strcmp(fileInfo->operation, "generate") == 0) {
            generateSys(fileInfo);
            timeCheckpoint("Generating file\n");
        } else if (strcmp(fileInfo->operation, "shuffle") == 0) {
            shuffleSys(fileInfo);
            timeCheckpoint("Shuffling file\n");
        } else if (strcmp(fileInfo->operation, "sort") == 0) {
            sortSys(fileInfo);
            timeCheckpoint("Sorting file\n");
        }
    } else if (strcmp(fileInfo->mode, "lib") == 0) {

        printf("USING LIBRARY FUNCTIONS\n");

        if (strcmp(fileInfo->operation, "generate") == 0) {
            generateLib(fileInfo);
            timeCheckpoint("Generating file\n");
        } else if (strcmp(fileInfo->operation, "shuffle") == 0) {
            shuffleLib(fileInfo);
            timeCheckpoint("Shuffling file\n");
        } else if (strcmp(fileInfo->operation, "sort") == 0) {
            sortLib(fileInfo);
            timeCheckpoint("Sorting file\n");
        }
    }

    return 0;
}

