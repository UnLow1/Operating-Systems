#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sem.h>
#include <bits/signum.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <unistd.h>
#include "header.h"

void timeCheckpoint();

void sighandler(int signum);

void semaphore(unsigned short num, short op, short flg, struct sembuf *buffer, int semID);

bool open;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Wrong arguments! 1 arg is needed: N");
        exit(-1);
    }

    int N = atoi(argv[1]);
    // getFIFO
    key_t key = ftok(PATH, PROJ_ID);
    int queueID = shmget(key, sizeof(struct MyQueue), IPC_CREAT | 0666);
    struct MyQueue *myQueue = (struct MyQueue *) shmat(queueID, 0, 0);

    myQueue->max_size = N;
    myQueue->chair = 0;
    myQueue->actual_size = 0;
    open = true;

    // createSemaphores
    int semID = semget(key, 2, IPC_CREAT | 0666);
    semctl(semID, 0, SETVAL, 0);
    semctl(semID, 1, SETVAL, 1);

    printf("Barber in work\n");
    struct sembuf buffer;

    while (open) {
        timeCheckpoint();
        printf("Barber is going sleep\n");

        semaphore(0, -1, 0, &buffer, semID);

        pid_t clientID;

        semaphore(1, -1, 0, &buffer, semID);

        clientID = myQueue->chair;

        semaphore(1, 1, 0, &buffer, semID);

        timeCheckpoint();
        printf("Barber start cutting client %d\n", clientID);
        kill(clientID, SIGUSR1);
        timeCheckpoint();
        printf("Barber end cutting client %d\n", clientID);

        while (true) {

            semaphore(1, -1, 0, &buffer, semID);

            if (myQueue->actual_size > 0) {
                clientID = myQueue->queue[0];
                int i;
                for (i = 0; i < myQueue->actual_size - 1; ++i) {
                    myQueue->queue[i] = myQueue->queue[i + 1];
                }
                myQueue->actual_size--;

                semaphore(1, 1, 0, &buffer, semID);

                timeCheckpoint();
                printf("Barber start cutting client %d\n", clientID);
                sleep(1);
                kill(clientID, SIGUSR1);
                timeCheckpoint();
                printf("Barber end cutting client %d\n", clientID);
            } else {

                semaphore(0, -1, 0, &buffer, semID);

                semaphore(1, 1, 0, &buffer, semID);

                break;
            }
        }

    }

    return 1;
}

void timeCheckpoint() {
    struct timespec real_time;
    clock_gettime(CLOCK_MONOTONIC, &real_time);
    printf("timeCheckpoint %lld    ", (long long) (real_time.tv_sec * 1000000 + real_time.tv_nsec / 1000));
}

void sighandler(int signum) {
    if (signum == SIGINT) {
        printf("\nBarber, interrupted\n");
        open = false;
        exit(1);
    }
}

void semaphore(unsigned short num, short op, short flg, struct sembuf *buffer, int semID) {
    buffer->sem_num = num;
    buffer->sem_op = op;
    buffer->sem_flg = flg;
    semop(semID, buffer, 1);
}
