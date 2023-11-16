#include "task1.h"

// 1. 
// Создать очередь сообщений или получить доступ к уже существующей очереди. 
// Вывести на экран статистическую информацию об очереди сообщений. 
// Поместить несколько сообщений различных типов в очередь.

typedef struct msqid_ds MessageQueue;

typedef struct {
    long type;
    char data[32];
} Message;

int main() {
    int identifier;
    //IPC_PRIVATE - every time new resource and identifier
    //IPC_EXCL - to take care if identifier already exists
    if ((identifier = msgget(IPC_PRIVATE, 0666 | IPC_CREAT | IPC_EXCL)) == -1) perror("Cannot create new identifier");
    else printf("New identifier is %d\n", identifier);

    MessageQueue queue;
    if (msgctl(identifier, IPC_STAT, &queue) == -1) perror("Cannot read statistics of message queue");
    printf("Key: %d\n", queue.msg_perm.__key);
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

    for (size_t i = 1; i <= 10; i++) {
        Message msg;
        msg.type = i;
        sprintf(msg.data, "Hello from type %d!", i);

        //IPC_NOWAIT is set, so on running out of buffer msgsnd will be blocked
        if (msgsnd(identifier, &msg, sizeof(msg.data), IPC_NOWAIT) == -1) perror("Error on sending message to the queue");
    }
}