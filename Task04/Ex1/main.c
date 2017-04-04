#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

bool forward = true;

void sigHandler(int signum)
{
    if (signum == SIGINT) {
        printf("Received signal SIGINT.\n");
        exit(1);
    }
    else if (signum == SIGTSTP) {
        forward = !forward;
    }
}


int main(int argc, char *argv[]) {

    char letter = 'A';
    struct sigaction sa;
    sa.sa_handler = sigHandler;

    while(1) {
        printf(" %c\n",letter);
        usleep(150000);

        if (forward)
            letter++;
        else
            letter--;

        if(letter > 'Z')
            letter = 'A';
        else if (letter < 'A')
            letter = 'Z';

        signal(SIGINT, sigHandler);
        sigaction(SIGTSTP, &sa, NULL);
    }

    return 0;
}
