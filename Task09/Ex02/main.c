#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

bool wait = true;
bool description_option = false;
int writers;
int writers_active = 0;
int reads;
int *table;
sem_t *readers;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t readers_queue = PTHREAD_COND_INITIALIZER;
pthread_cond_t writers_queue = PTHREAD_COND_INITIALIZER;

void error(char *msg) {
    perror(msg);
    exit(-1);
}

void *writerFunction(void *args) {
    while (wait);
    if (description_option)
        printf("Writer %ld is live\n", pthread_self());
    int i;
    for (i = 0; i < writers; ++i) {
        pthread_mutex_lock(&mutex);
        writers++;
        while (readers > 0 || writers_active > 0) {
            pthread_cond_wait(&writers_queue, &mutex);
        }
        writers_active++;
        pthread_mutex_unlock(&mutex);

        int change = rand() % 11;
        int j;
        for (j = 0; j < change; ++j) {
            int ind = rand() % 11;
            int number = rand() % 100;
            if (description_option)
                printf("Writer %ld wrote number = %d at index ind = %d\n", pthread_self(), number, ind);
            table[ind] = number;
        }
        printf("Writer %ld changed %d numbers\n", pthread_self(), change);

        pthread_mutex_lock(&mutex);
        writers_active--;
        writers--;
        if (writers > 0)
            pthread_cond_signal(&writers_queue);
        else
            pthread_cond_broadcast(&readers_queue);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

void *readerFunction(void *args) {
    int div = 2 + (unsigned int) rand() % 11;
    while (wait);
    if (description_option)
        printf("Reader %ld is live with div = %d\n", pthread_self(), div);
    int i;
    for (i = 0; i < reads; ++i) {
        pthread_mutex_lock(&mutex);
        while (writers != 0)
            pthread_cond_wait(&readers_queue, &mutex);
        readers++;
        pthread_mutex_unlock(&mutex);

        int counter = 0;
        int j;
        for (j = 0; j < 11; ++j) {
            if (table[j] % div == 0) {
                counter++;
                if (description_option)
                    printf("Reader %ld found number = %d at index ind = %d\n", pthread_self(), table[j], j);
            }
        }
        printf("Reader %ld found %d numbers\n", pthread_self(), counter);

        pthread_mutex_lock(&mutex);
        readers--;
        if (readers == 0)
            pthread_cond_broadcast(&writers_queue);
        else
            pthread_cond_broadcast(&readers_queue);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 6)
        description_option = true;
    else if (argc != 5)
        error("Wrong arguments! 5 args are needed: writers_number, readers_number, writes, reads");

    int shift;
    if (description_option)
        shift = 1;
    else
        shift = 0;
    int writers_number = atoi(argv[1 + shift]);
    int readers_number = atoi(argv[2 + shift]);
    writers = atoi(argv[3 + shift]);
    reads = atoi(argv[4 + shift]);
    srand((unsigned int) time(NULL));

    // getResources()
    int memoryID = shm_open("shared_mem", O_CREAT | O_RDWR, 0666);
    if (memoryID == -1)
        error("shm_open memoryID");
    if (ftruncate(memoryID, 11 * sizeof(int)) == -1)
        error("ftruncate(memoryID,__)");
    void *shared = mmap(NULL, 11 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, memoryID, 0);
    if (shared == (void *) -1)
        error("mmap shared");
    table = (int *) shared;
    pthread_t *threadID = (pthread_t *) calloc((size_t) writers_number + readers_number, sizeof(pthread_t));
    if (threadID == NULL)
        error("calloc threadID");

    int i;
    for (i = 0; i < writers_number; ++i)
        if (pthread_create(&threadID[i], NULL, writerFunction, NULL) != 0)
            error("pthread_create writersFunction");


    for (i = writers_number; i < readers_number + writers_number; ++i)
        if (pthread_create(&threadID[i], NULL, readerFunction, NULL) != 0)
            error("pthread_create readerFunction");


    wait = false;
    for (i = 0; i < writers_number + readers_number; ++i)
        if (pthread_join(threadID[i], NULL) != 0)
            error("pthread_join");


    if (munmap(table, 11 * sizeof(int)) == -1)
        error("munmap");
    if (shm_unlink("shared_mem") == -1)
        error("shm_unlink");

    free(threadID);

    return 0;
}
