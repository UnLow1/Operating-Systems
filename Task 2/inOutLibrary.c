#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "inOutLibrary.h"


void generateLib(FileInfo *fileInfo) {
    FILE *randomData = fopen("/dev/urandom", "r");
    FILE *results = fopen(fileInfo->filename, "w");
    char sign;
    srand(time(NULL));
    for (int i = 0; i < fileInfo->quantity; i++) {
        for (int j = 0; j < fileInfo->size; j++) {
            fread(&sign, sizeof(sign), 1, randomData);
            //write(results, &sign, sizeof(sign));
            sign = (char) (rand() % 25 + 97);
            fwrite(&sign, sizeof(sign), 1, results);
        }
        fwrite("\n", sizeof(char), 1, results);
    }
    fclose(results);
    fclose(randomData);
}

void shuffleLib(FileInfo *fileInfo) {
    FILE *source = fopen(fileInfo->filename, "r+");
    char lines[2][fileInfo->size + 1];
    srand(time(NULL));
    for (int i = 0; i < fileInfo->quantity; ++i) {
        fseek(source, i * sizeof(lines[0]), SEEK_SET);
        fread(lines[0], sizeof(lines[0]), 1, source);
        int random = rand() % fileInfo->quantity;
        fseek(source, random * sizeof(lines[1]), SEEK_SET);
        fread(lines[1], sizeof(lines[1]), 1, source);
        // SWAP LINES
        fseek(source, i * sizeof(lines[0]), SEEK_SET);
        fwrite(lines[1], sizeof(lines[1]), 1, source);
        fseek(source, random * sizeof(lines[1]), SEEK_SET);
        fwrite(lines[0], sizeof(lines[0]), 1, source);
    }
    fclose(source);
}

void sortLib(FileInfo *fileInfo) {
    FILE *source = fopen(fileInfo->filename, "r+");
    char lines[2][fileInfo->size + 1];
    for (int i = fileInfo->quantity - 1; i >= 0; i--) {
        for (int j = 0; j < i; j++) {
            fseek(source, j * sizeof(lines[0]), SEEK_SET);
            fread(lines[0], sizeof(lines[0]), 1, source);
            fread(lines[1], sizeof(lines[1]), 1, source);
            fseek(source, j * sizeof(lines[0]), SEEK_SET);
            if (lines[0][0] > lines[1][0]) {
                fwrite(lines[1], sizeof(lines[1]), 1, source);
                fwrite(lines[0], sizeof(lines[0]), 1, source);
            } else {
                fwrite(lines[0], sizeof(lines[0]), 1, source);
                fwrite(lines[1], sizeof(lines[1]), 1, source);
            }
        }
    }
    fclose(source);
}