#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

bool forward = true;

void sigIntHandler(int signum)
{
   printf("Received signal SIGINT.\n");
   exit(1);
}

void sigStpHandler(int signum)
{
   forward = !forward;
}

int main(int argc, char *argv[]) {
   
   char letter = 'A';
   struct sigaction sa;
   sa.sa_handler = sigStpHandler;
   
   while(1) {
   	printf(" %c\n",letter);
   	usleep(150000);
   	
   	if (forward)
	   	letter++;
   	else
   		letter--;
   		
   	if(letter > 'Z')
   		letter = 'A';
   	if (letter < 'A')
   		letter = 'Z';
   		
   	signal(SIGINT, sigIntHandler);
   	sigaction(SIGTSTP, &sa, NULL);
   }

    return 0;
}
