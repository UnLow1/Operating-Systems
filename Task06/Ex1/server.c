#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include "message.h"

int server_queue_ID;

int open_queue(key_t keyval);

void convert_to_upper_case(char *sPtr);

void close_queue();

void send_message(Message message);

void sig_handler(int signum);

void set_time(char *text);

void receive_message();

int main(int argc, char *argv[]) {
    if (argc != 1) {
        perror("Wrong arguments! 0 args needed");
        exit(-1);
    }
    atexit(close_queue);
    int clients_counter = 0;
    key_t keyval = ftok("HOME", ID);
    server_queue_ID = open_queue(keyval);
    signal(SIGINT, sig_handler);

    printf("Server is running...\n");

    struct Message message;
    while (true) {
        if (msgrcv(server_queue_ID, &message, MSG_SIZE, 0, 0) == -1) {
            perror("msgrcv problem");
            exit(-1);
        }
        switch (message.type) {
            case CONNECT:
                if (clients_counter < MAX_CLIENTS) {
                    clients_counter++;
                    printf("Client %d connected\n", message.pid);
                    message.type = CONNECTED;
                    strcpy(message.text, "You are connected to server");
                } else {
                    printf("Server is full, Client %d didn't connect\n", message.pid);
                    strcpy(message.text, "You aren't connected to server");
                }
                send_message(message);
                break;
            case ECHO:
                printf("Got 'echo' from Client %d\n", message.pid);
                send_message(message);
                break;
            case UPPER:
                printf("Got 'upper' from Client %d\n", message.pid);
                convert_to_upper_case(message.text);
                send_message(message);
                break;
            case TIME:
                printf("Got 'time' from Client %d\n", message.pid);
                set_time(message.text);
                send_message(message);
                break;
            case TERMINATE:
                printf("Got 'terminate' from Client %d\n", message.pid);
                exit(0);
            case WRONG:
                printf("Got wrong command from Client %d\n", message.pid);
                strcpy(message.text, "Wrong command");
                send_message(message);
                break;
            default:
                printf("default from Client %d\n", message.pid);
                break;
        }
    }

    return 0;
}

int open_queue(key_t keyval) {
    int qid;
    if ((qid = msgget(keyval, IPC_CREAT | 0600)) == -1) {
        return (-1);
    }
    return (qid);
}

void sig_handler(int signum) {
    printf("Caught signal %d, closing queue and coming out...\n", signum);
    exit(1);
}

void convert_to_upper_case(char *string) {
    while (*string) {
        *string = (char) toupper(*string);
        string++;
    }
}

void set_time(char *text) {
    time_t t;
    time(&t);
    strcpy(text, asctime(localtime(&t)));
}

void send_message(Message message) {
    if (msgsnd(message.queue_ID, &message, MSG_SIZE, 0) == -1) {
        printf("Could not send message\n");
        exit(1);
    }
}

void close_queue() {
    msgctl(server_queue_ID, IPC_RMID, NULL);
}

//void receive_message() {
//    if (msgrcv(server_queue_ID, &message, MSG_SIZE, 0, 0) == -1) {
//        perror("msgrcv problem");
//        exit(-1);
//    }
//}