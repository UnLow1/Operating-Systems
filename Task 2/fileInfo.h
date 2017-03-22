#ifndef TASK2_FILEINFO_H
#define TASK2_FILEINFO_H

typedef struct FileInfo FileInfo;

typedef enum mode mode;

struct FileInfo {
    char *filename;
    int size;
    int quantity;
    char *mode;
    char *operation;
};

FileInfo *enterFileInfo(char *argv[]);


#endif //TASK2_FILEINFO_H
