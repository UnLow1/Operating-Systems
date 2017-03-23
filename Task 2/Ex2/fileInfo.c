#include <stdlib.h>
#include "fileInfo.h"


FileInfo *enterFileInfo(char *argv[]) {

    FileInfo *fileInfo = (FileInfo *) malloc(sizeof(FileInfo));
    fileInfo->path = realpath(argv[1],fileInfo->path);
    fileInfo->size = atoi(argv[2]);
    return fileInfo;
}