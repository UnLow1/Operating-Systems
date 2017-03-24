#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Wrong arguments\n");
        return 1;
    }

    int file;

    //ERROR WHILE OPENING
    if ((file = open(argv[1], O_RDWR)) == -1) {
        perror("open");
        exit(1);
    }

    char option[255];
    int bit_number;
    struct flock fl;
    struct stat *fileStat = malloc(sizeof(struct stat));
    stat(argv[1], fileStat);

    // INFO
    printf("Use commends like:\n"
                   "read_lock1, read_lock 2, write_lock1, write_lock2, info, unlock, read_char, write_char, quit\n");

    scanf("%s", option);
    while (strcmp(option, "quit") != 0) {
        // READ LOCK WITH NO BLOCK
        if (strcmp(option, "read_lock1") == 0) {
            scanf("%d", &bit_number);
            fl.l_type = F_RDLCK;
            fl.l_whence = SEEK_SET;
            fl.l_start = bit_number;
            fl.l_len = 1;

            //ERROR
            if (fcntl(file, F_SETLK, &fl) == -1) {
                perror("fcntl read_lock");
                exit(1);
            }

            printf("read_lock done!\n");
        }
            // READ LOCK WITH WAITING
        else if (strcmp(option, "read_lock2") == 0) {
            scanf("%d", &bit_number);
            fl.l_type = F_RDLCK;
            fl.l_whence = SEEK_SET;
            fl.l_start = bit_number;
            fl.l_len = 1;

            //ERROR
            if (fcntl(file, F_SETLKW, &fl) == -1) {
                perror("fcntl read_lock");
                exit(1);
            }

            printf("read_lock done!\n");
        }

            // WRITE LOCK WITH NO BLOCK
        else if (strcmp(option, "write_lock1") == 0) {
            scanf("%d", &bit_number);
            fl.l_type = F_WRLCK;
            fl.l_whence = SEEK_SET;
            fl.l_start = bit_number;
            fl.l_len = 1;

            //ERROR
            if (fcntl(file, F_SETLK, &fl) == -1) {
                perror("fcntl write_lock");
                exit(1);
            }

            printf("write_lock done!\n");
        }
            // WRITE LOCK WITH WAITING
        else if (strcmp(option, "write_lock2") == 0) {
            scanf("%d", &bit_number);
            fl.l_type = F_WRLCK;
            fl.l_whence = SEEK_SET;
            fl.l_start = bit_number;
            fl.l_len = 1;

            //ERROR
            if (fcntl(file, F_SETLKW, &fl) == -1) {
                perror("fcntl write_lock");
                exit(1);
            }

            printf("write_lock done!\n");
        }
            // INFO
        else if (strcmp(option, "info") == 0) {
            int i;
            for (i = 0; i < fileStat->st_size; i++) {
                fl.l_type = F_WRLCK;
                fl.l_whence = SEEK_SET;
                fl.l_start = i;
                fl.l_len = 1;

                fcntl(file, F_GETLK, &fl);

                if (fl.l_type == F_RDLCK) {
                    printf("byte: %d, lock: F_RDLCK, pid: %d\n", i, fl.l_pid);
                } else if (fl.l_type == F_WRLCK) {
                    printf("byte: %d, lock: F_WRLCK, pid: %d\n", i, fl.l_pid);
                }
            }
            printf("End of info statement\n");
        }
            // UNLOCK
        else if (strcmp(option, "unlock") == 0) {
            scanf("%d", &bit_number);
            fl.l_type = F_UNLCK;
            fl.l_whence = SEEK_SET;
            fl.l_start = bit_number;
            fl.l_len = 1;

            if (fcntl(file, F_SETLK, &fl) == -1) {
                perror("fcntl unlock");
                exit(1);
            }

            printf("unlock done!\n");
        }
            // READ CHAR
        else if (strcmp(option, "read_char") == 0) {
            scanf("%d", &bit_number);
            fl.l_type = F_WRLCK;
            fl.l_whence = SEEK_SET;
            fl.l_start = bit_number;
            fl.l_len = 1;

            fcntl(file, F_GETLK, &fl);

            if (fl.l_type == F_WRLCK) {
                perror("read_char - locked for writting");
                exit(1);
            } else {
                // SET READ LOCK
                fl.l_type = F_RDLCK;
                fl.l_whence = SEEK_SET;
                fl.l_start = bit_number;
                fl.l_len = 1;

                fcntl(file, F_SETLK, &fl);

                char result;
                read(file, &result, sizeof(result));
                printf("Found char: %c\n", result);
            }
        }
            // WRITE CHAR
        else if (strcmp(option, "write_char") == 0) {
            scanf("%d", &bit_number);
            fl.l_type = F_WRLCK;
            fl.l_whence = SEEK_SET;
            fl.l_start = bit_number;
            fl.l_len = 1;

            fcntl(file, F_GETLK, &fl);

            if (fl.l_type != F_UNLCK) {
                perror("write_char - locked for writting or reading");
                exit(1);
            }
            char sign;
            printf("Type char you want to write\n");
            scanf(" %c", &sign);
            lseek(file, sign, SEEK_SET);
            write(file, &sign, sizeof(sign));
            printf("Wrote char %c in position %d\n", sign, bit_number);
        }
            // BAD OPTION
        else {
            printf("Bad option, try again\n");
        }
        // INPUT ANOTHER COMMAND
        scanf("%s", option);
    }


    return 0;
}