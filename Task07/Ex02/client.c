#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <bits/signum.h>
#include <bits/time.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <wait.h>
#include "header.h"

void sighandler(int signum);

void timeCheckpoint();

int counter;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("Wrong arguments! 2 args are needed: N and S");
        exit(-1);
    }
    int N = atoi(argv[1]);  // number of clients
    int S = atoi(argv[2]);  // how many times client should be cut
    counter = 0;

    // getFIFO
    int queueID = shm_open("shared_memory", O_RDWR, 0666);
    MyQueue *myQueue = mmap(NULL, sizeof(MyQueue), PROT_READ | PROT_WRITE, MAP_SHARED, queueID, 0);

    // getSemaphores
    sem_t *semaphore_barber, *semaphore_queue;
//    semaphore_barber = sem_open(BARBER, O_RDWR);
//    semaphore_queue = sem_open(QUEUE, O_RDWR);
    if ((semaphore_barber = sem_open("barber", O_RDWR)) == SEM_FAILED)
        printf("Problem with barber semaphore\n");
    if ((semaphore_queue = sem_open("queue", O_RDWR)) == SEM_FAILED)
        printf("Problem with queue semaphore\n");

    signal(SIGUSR1, sighandler);
    sigset_t eventMask;
    sigfillset(&eventMask);
    sigdelset(&eventMask, SIGUSR1);
    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &blockMask, NULL);

    bool inSalon = false;
    int i;
    pid_t child_pid = NULL;
    for (i = 0; i < N; i++) {
        child_pid = fork();
        if (child_pid == 0) {
            // child process
            pid_t pid = getpid();
            timeCheckpoint();
            printf("Client %d starting\n", pid);
            while (counter < S) {
                sem_wait(semaphore_queue);

                int barberActive;
                sem_getvalue(semaphore_barber, &barberActive);
                if (!barberActive) {
                    sem_post(semaphore_barber);
                    timeCheckpoint();
                    printf("Client %d is waking up barber\n", pid);
                    inSalon = true;
                    sem_post(semaphore_barber);
                    myQueue->chair = pid;
                } else {
                    if (myQueue->max_size > myQueue->actual_size) {
                        // take a seat in a myQueue
                        myQueue->queue[myQueue->actual_size] = pid;
                        myQueue->actual_size++;
                        inSalon = true;
                        timeCheckpoint();
                        printf("Client %d is in queue\n", pid);
                    } else {
                        // Barber's studio is full
                        timeCheckpoint();
                        printf("Queue is full. Client %d is leavnig Barber.\n", pid);
                        sleep(1);
                    }
                }

                sem_post(semaphore_queue);
                if (inSalon) {
                    sigsuspend(&eventMask);
                    timeCheckpoint();
                    printf("Client %d got cut. Client is leaving Barber.\n", pid);
                    sleep(1);
                    inSalon = false;
                }

            }
            timeCheckpoint();
            printf("Client %d ending work\n", pid);
            sem_close(semaphore_queue);
            sem_close(semaphore_barber);
            exit(1);
        }
    }
    int status;
    waitpid(child_pid, &status, 0);

    sem_close(semaphore_queue);
    sem_close(semaphore_barber);

    return 0;
}

void sighandler(int signum) {
    if (signum == SIGUSR1)
        counter++;
    else if (signum == SIGINT) {
        printf("\nGot SIGINT, ending work.\n");
        exit(1);
    }
}

void timeCheckpoint() {
    struct timespec real_time;
    clock_gettime(CLOCK_MONOTONIC, &real_time);
    printf("timeCheckpoint %lld    ", (long long) (real_time.tv_sec * 1000000 + real_time.tv_nsec / 1000));
}
