#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("Wrong arguments! Two args needed. Path, R");
        exit(-1);
    }
    int R = atoi(argv[2]), i;
    mkfifo(argv[1],0666);
    FILE *pipe = fopen(argv[1], "r");

    int **T = (int **) malloc(R* sizeof(*T));
    for (i = 0; i <= R; i++)
        T[i] = (int *) malloc(R* sizeof(*T[i]));

    // index in points[] meaning: 0 - re, 1 - im, 2 - iters
    char *buffor;
    char *line = malloc(64*sizeof(char));
    int x, y;
    double points[3];
    FILE *file = fopen("data","w");

    while (fgets(line, 64, pipe)) {
        buffor = strtok(line, " \n");
        points[0] = atof(buffor);
        buffor = strtok(NULL, " \n");
        points[1] = atof(buffor);
        buffor = strtok(NULL, " \n");
        points[2] = atoi(buffor);

        x = (int) ((points[0] + 2) / 3 * R);
        y = (int) ((points[1] + 1) / 2 * R);
        T[x][y] = (int) points[2];
    }
    fclose(pipe);

    int j;
    for (i =0 ;i<R;i++)
        for(j=0;j<R;j++)
            fprintf(file,"%d %d %d\n",i,j,T[i][j]);

    fclose(file);

    FILE *plot = popen("gnuplot", "w");
    fprintf(plot, "set view map\n");
    fprintf(plot, "set xrange [0:%d]\n", R);
    fprintf(plot, "set yrange[0:%d]\n",R);
    fprintf(plot,"plot 'data' with image\n");
    fflush(plot);
    getchar();

    pclose(plot);

    return 0;
}
