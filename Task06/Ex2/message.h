#ifndef EX1_MESSAGE_H
#define EX1_MESSAGE_H

#define MSG_SIZE (sizeof(struct Message))
#define MAX_CLIENTS 2
#define MAX_QUEUE_SIZE 10
#define SIZE 100
#define SERVER_QUEUE_NAME "/server"

typedef struct Message {
    long type;
    pid_t pid;
    char text[SIZE];
    int ID;
} Message;

typedef enum Message_Type {
    CONNECT = 1,
    CONNECTED,
    ECHO,
    UPPER,
    TIME,
    TERMINATE,
    WRONG
} Message_Type;

#endif //EX1_MESSAGE_H
