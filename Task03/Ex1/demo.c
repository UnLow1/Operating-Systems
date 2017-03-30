#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Wrong arguments! The only argument should be name of the filen");
        exit(-1);
    }
    if (getenv(argv[1]) == NULL) {
        perror("No Environmental Variable with that name actually");
        exit(-1);
    }
    printf("getenv(%s) = %s\n", argv[1], getenv(argv[1]));
    return 1;
}
