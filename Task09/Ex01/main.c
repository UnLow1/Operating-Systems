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
int writes;
int reads;
int readers_waiting = 0;
int *table;
sem_t *resources;
sem_t *readers;

void error(char *msg) {
    perror(msg);
    exit(-1);
}

void *writerFunction(void *args) {
    while (wait);
    if (description_option)
        printf("Writer %ld is live\n", pthread_self());
    int i;
    for (i = 0; i < writes; ++i) {
        if (sem_wait(resources) == -1)
            error("sem_wait(resources)");
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
        if (sem_post(resources) == -1)
            error("sem_post(resources)");
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
        if (sem_wait(readers) == -1)
            error("sem_wait(readers)");
        readers_waiting++;
        if (readers_waiting == 1)
            if (sem_wait(resources) == -1)
                error("sem_wait(resources)");
        if (sem_post(readers) == -1)
            error("sem_post(readers)");

        int counter = 0;
        int j;
        for (j = 0; j < 11; ++j) {
            if (table[j] % div == 0) {
                counter++;
                if (description_option)
                    printf("Reader %ld found number = %d at index ind = %d\n", pthread_self(), table[j], j);
            }
        }
        printf("Reader %ld found %d numbers\n", pthread_self(),counter);

        if (sem_wait(readers) == -1)
            error("sem_wait(readers)");
        readers_waiting--;
        if (readers_waiting == 0)
            if (sem_post(resources) == -1)
                error("sem_post(resources)");
        if (sem_post(readers) == -1)
            error("sem_post(readers)");

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
    writes = atoi(argv[3 + shift]);
    reads = atoi(argv[4 + shift]);
    srand((unsigned int) time(NULL));

    readers = sem_open("readers", O_CREAT | O_RDWR, 0666, 1);
    if (readers == SEM_FAILED)
        error("sem_open readers");
    sem_t *writers = sem_open("writes", O_CREAT | O_RDWR, 0666, 1);
    if (writers == SEM_FAILED)
        error("sem_open writes");
    resources = sem_open("resources", O_CREAT | O_RDWR, 0666, 1);
    if (resources == SEM_FAILED)
        error("sem_open resources");

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
    for (i = 0; i < writers_number; ++i) {
        if (pthread_create(&threadID[i], NULL, writerFunction, NULL) != 0)
            error("pthread_create writersFunction");
    }

    for (i = writers_number; i < readers_number + writers_number; ++i) {
        if (pthread_create(&threadID[i], NULL, readerFunction, NULL) != 0)
            error("pthread_create readerFunction");
    }

    wait = false;
    for (i = 0; i < writers_number + readers_number; ++i) {
        if (pthread_join(threadID[i], NULL) != 0)
            error("pthread_join");
    }

    if (munmap(table, 11 * sizeof(int)) == -1)
        error("munmap");
    if (shm_unlink("shared_mem") == -1)
        error("shm_unlink");

    if(sem_close(writers) != 0)
        error("sem_close writes");
    if(sem_unlink("writes") != 0)
        error("sem_unlink writes");

    if(sem_close(readers) != 0)
        error("sem_close readers");
    if(sem_unlink("readers") != 0)
        error("sem_unlink readers");

    if(sem_close(resources) != 0)
        error("sem_close resources");
    if(sem_unlink("resources") != 0)
        error("sem_unlink resources");

    free(threadID);

    return 0;
}
