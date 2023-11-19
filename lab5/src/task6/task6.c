#include "task6.h"

// 6. Написать программы для работы с очередями сообщений в соответствии с моделью клиент-сервер: каждый процесс использует собственную очередь сообщений. 
// Процесс-сервер читает запросы из своей очереди сообщений и посылает ответ процессам-клиентам в их очереди. 
// Процессы-клиенты читают ответ и выводят его в выходной поток. 
// Процессы-клиенты должны передавать процессу-серверу информацию о своих очередях сообщений (куда записывать ответ).

int main(int argc, char** argv) {
    if (argc != 2) {
        return -1;
    }

    if (!strcmp(argv[1], "-c")) {
        printf("Client was launched\n\n");
        clientProcess();
    } else {
        printf("Server was launched\n\n");
        serverProcess();
    }

    return 0;
}

void clientProcess() {
    int serverIdentifier;
    if ((serverIdentifier = getServerIdentifier()) == -1) {
        return;
    }

    int clientIdentifier;
    if ((clientIdentifier = getClientIdentifier()) == -1) {
        return;
    }

    int counter;
    while(1) {
        sleep(1);

        if (sendRequestToServer(counter, serverIdentifier, clientIdentifier) == -1) {
            printf("Something went wrong on sending request to the server\n");
        }

        if (readResponseFromServer(clientIdentifier) == -1) {
            printf("Something went wrong on recieving response\n");
        }
    }
}

int sendRequestToServer(int counter, int serverIdentifier, int clientIdentifier) {
    Message msg;
    msg.type = 1;
    char tmpBuf[3];
    sprintf(tmpBuf, "%d", clientIdentifier);
    strcat(msg.data, tmpBuf);
    strcat(msg.data, "Hello from client #");
    sprintf(tmpBuf, "%3d", counter);
    strcat(msg.data, tmpBuf);
    if (msgsnd(serverIdentifier, &msg, sizeof(msg.data), IPC_NOWAIT) == -1) {
        return -1;
    }

    return 0;
}

int readResponseFromServer(int clientIdentifier) {
    Message msg;
    if (msgrcv(clientIdentifier, &msg, sizeof(msg.data), 2, IPC_NOWAIT) == -1) {
        return -1;
    }

    printf("[CLIENT] %s\n", msg.data);
}

int getClientIdentifier() {
    int clientIdentifier;
    if ((clientIdentifier = msgget(IPC_PRIVATE, 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        perror("Error on creating identifier");
    }

    return clientIdentifier;
}

void serverProcess() {
    int identifier;
    if ((identifier = getServerIdentifier()) == -1) {
        return;
    }
    
    while(1) {
        //clean queue
        // Message msg;
        // int bytesRead = msgrcv(identifier, &msg, sizeof(msg.data), 1, MSG_NOERROR | IPC_NOWAIT);

        sleep(1);

        Message msg;
        int bytesRead = msgrcv(identifier, &msg, sizeof(msg.data), 1, MSG_NOERROR | IPC_NOWAIT);
        if (bytesRead == -1) {
            printf("Error on receiving message\n");
            continue;
        } else 
        if (bytesRead <= 3) {
            printf("Cannot read client identifier\n");
            continue;
        } else {
            int clientIdentifier = getClientIdentifierFromMessage(msg.data);
            char *clientMessage = getMessageFromClient(msg.data);

            sendResponse(clientIdentifier, strcat("It is an answer on ", clientMessage));
        }
    }
}

int getServerIdentifier() {
    key_t serverKey = ftok("/", 'S');

    int identifier;
    if ((identifier = msgget(serverKey, (IPC_CREAT | 0666))) == -1) {
        perror("Error on creating server identifier: ");
    }

    return identifier;
}

int sendResponse(int clientIdentifier, char* message) {
    Message msg;
    msg.type = 2;
    strcat(msg.data, message);

    if (msgsnd(clientIdentifier, &msg, sizeof(message), IPC_NOWAIT) == -1) {
        printf("Error occured while sending a response\n");
    }
}

int getClientIdentifierFromMessage(char* messageData) {
    char stringClientId[3];
    for (int i = 0; i < 3; i++) {
        stringClientId[i] = messageData[i];
    }

    return atoi(stringClientId);
}

char* getMessageFromClient(char* messageData) {
    int messageSize = sizeof(messageData) - 3;
    char* clientMessage = calloc(messageSize, sizeof(char));
    for (int i = 0; i < messageSize; i++) {
        clientMessage[i] = messageData[i + 3];
    }

    return clientMessage;
}