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
//    FILE *pipe = fopen(argv[1], "r");
    FILE *pipe = fopen("pipe.txt", "r");

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
//        printf("line = %s",line);
        buffor = strtok(line, " \n");
        points[0] = atof(buffor);
//        printf("points[0] = %f\n",points[0]);
        buffor = strtok(NULL, " \n");
        points[1] = atof(buffor);
//        printf("points[1] = %f\n",points[1]);
        buffor = strtok(NULL, " \n");
        points[2] = atoi(buffor);
//        printf("points[2] = %d\n",(int)points[2]);

        x = (int) ((points[0]+2.0) / 3.0 * R);
//        printf("x = %d\n",x);
        y = (int) ((points[1]+1.0) / 2.0 * R);
//        printf("y = %d\n",y);
        T[x][y] = (int) points[2];
//        printf("T[x][y] = %d\n",T[x][y]);

        fprintf(file,"%d %d %d\n",x,y,T[x][y]);


    }
    fclose(file);

    FILE *gplotPipe = popen("gnuplot", "w");
    fprintf(gplotPipe, "set view map\nset xrange[0:%d]\nset yrange[0:%d]\nplot 'data' with image\n", R, R);

    fflush(gplotPipe);
    printf("Press [ENTER] to exit\n");
    getchar();

    pclose(gplotPipe);

    return 0;
}
