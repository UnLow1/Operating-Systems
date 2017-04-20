#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <complex.h>
#include <unistd.h>
#include <sys/stat.h>


int countIters(double re, double im, int K);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("Wrong arguments! 3 args needed. Path, N, K");
        exit(-1);
    }

    int N = atoi(argv[2]);
    int K = atoi(argv[3]);
    FILE *pipe = fopen(argv[1],"w");
    srand(time(NULL));

    // index in points[] meaning: 0 - re, 1 - im, 2 - iters
    double points[3];
    int i;
    for (i = 0; i < N; i++) {
        points[0] = -2 + (rand() / (RAND_MAX / 3.0));  // -2 < re < 1
        points[1] = -1 + (rand() / (RAND_MAX / 2.0));  // -1 < im < 1
        points[2] = countIters(points[0], points[1], K);

        fprintf(pipe, "%.3f %.3f %d\n", points[0], points[1], (int) points[2]);
    }
    fclose(pipe);

    return 0;
}

int countIters(double re, double im, int K) {
    int i;
    double complex z = 0.0;

    for (i = 0; i <= K; i++) {
        z = cpow(z, 2.0) + re + im * I;
        if (cabs(z) > 2) {
            break;
        };
    }
    if (i <= 0)
        return 0;
    return i - 1;
}
