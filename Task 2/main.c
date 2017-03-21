// compilation arguments: "./program sys shuffle datafile.txt 100 512"

#include <stdio.h>
#include "fileInfo.h"
#include "inOutSystems.h"

int main(int argc, char *argv[]) {

    FileInfo *fileInfo = enterFileInfo(argc, argv);

    printf("size = %d\nquantity = %d\nname = %s\n", fileInfo->size, fileInfo->quantity, fileInfo->filename);
    printf("\nGenerating random file\n");
    generateSyS(fileInfo);
    printf("\nSorting file\n");
    sortSys(fileInfo);
    printf("\nShuffling file\n");
    shuffleSys(fileInfo);

    return 0;
}

