#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <wait.h>

int counterReceivedSig1;
int counterParentReceivedSig1;
pid_t parent_pid;

void sighandler(int signum, siginfo_t *siginfo, void *context) {
    if (signum == SIGINT) {
        printf("%d Received SIGINT From %d\n", getpid(), siginfo->si_pid);
        exit(1);
    } else if (signum == SIGUSR1) {
        if (siginfo->si_pid == parent_pid) {
            counterReceivedSig1++;
            printf("counterReceivedSig1 = %d     %d Received SIGUSR1 From %d\n", counterReceivedSig1, getpid(),
                   siginfo->si_pid);
            kill(siginfo->si_pid, SIGUSR1);
        } else {
            // parent recevied signal
            counterParentReceivedSig1++;
            printf("counterParentReceivedSig1 = %d     %d Received SIGUSR1 From %d\n", counterParentReceivedSig1,
                   getpid(), siginfo->si_pid);
        }
    } else if (signum == SIGUSR2) {
        if (siginfo->si_pid == parent_pid) {
            printf("%d Received SIGUSR2 From %d\nEND WORKING\n", getpid(), siginfo->si_pid);
            exit(1);
        }
    } else if (signum == SIGRTMIN) {
        if (siginfo->si_pid == parent_pid) {
            counterReceivedSig1++;
            printf("counterReceivedSig1 = %d     %d Received SIGRTMIN From %d\n", counterReceivedSig1, getpid(),
                   siginfo->si_pid);
            kill(siginfo->si_pid, SIGRTMIN);
        } else {
            // parent recevied signal
            counterParentReceivedSig1++;
            printf("counterParentReceivedSig1 = %d     %d Received SIGRTMIN From %d\n", counterParentReceivedSig1,
                   getpid(), siginfo->si_pid);
        }
    } else if (signum == SIGRTMAX) {
        if (siginfo->si_pid == parent_pid) {
            printf("%d Received SIGRTMAX From %d\nEND WORKING\n", getpid(), siginfo->si_pid);
            exit(1);
        }
    } else {
        printf("%d Received SIGNAL %d From %d\n", getpid(), signum, siginfo->si_pid);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("Wrong arguments! Arguments should be L and Type (1, 2, 3)");
        exit(1);
    }
    int L = atoi(argv[1]);
    int Type = atoi(argv[2]);

    parent_pid = getpid();
    counterReceivedSig1 = 0;
    counterParentReceivedSig1 = 0;

    struct sigaction act;
    act.sa_sigaction = &sighandler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    sigaction(SIGRTMIN, &act, NULL);
    sigaction(SIGRTMAX, &act, NULL);

    int child_pid = fork(), i;

    if (child_pid == 0) {
        // child process

        while (true) {
        }
    } else if (child_pid > 0) {
        int counterSendSig1 = 0;
        union sigval sg;

        for (i = 0; i < L; ++i) {
            counterSendSig1++;
            printf("counterSendSig1 = %d     SIGUSR1 sending to %d\n", counterSendSig1, child_pid);
            switch (Type) {
                case 1:
                    kill(child_pid, SIGUSR1);
                    break;
                case 2:
                    sigqueue(child_pid, SIGUSR1, sg);
                    break;
                case 3:
                    kill(child_pid, SIGRTMIN);
                    break;
                default:
                    printf("Bad Type argument\n");
            }
            sleep(1);
        }

        if (Type == 3)
            printf("SIGRTMAX sending to %d\n", child_pid);
        else
            printf("SIGUSR2 sending to %d\n", child_pid);
        switch (Type) {
            case 1:
                kill(child_pid, SIGUSR2);
                break;
            case 2:
                sigqueue(child_pid, SIGUSR2, sg);
                break;
            case 3:
                kill(child_pid, SIGRTMAX);
                break;
            default:
                printf("Bad Type argument\n");
        }
    }
    int status;
    waitpid(child_pid, &status, 0);

    return 0;
}
