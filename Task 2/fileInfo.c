#include <malloc.h>
#include <stdlib.h>
#include "fileInfo.h"


FileInfo *enterFileInfo(char *argv[]) {

    FileInfo *fileInfo = (FileInfo *) malloc(sizeof(FileInfo));
    fileInfo->mode = argv[1];
    fileInfo->operation = argv[2];
    fileInfo->filename = argv[3];
    fileInfo->quantity = atoi(argv[4]);
    fileInfo->size = atoi(argv[5]);

    return fileInfo;
}
