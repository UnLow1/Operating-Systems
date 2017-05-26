#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
FILE *file;
int records_number;
char *word;
int threads_number;
pthread_t *threads;
int counter;

void error(char *msg) {
    perror(msg);
    exit(-1);
}

void *read_function(void *a) {
    if (pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL) != 0)
        error("Error pthread_setcanceltype");

    pthread_testcancel();

    if (pthread_mutex_lock(&mutex) != 0)
        error("Error pthread_mutex_lock");

    if (pthread_mutex_unlock(&mutex) != 0)
        error("Error pthread_mutex_lock");


    char *buffer;
    buffer = malloc(1024 * sizeof(char));
    bool loop = true;
    while (loop) {
        if (pthread_mutex_lock(&mutex) != 0)
            error("Error pthread_mutex_lock");

        int i;
        for (i = 0; i < records_number && loop; i++) {
            if (fread(buffer, 1024, 1, file) == 0)
                loop = false;   // end of file
            counter++;

            if (strstr(buffer, word) != NULL) {
                // found word in buffer
                loop = false;
                pthread_t ID = pthread_self();
                printf("Thread %ld found word in record %d\n", ID, counter);
                int j;
                for (j = 0; j < threads_number; j++) {
                    if (!pthread_equal(ID, threads[j])) {
                        if (pthread_cancel(threads[j]) != 0)
                            error("Error pthread_cancel");
                    }
                }
            }
        }
        if (pthread_mutex_unlock(&mutex) != 0)
            error("Error pthread_mutex_unlock");
        pthread_testcancel();
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 5)
        error("Wrong arguments! 4 args are needed: threads_number, filename, records_number and word");


    threads_number = atoi(argv[1]);
    char *filename = argv[2];
    records_number = atoi(argv[3]);
    word = argv[4];

    file = fopen(filename, "r");
    if (file == NULL)
        error("Error when opening file");

    counter = 0;
    threads = malloc(threads_number * sizeof(pthread_t));
    int i;
    for (i = 0; i < threads_number; i++) {
        if (pthread_create(&threads[i], NULL, read_function, NULL) == -1)
            error("Can't open thread");

    }


    for (i = 0; i < threads_number; i++) {
        if (pthread_join(threads[i], NULL) != 0)
            error("Error pthread_join");
    }


    fclose(file);
    free(threads);

    return 0;
}
