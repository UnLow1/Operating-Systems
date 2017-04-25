#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <bits/signum.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "message.h"

int server_queue_ID;
int client_queue_ID;
bool connected;

void send_message(Message message);

void close_queue();

void receive_message();

int open_queue(key_t keyval, int flags);

void sig_handler(int signum);

int main(int argc, char *argv[]) {
    if (argc != 1) {
        perror("Wrong arguments! 0 args needed");
        exit(-1);
    }
    signal(SIGINT, sig_handler);
    atexit(close_queue);

    key_t keyval = ftok("HOME", ID);
    server_queue_ID = open_queue(keyval, 0);
    client_queue_ID = open_queue(IPC_PRIVATE, 0600);

    printf("Client is now running...\n");

    struct Message message;
    message.pid = getpid();
    message.queue_ID = client_queue_ID;
    connected = false;
    char command[SIZE];
    while (true) {
        scanf("%s", command);
        if (!connected) {
            if (strcmp(command, "connect") == 0) {
                message.type = CONNECT;
                send_message(message);
            }
        } else {
            if (strcmp(command, "echo") == 0) {
                message.type = ECHO;
                scanf("%s", message.text);
                send_message(message);
            } else if (strcmp(command, "upper") == 0) {
                message.type = UPPER;
                scanf("%s", message.text);
                send_message(message);
            } else if (strcmp(command, "time") == 0) {
                message.type = TIME;
                send_message(message);
            } else if (strcmp(command, "terminate") == 0) {
                message.type = TERMINATE;
                send_message(message);
                msgctl(client_queue_ID, IPC_RMID, NULL);
                exit(0);
            } else {
                message.type = WRONG;
                send_message(message);
            }
        }


        receive_message();
    }

    return 1;
}

int open_queue(key_t keyval, int flags) {
    int qid;
    if ((qid = msgget(keyval, flags)) == -1) {
        perror("Error while opening queue");
        exit(-1);
    }
    return (qid);
}

void sig_handler(int signum) {
    printf("Caught signal %d, closing queue and coming out...\n", signum);
    Message message;
    message.type = TERMINATE;
    message.pid = getpid();
    send_message(message);
    exit(1);
}

void send_message(Message message) {
    if (msgsnd(server_queue_ID, &message, MSG_SIZE, 0) == -1) {
        printf("Error when sending message\n");
        exit(1);
    }
}

void receive_message() {
    Message message;
    if (msgrcv(client_queue_ID, &message, MSG_SIZE, 0, 0) == -1) {
        perror("msgrcv problem");
        exit(-1);
    }
    if (message.type == CONNECTED)
        connected = true;
    printf("%s\n", message.text);
}

void close_queue() {
    msgctl(client_queue_ID, IPC_RMID, NULL);
}
