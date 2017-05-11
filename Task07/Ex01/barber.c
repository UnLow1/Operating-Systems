#include <cstdio>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Wrong arguments! 1 arg is needed: N");
        exit(-1);
    }

    int N = atoi(argv[1]);



    return 1;
}
