#include <stdio.h>
#include <stdlib.h>
#include <bits/signum.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include "header.h"

void sighandler(int signum);

void timeCheckpoint();

bool is_open;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Wrong arguments! 1 arg is needed: N");
        exit(-1);
    }

    int N = atoi(argv[1]);
    signal(SIGINT, sighandler);

    int queueID = shm_open("shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(queueID, sizeof(MyQueue));
    MyQueue *myQueue = (MyQueue *) mmap(NULL, sizeof(MyQueue), PROT_READ | PROT_WRITE, MAP_SHARED, queueID, 0);
    myQueue->actual_size = 0;
    myQueue->chair = 0;
    myQueue->max_size = N;

    sem_t *semaphore_barber, *semaphore_queue;
    semaphore_barber = sem_open("barber", O_CREAT | O_RDWR, 0666, 0);
    semaphore_queue = sem_open("queue", O_CREAT | O_RDWR, 0666, 0);

    printf("Barber in work\n");
    is_open = true;

    while (is_open) {
        timeCheckpoint();
        printf("Barber is going sleep\n");

        sem_wait(semaphore_barber);
        pid_t clientID;
        sem_wait(semaphore_queue);
        clientID = myQueue->chair;
        sem_post(semaphore_queue);

        timeCheckpoint();
        printf("Barber start cutting client %d\n", clientID);
        kill(clientID, SIGUSR1);
        timeCheckpoint();
        printf("Barber end cutting client %d\n", clientID);

        while (true) {
            sem_wait(semaphore_queue);

            if (myQueue->actual_size > 0) {
                clientID = myQueue->queue[0];
                int i;
                for (i = 0; i < myQueue->actual_size - 1; ++i) {
                    myQueue->queue[i] = myQueue->queue[i + 1];
                }
                myQueue->actual_size--;

                sem_post(semaphore_queue);

                timeCheckpoint();
                printf("Barber start cutting client %d\n", clientID);
                sleep(1);
                kill(clientID, SIGUSR1);
                timeCheckpoint();
                printf("Barber end cutting client %d\n", clientID);
            } else {
                sem_wait(semaphore_barber);
                sem_post(semaphore_queue);
                break;
            }
        }

    }
    shm_unlink("shared_memory");
    sem_close(semaphore_queue);
    sem_unlink("queue");
    sem_close(semaphore_barber);
    sem_unlink("barber");

    return 0;
}

void sighandler(int signum) {
    if (signum == SIGINT) {
        printf("\nGot SIGINT, ending work.\n");
        is_open = false;
        exit(1);
    }
//    else if (signum == SIGUSR1) {
//        printf("SIGUSR1\n");
//    }
}

void timeCheckpoint() {
    struct timespec real_time;
    clock_gettime(CLOCK_MONOTONIC, &real_time);
    printf("timeCheckpoint %lld    ", (long long) (real_time.tv_sec * 1000000 + real_time.tv_nsec / 1000));
}
