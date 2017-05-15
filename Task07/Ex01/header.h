#ifndef EX01_HEADER_H
#define EX01_HEADER_H

#define PROJ_ID 1
#define PATH "."

typedef struct MyQueue MyQueue;

struct MyQueue {
    int max_size;
    int actual_size;
    pid_t chair;
    pid_t queue[100];
};

//struct sembuf *buffer;

//void semaphore(unsigned short num, short op, short flg, struct sembuf *buffer, int semID);
#endif //EX01_HEADER_H
