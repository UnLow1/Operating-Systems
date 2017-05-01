#include <stdio.h>
#include <stdlib.h>
#include <bits/signum.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <mqueue.h>
#include "message.h"

int server_queue_ID;
int client_queue_ID;
char client_queue_name[SIZE];
bool connected;
int ID;

void send_message(Message message);

void close_queue();

void receive_message();

int open_queue(char client_queue_name[SIZE]);

void sig_handler(int signum);

int main(int argc, char *argv[]) {
    if (argc != 1) {
        perror("Wrong arguments! 0 args needed");
        exit(-1);
    }
    signal(SIGINT, sig_handler);
    atexit(close_queue);

    server_queue_ID = mq_open(SERVER_QUEUE_NAME, O_WRONLY);

    sprintf(client_queue_name, "/client_%d", getpid());
    client_queue_ID = open_queue(client_queue_name);

    printf("Client is now running...\n");

    struct Message message;
    message.pid = getpid();
    connected = false;
    char command[SIZE];
    while (true) {
        message.ID = ID;
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
                close_queue();
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

int open_queue(char client_queue_name[SIZE]) {
    struct mq_attr attr;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = MAX_QUEUE_SIZE;
    return mq_open(client_queue_name, O_CREAT | O_RDONLY, 0600, &attr);
}

void sig_handler(int signum) {
    printf("Caught signal %d, closing queue and coming out...\n", signum);
    exit(1);
}

void send_message(Message message) {
    if (mq_send(server_queue_ID, (char *) &message, MSG_SIZE, 0) == -1) {
        perror("Could not send message");
        exit(1);
    }
}

void receive_message() {
    Message message;
    if (mq_receive(client_queue_ID, (char *) &message, MSG_SIZE, 0) == -1) {
        perror("mq_receive problem");
        exit(-1);
    }
    if (message.type == CONNECTED) {
        connected = true;
        ID = message.ID;
    }

    printf("%s\n", message.text);
}

void close_queue() {
    mq_close(server_queue_ID);
    mq_close(client_queue_ID);
    mq_unlink(client_queue_name);
}