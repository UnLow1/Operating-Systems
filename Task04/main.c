#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>

int counter, N, M, copyM;
pid_t *pids;
pid_t parent_pid;

void sighandler(int signum, siginfo_t *siginfo, void *context) {
    if (signum == SIGUSR1) {
        printf("%d Received SIGUSR1 %d\n", getpid(), signum);
        if (M > 0) {
            M--;
            pids[M] = siginfo->si_pid;
            printf("M = %d    pid[%d] = %d\n", M, M, pids[M]);
            if (M == 0) {
                int i;
                for (i = 0; i < copyM; i++) {
                    printf("kill(%d, SIGUSR2)\n", pids[i]);
                    kill(pids[i], SIGUSR2);
                }
            }
        } else {
            printf("kill(%d, SIGUSR2)\n", siginfo->si_pid);
            kill(siginfo->si_pid, SIGUSR2);
        }
    } else if (signum == SIGINT) {
        printf("%d Received SIGINT %d\n", getpid(), signum);
        exit(1);
    } else if (signum == SIGUSR2) {
        printf("%d Received SIGUSR2 %d\n", getpid(), signum);
        int random = (unsigned) rand() % (SIGRTMAX - SIGRTMIN);
        random += SIGRTMIN;
        printf("kill(%d, random)\n", siginfo->si_pid);
        kill(siginfo->si_pid, random);
        //losuj
    } else {
        printf("%d Received signal %d\n", getpid(), signum);
    }
}

int main(int argc, char *argv[]) {
    counter = 0;
    pids = malloc(M * sizeof(int));
    if (argc != 3) {
        perror("Wrong arguments! Arguments should be N and M");
        return -1;
    }
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    copyM = M;
    time_t t;
    srand((unsigned) time(&t));
    parent_pid = getpid();
    struct sigaction act;
    act.sa_sigaction = &sighandler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    int i;
    for (i = SIGRTMIN; i <= SIGRTMAX; i++)
        sigaction(i, &act, NULL);
    printf("%d\n", counter);

    for (i = 0; i < N; ++i) {
        unsigned int r = (unsigned) rand() % 11;
        if (fork() == 0) {
            // do the job specific to the child process
            int child_pid;
            child_pid = getpid();
            printf("%d Start working\n", child_pid);
            printf("%d Going sleep for %d seconds\n", child_pid, r);
            union sigval value;
            value.sival_int = i;
            sleep(r);
            if (sigqueue(parent_pid, SIGUSR1, value) != 0) {
                perror("SIGSENT-ERROR:");
                exit(1);
            }
            while (true) {
                sleep(1);
            }   // just wait

            printf("%d End working\n", child_pid);

            // don't forget to exit from the child
            exit(0);
        }

    }



// wait all child processes
    int status;
    for (i = 0; i < N; ++i)
        wait(&status);
    printf("END MATHER %d\n", parent_pid);

    return 0;
}