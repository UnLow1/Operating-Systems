#ifndef EX2_FILEINFO_H
#define EX2_FILEINFO_H

typedef struct FileInfo FileInfo;

struct FileInfo {
    char *path;
    int size;
};

FileInfo *enterFileInfo(char *argv[]);

#endif //EX2_FILEINFO_H
