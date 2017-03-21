#ifndef TASK2_FILEINFO_H
#define TASK2_FILEINFO_H

typedef struct FileInfo FileInfo;

struct FileInfo {
    char *filename;
    int size;
    int quantity;
};

FileInfo *enterFileInfo(int argc, char *argv[]);


#endif //TASK2_FILEINFO_H
