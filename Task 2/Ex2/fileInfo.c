#include <stdlib.h>
#include "fileInfo.h"


FileInfo *enterFileInfo(char *argv[]) {

    FileInfo *fileInfo = (FileInfo *) malloc(sizeof(FileInfo));
    fileInfo->path = argv[1];
    fileInfo->size = atoi(argv[2]);
    return fileInfo;
}