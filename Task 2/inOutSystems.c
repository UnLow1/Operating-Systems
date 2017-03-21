#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "inOutSystems.h"


void generateSys(FileInfo *fileInfo) {
    int randomData = open("/dev/urandom", O_RDONLY);
    int results = open(fileInfo->filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    char sign;
    srand(time(NULL));
    for (int i = 0; i < fileInfo->quantity; i++) {
        for (int j = 0; j < fileInfo->size; j++) {
            read(randomData, &sign, sizeof(sign));
            //write(results, &sign, sizeof(sign));
            sign = (char) (rand() % 25 + 97);
            write(results, &sign, sizeof(sign));
        }
        write(results, "\n", sizeof(char));
    }
    close(results);
    close(randomData);
}

void shuffleSys(FileInfo *fileInfo) {
    int source = open(fileInfo->filename, O_RDWR);
    char lines[2][fileInfo->size + 1];
    srand(time(NULL));
    for (int i = 0; i < fileInfo->quantity; ++i) {
        lseek(source, i * sizeof(lines[0]), SEEK_SET);
        read(source, lines[0], sizeof(lines[0]));
        int random = rand() % fileInfo->quantity;
        lseek(source, random * sizeof(lines[1]), SEEK_SET);
        read(source, lines[1], sizeof(lines[1]));
        // SWAP LINES
        lseek(source, i * sizeof(lines[0]), SEEK_SET);
        write(source, lines[1], sizeof(lines[1]));
        lseek(source, random * sizeof(lines[1]), SEEK_SET);
        write(source, lines[0], sizeof(lines[0]));
    }
    close(source);
}

void sortSys(FileInfo *fileInfo) {
    int source = open(fileInfo->filename, O_RDWR);
    char lines[2][fileInfo->size + 1];
    for (int i = fileInfo->quantity - 1; i >= 0; i--) {
        for (int j = 0; j < i; j++) {
            lseek(source, j * sizeof(lines[0]), SEEK_SET);
            read(source, lines[0], sizeof(lines[0]));
            read(source, lines[1], sizeof(lines[1]));
            lseek(source, j * sizeof(lines[0]), SEEK_SET);
            if (lines[0][0] > lines[1][0]) {
                write(source, lines[1], sizeof(lines[1]));
                write(source, lines[0], sizeof(lines[0]));
            } else {
                write(source, lines[0], sizeof(lines[0]));
                write(source, lines[1], sizeof(lines[1]));
            }
        }
    }
    close(source);
}