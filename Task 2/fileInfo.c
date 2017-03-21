#include <malloc.h>
#include <stdlib.h>
#import "fileInfo.h"


FileInfo *enterFileInfo(int argc, char *argv[]) {

    FileInfo *fileInfo = (FileInfo *) malloc(sizeof(FileInfo));
    fileInfo->filename = argv[3];
    fileInfo->size = atoi(argv[4]);
    fileInfo->quantity = atoi(argv[5]);

    return fileInfo;
}
