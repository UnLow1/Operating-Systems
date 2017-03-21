// compilation arguments: "./program sys shuffle datafile.txt 100 512"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void generate(char *filename, int size, int quantity);

int main(int argc, char *argv[]) {

    char *filename = argv[3];
    int size = atoi(argv[4]);
    int quantity = atoi(argv[5]);
    printf("size = %d\nquantity = %d\n\n", size, quantity);
    generate(filename, size, quantity);


    return 0;
}

void generate(char *filename, int size, int quantity) {
    int randomData = open("/dev/urandom", O_RDONLY);
    int results = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    char sign;
    for (int i = 0; i < quantity; i++) {
        for (int j = 0; j < size; j++) {
            read(randomData, &sign, sizeof(sign));
            //printf("%c", (int) (sign));
            //write(results, &sign, sizeof(sign));
            sign = (char) (rand() % 25 + 97);
            printf("%c", (int) (sign));
            write(results, &sign, sizeof(sign));
        }
        write(results, "\n", sizeof(char));
        printf("\n");
    }
    close(results);
    close(randomData);
}

void shuffle();