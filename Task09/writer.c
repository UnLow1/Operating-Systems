#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

static pthread_once_t foo_once = PTHREAD_ONCE_INIT;
static pthread_mutex_t foo_mutex;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Wrong arguments! 1 arg is needed: N");
        exit(-1);
    }
    int N = atoi(argv[1]);

    printf("Writer start\n");
    sem_t sem1, sem2;

    int queueID = shm_open("shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(queueID, sizeof(int[N]));
    int *tab = (int *) mmap(NULL, sizeof(int[N]), PROT_READ | PROT_WRITE, MAP_SHARED, queueID, 0);

    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 1);
    sem_open("sem1", O_CREAT);
    sem_open("sem2", O_CREAT);

    srand((unsigned int) time(NULL));
    pthread_mutex_lock(&foo_mutex);
    /* Do work. */
    int numbers_to_change = rand() % (N + 1);

    while (numbers_to_change >= 0) {
        int place = rand() % N;
        tab[place] = rand();

        numbers_to_change--;
    }


    pthread_mutex_unlock(&foo_mutex);

    sem_destroy(&sem1);
    sem_destroy(&sem2);
    printf("Writer end\n");

    return 0;
}
