#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>

int N, M, ind;
pid_t *pids;
pid_t parent_pid;

void sighandler(int signum, siginfo_t *siginfo, void *context) {
    if (signum == SIGUSR1) {
        printf("%d Received SIGUSR1 From %d\n", getpid(), siginfo->si_pid);
        pids[ind] = siginfo->si_pid;
        ind++;
        if (M > 0) {
            M--;
            if (M == 0) {
                int i;
                for (i = 0; i < ind; i++) {
//                    printf("kill(%d, SIGUSR2)\n", pids[i]);
                    kill(pids[i], SIGUSR2);
                }
            }
        } else {
//            printf("kill(%d, SIGUSR2)\n", siginfo->si_pid);
            kill(siginfo->si_pid, SIGUSR2);
        }
    } else if (signum == SIGINT) {
        printf("%d Received SIGINT From %d\n", getpid(), siginfo->si_pid);
        exit(1);
    } else if (signum == SIGUSR2) {
        printf("%d Received SIGUSR2 From %d\n", getpid(), siginfo->si_pid);
        int random = (unsigned) rand() % (SIGRTMAX - SIGRTMIN);
        random += SIGRTMIN;
//        printf("kill(%d, %d)\n", siginfo->si_pid,random);
        kill(siginfo->si_pid, random);
    } else {
        printf("%d Received signal %d From %d\n", getpid(), signum, siginfo->si_pid);
    }
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        perror("Wrong arguments! Arguments should be N and M");
        exit(1);
    }
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    pids = malloc(N * sizeof(int));
    ind = 0;
    time_t t;
    srand((unsigned) time(&t));
    parent_pid = getpid();
    printf("PARENT PID = %d\n",parent_pid);
    struct sigaction act;
    act.sa_sigaction = &sighandler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    int i;
    for (i = SIGRTMIN; i <= SIGRTMAX; i++)
        sigaction(i, &act, NULL);

    for (i = 0; i < N; ++i) {
        unsigned int r = (unsigned) rand() % 11;
        if (fork() == 0) {
            // do the job specific to the child process
            int child_pid;
            child_pid = getpid();
            printf("%d Start working\n", child_pid);
            printf("%d Going sleep for %d seconds\n", child_pid, r);
            sleep(r);

            time_t before;
            time(&before);

            if (kill(parent_pid, SIGUSR1) != 0) {
                perror("SIGSENT-ERROR:");
                exit(1);
            }

            while(true) {
                continue;
            }

            time_t after;
            time(&after);
            exit((int)difftime(before,after));
        }
    }
// wait all child processes

    int status;
    for (i = 0; i < N; i++) {
        waitpid(pids[i], &status, 0);
        status = WEXITSTATUS(status);
        printf("%d exited with code %d\n", pids[i], status);
    }

    sleep(7);
    printf("END PARENT %d\n", parent_pid);

    return 0;
}
