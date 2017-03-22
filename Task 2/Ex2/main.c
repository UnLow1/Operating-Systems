#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <malloc.h>
#include <time.h>
#include <string.h>
#include "fileInfo.h"

void printFileInfo(char *path, const struct stat *fileStat) {

    printf("\nFile path: %s\n", path);
    printf("File size: %zd bytes\n",fileStat->st_size);
    printf("File permissions: ");
    printf( (fileStat->st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat->st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat->st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXOTH) ? "x" : "-");

    time_t t = fileStat->st_mtime;
    struct tm lt;
    localtime_r(&t, &lt);
    char timbuf[80];
    strftime(timbuf, sizeof(timbuf), "%c", &lt);

    printf("\nFile last modified: %s",timbuf);


    printf("\n\n");
}

void checkFile(char *path, int max_size) {

    DIR *dir = opendir(path);
    struct dirent *file;
    struct stat *fileStat = malloc(sizeof(struct stat));
    char actualPath[250];

    while ((file = readdir(dir)) != NULL) {
        strcpy(actualPath, path);

        if (file->d_name[0] != '.') {

            strcat(actualPath,"/");
            strcat(actualPath,file->d_name);

            stat(actualPath,fileStat);

            if (fileStat->st_size <= max_size && S_ISREG((fileStat->st_mode))){
                // FILE HERE, PRINT INFO
                printFileInfo(actualPath, fileStat);
            }

            else if (S_ISDIR(fileStat->st_mode)) {
                // DIRECTORY HERE, MOVE DEEPER
                checkFile(actualPath,max_size);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("ERROR!!!! BAD ARGUMENTS!!!\n");
        return 0;
    }

    FileInfo *fileInfo = enterFileInfo(argv);
    checkFile(fileInfo->path, fileInfo->size);

    return 0;
}
