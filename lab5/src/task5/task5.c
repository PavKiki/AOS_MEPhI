#include "task5.h"

// 5.
// Написать программу, позволяющую удалять очереди сообщений по идентификатору. 
// Обработать возможные ошибочные ситуации.

int main(int argc, char** argv) {
    int identifier = atoi(argv[1]);
    printMessageQueueInfo(identifier);

    if (msgctl(identifier, IPC_RMID, NULL) == -1) {
        //possible cases according to https://www.opennet.ru/man.shtml?topic=msgctl&category=2&russian=0
        switch (errno) {
            case EIDRM:
                printf("Successfully deleted queue with identifier %d\n", identifier);
                break;
                
            case EINVAL:
                printf("Wrong identifier: %d\n", identifier);
                break;
            case EPERM:
                printf("No permissions to access message queue with identifier %d\n", identifier);
                break;
            default:
                printf("Non-processable mistake");
                break;
        }
    }

    
}