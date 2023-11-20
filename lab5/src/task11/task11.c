#include "task11.h"

// 11. Написать программу, иллюстрирующую возможность синхронизации процессов при помощи очередей сообщений.

int main(int argc, char** argv) {
    int identifier = msgget(IPC_PRIVATE, 0666 | IPC_CREAT | IPC_EXCL);

    pid_t pid = fork();
    switch (pid) {
        case 0: {
            /* child */
            for (int i = 0; i < 10000000; i++) {
                if (i % 10 == 0) {
                    printf("%d\n", i);
                }
            }

            Message msg;
            msg.type = 1;
            strcat(msg.data, "The end of the execution!\n");
            msgsnd(identifier, &msg, sizeof(msg.data), 0);

            printf("%s\n", msg.data);
            
            break;
        }
        
        default: {
            /* parent */
            Message msg;
            msgrcv(identifier, &msg, sizeof(msg.data), 1, MSG_NOERROR);
            
            printf("%s\n", msg.data);

            wait(NULL);
            break;
        }
    } 

    return 0;
}