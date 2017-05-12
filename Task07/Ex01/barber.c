#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sem.h>
#include <bits/signum.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/shm.h>
#include "header.h"

void timeCheckpoint();

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

    bool open = true;
    // createSemaphores
    int semID = semget(key, 2, IPC_CREAT | 0666);
    semctl(semID, 0, SETVAL, 0);
    semctl(semID, 1, SETVAL, 1);

    printf("Barber in work\n");
    struct sembuf *buffer;

    while (open) {
        timeCheckpoint();
        printf("Barber is going sleep\n");
        // takeSemaphore
        buffer->sem_num = 1;       // FIFO == 1
        buffer->sem_op = -1;
        buffer->sem_flg = 0;
        semop(semID, buffer, 1);

        pid_t clientID;
        // takeSemaphore
        buffer->sem_num = 1;       // FIFO == 1
        buffer->sem_op = -1;
        buffer->sem_flg = 0;
        semop(semID, buffer, 1);

        clientID = myQueue->chair;
        // giveSemaphore
        buffer->sem_num = 1;    // FIFO == 1
        buffer->sem_op = 1;
        buffer->sem_flg = 0;
        semop(semID, buffer, 1);

        timeCheckpoint();
        printf("Barber is cutting client %d\n", clientID);
        kill(clientID, SIGUSR1);
        timeCheckpoint();
        printf("Barber end cutting client %d\n", clientID);

        while (true) {
            //takeSemaphore
            buffer->sem_num = 1;       // FIFO == 1
            buffer->sem_op = -1;
            buffer->sem_flg = 0;
            semop(semID, buffer, 1);

            if (myQueue->actual_size > 0) {
                clientID = myQueue->queue[0];
                int i;
                for (i = 0; i < myQueue->actual_size; ++i) {
                    myQueue->queue[i] = myQueue->queue[i + 1];
                }
                // giveSemaphore
                buffer->sem_num = 1;    // FIFO == 1
                buffer->sem_op = 1;
                buffer->sem_flg = 0;
                semop(semID, buffer, 1);

                timeCheckpoint();
                printf("Barber is cutting client %d\n", clientID);
                kill(clientID, SIGUSR1);
                timeCheckpoint();
                printf("Barber end cutting client %d\n", clientID);
            } else {
                // takeSemaphore
                buffer->sem_num = 0;       // barber == 0
                buffer->sem_op = -1;
                buffer->sem_flg = 0;
                semop(semID, buffer, 1);

                // giveSemaphore
                buffer->sem_num = 1;    // FIFO == 1
                buffer->sem_op = 1;
                buffer->sem_flg = 0;
                semop(semID, buffer, 1);

                break;
            }
        }

    }

    return 1;
}

void timeCheckpoint() {
    struct timespec real_time;
    clock_gettime(CLOCK_MONOTONIC, &real_time);
    printf("timeCheckpoint %lld\n", (long long) (real_time.tv_sec * 1000000 + real_time.tv_nsec / 1000));
}
