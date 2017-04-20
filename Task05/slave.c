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
//    mkfifo(argv[1],0666);
//    FILE *pipe = fopen(argv[1], "w");
    FILE *file = fopen("pipe.txt","w");

    srand(time(NULL));

    // second index in points[][]
    // meaning: 0 - re, 1 - im, 2 - iters
    double points[N][3];
    int i;
//    char buffer[100];
    for (i = 0; i < N; i++) {
        points[i][0] = -2 + (rand() / (RAND_MAX / 3.0));  // -2 < re < 1
        points[i][1] = -1 + (rand() / (RAND_MAX / 2.0));  // -1 < im < 1
        points[i][2] = countIters(points[i][0], points[i][1], K);
//        printf("Points[%d] = (%lf, %lf, %d)\n", i, points[i][0], points[i][1], (int) points[i][2]);

        fprintf(file,"%lf %lf %d\n",points[i][0], points[i][1], (int) points[i][2]);
//        sprintf(buffer, "%lf %lf %d\n", points[i][0], points[i][1], (int) points[i][2]);
//        fprintf(pipe, "%s", buffer);
//        fprintf(pipe, "%lf %lf %d\n", points[i][0], points[i][1], (int) points[i][2]);
    }
//    fclose(pipe);

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
    if (i < 0)
        return 0;
    return i - 1;
}