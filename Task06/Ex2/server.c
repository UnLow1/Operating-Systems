#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include "message.h"
#include <time.h>

int server_queue_ID;
char server_queue_name[] = SERVER_QUEUE_NAME;
int clients_queue_ID[MAX_CLIENTS];
int clients_counter;

int open_queue();

int open_client_queue(Message message);

void close_queue();

void sig_handler(int signum);

void send_message(Message message, int name);

void convert_to_upper_case(char *string);

void set_time(char *text);

int main(int argc, char *argv[]) {
    if (argc != 1) {
        perror("Wrong arguments! 0 args needed");
        exit(-1);
    }

    atexit(close_queue);
    clients_counter = 0;
    server_queue_ID = open_queue();
    signal(SIGINT, sig_handler);

    printf("Server is running...\n");

    struct Message message;
    while (true) {
        if (mq_receive(server_queue_ID, (char *) &message, MSG_SIZE, 0) == -1) {
            perror("mq_receive problem");
            exit(-1);
        }
        int tmp;
        switch (message.type) {
            case CONNECT:
                tmp = open_client_queue(message);
                if (clients_counter < MAX_CLIENTS) {
                    clients_queue_ID[clients_counter] = tmp;
                    message.ID = clients_counter;
                    clients_counter++;
                    printf("Client %d connected\n", message.pid);
                    message.type = CONNECTED;
                    strcpy(message.text, "You are connected to server");
                    send_message(message, clients_queue_ID[clients_counter - 1]);
                } else {
                    printf("Server is full, Client %d didn't connect\n", message.pid);
                    strcpy(message.text, "You didn't connect to server");
                    send_message(message, tmp);
                    mq_close(tmp);
                }
                break;
            case ECHO:
                printf("Got 'echo' from Client %d\n", message.pid);
                send_message(message, clients_queue_ID[message.ID]);
                break;
            case UPPER:
                printf("Got 'upper' from Client %d\n", message.pid);
                convert_to_upper_case(message.text);
                send_message(message, clients_queue_ID[message.ID]);
                break;
            case TIME:
                printf("Got 'time' from Client %d\n", message.pid);
                set_time(message.text);
                send_message(message, clients_queue_ID[message.ID]);
                break;
            case TERMINATE:
                printf("Got 'terminate' from Client %d\n", message.pid);
                exit(0);
            case WRONG:
                printf("Got wrong command from Client %d\n", message.pid);
                strcpy(message.text, "Wrong command");
                send_message(message, clients_queue_ID[message.ID]);
                break;
            default:
                printf("default from Client %d\n", message.pid);
                break;
        }
    }

    return 0;
}

int open_queue() {
    struct mq_attr attr;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = MAX_QUEUE_SIZE;
    return mq_open(server_queue_name, O_CREAT | O_RDONLY, 0600, &attr);
}

void close_queue() {
    int i;
    for (i = 0; i < clients_counter; ++i) {
        mq_close(clients_queue_ID[i]);
    }
    mq_close(server_queue_ID);
    mq_unlink(server_queue_name);
}

void sig_handler(int signum) {
    printf("Caught signal %d, closing queue and coming out...\n", signum);
    close_queue();
    exit(1);
}

void send_message(Message message, int name) {
    if (mq_send(name, (char *) &message, MSG_SIZE, 0) == -1) {
        perror("Could not send message");
        exit(1);
    }
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

int open_client_queue(Message message) {
    char name[SIZE];
    sprintf(name, "/client_%d", message.pid);
    return mq_open(name, O_WRONLY);
}