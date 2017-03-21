// compilation arguments: "./program sys shuffle datafile.txt 100 512"

#include <stdio.h>
#include "fileInfo.h"
#include "inOutSystems.h"

int main(int argc, char *argv[]) {

    FileInfo *fileInfo = enterFileInfo(argc, argv);

    printf("size = %d\nquantity = %d\n%s\n\n", fileInfo->size, fileInfo->quantity, fileInfo->filename);
    printf("Generating random file\n\n");
    generate(fileInfo);
    printf("\nShuffling file\n\n");
    shuffle(fileInfo);


    return 0;
}

