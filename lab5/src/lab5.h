#ifndef LAB5_H
#define LAB5_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct msqid_ds MessageQueue;

typedef struct {
    long type;
    char data[32];
} Message;

typedef struct {
    long type;
    char data[2];
} ShortMessage;

void printMessageQueueInfo(int identifier) {
    MessageQueue queue;
    if (msgctl(identifier, IPC_STAT, &queue) == -1) perror("Cannot read statistics of message queue");
    else {
        printf("\nKey: %d\n", queue.msg_perm.__key);
        printf("UID: %d\n", queue.msg_perm.uid);
        printf("GID: %d\n", queue.msg_perm.gid);
        printf("MODE(decimal): %d\n", queue.msg_perm.mode);
        printf("Creation time: %d\n", queue.msg_ctime);
        printf("Last recieve time: %d\n", queue.msg_rtime);
        printf("Last send time: %d\n", queue.msg_stime);
        printf("Last recieve PID: %d\n", queue.msg_lrpid);
        printf("Last sent PID: %d\n", queue.msg_lspid);
        printf("Amount of messages: %d\n", queue.msg_qnum);
        printf("Size of queue in bytes: %d\n", queue.msg_qbytes);
    }
}

#endif