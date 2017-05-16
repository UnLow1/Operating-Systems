#ifndef EX02_HEADER_H
#define EX02_HEADER_H

typedef struct MyQueue MyQueue;

struct MyQueue {
    int max_size;
    int actual_size;
    pid_t chair;
    pid_t queue[100];
};

#endif //EX02_HEADER_H
