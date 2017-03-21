#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include "inOutSystems.h"


void generate(FileInfo *fileInfo) {
    int randomData = open("/dev/urandom", O_RDONLY);
    int results = open(fileInfo->filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    char sign;
    srand(time(NULL));
    for (int i = 0; i < fileInfo->quantity; i++) {
        for (int j = 0; j < fileInfo->size; j++) {
            read(randomData, &sign, sizeof(sign));
            //printf("%c", (int) (sign));
            //write(results, &sign, sizeof(sign));
            sign = (char) (rand() % 25 + 97);
            printf("%c", (int) (sign));
            write(results, &sign, sizeof(sign));
        }
        write(results, "\n", sizeof(char));
        printf("\n");
    }
    close(results);
    close(randomData);
}

void shuffle(FileInfo *fileInfo) {
    // Reading all lines from file to lines[][]
    int source = open(fileInfo->filename, O_RDONLY);
    char lines[fileInfo->quantity][fileInfo->size];
    for (int i = 0; i < fileInfo->quantity; ++i) {
        for (int j = 0; j <= fileInfo->size; ++j) {
            read(source, &lines[i][j], sizeof(lines[i][j]));
        }
    }
    close(source);

    srand(time(NULL));
    for (int i = 0; i < fileInfo->quantity; ++i) {
        int j = rand() % fileInfo->quantity;
        // SWAP lines[i][] with lines[j][]
        char tmp;
        for (int k = 0; k < fileInfo->size; ++k) {
            tmp = lines[i][k];
            lines[i][k] = lines[j][k];
            lines[j][k] = tmp;
        }
    }
    // Wirting shuffled lines[][] to file
    source = open(fileInfo->filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    for (int i = 0; i < fileInfo->quantity; ++i) {
        for (int j = 0; j < fileInfo->size; ++j) {
            printf("%c", lines[i][j]);
            write(source, &lines[i][j], sizeof(lines[i][j]));
        }
        write(source, "\n", sizeof(char));
        printf("\n");
    }
    close(source);
}

