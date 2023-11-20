#include "task10.h"

// 10. Еще раз модифицировать эти программы так, чтобы использовалась только одна очередь сообщений и для запросов к серверу, и для всех ответов.

int isTerminated = 0;

int main(int argc, char** argv) {
    signal(SIGINT, customHandler);

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

void customHandler() {
    isTerminated++;
}

void clientProcess() {
    int serverIdentifier;
    if ((serverIdentifier = getServerIdentifier()) == -1) {
        return;
    }

    int counter = 0;
    while(!isTerminated) {
        sleep(1);

        if (sendRequestToServer(counter, serverIdentifier, serverIdentifier) == -1) {
            printf("Something went wrong on sending request to the server\n");
        } else {
            counter++;
        }

        if (readResponseFromServer(serverIdentifier) == -1) {
            continue;
        }
    }
    msgctl(serverIdentifier, IPC_RMID, NULL);
}

int sendRequestToServer(int counter, int serverIdentifier, int clientIdentifier) {
    Message msg;
    msg.type = 1;
    char tmpBuf[64];
    sprintf(tmpBuf, "%d_", getpid());
    strcat(msg.data, tmpBuf);
    strcat(msg.data, "Hello from client ");

    char newTmpBuf[64];
    sprintf(newTmpBuf, "#%d", counter);
    strcat(msg.data, newTmpBuf);
    if (msgsnd(serverIdentifier, &msg, sizeof(msg.data), 0) == -1) {
        return -1;
    }

    return 0;
}

int readResponseFromServer(int clientIdentifier) {
    Message msg;
    if (msgrcv(clientIdentifier, &msg, sizeof(msg.data), getpid(), 0) == -1) {
        perror("Error on reading response ");
        return -1;
    }

    printf("[CLIENT] %s\n", msg.data);
}

void serverProcess() {
    int identifier;
    if ((identifier = getServerIdentifier()) == -1) {
        return;
    }
    
    while(!isTerminated) {
        sleep(1);

        Message msg;
        int bytesRead = msgrcv(identifier, &msg, sizeof(msg.data), 1, MSG_NOERROR);
        if (bytesRead == -1) {
            perror("Error on receiving message");
            continue;
        } else 
        if (bytesRead <= 3) {
            printf("Cannot read client identifier\n");
            continue;
        } else {
            char* stringClientIdentifier = getClientStringIdentifierFromMessage(&msg);
            int clientIdentifier = atoi(stringClientIdentifier);

            sendResponse(clientIdentifier, msg.data);
            printf("[SERVER] %s was sent to the %d\n", msg.data, clientIdentifier);
        }
    }
    msgctl(identifier, IPC_RMID, NULL);
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
    msg.type = clientIdentifier;
    strcat(msg.data, message);

    if (msgsnd(getServerIdentifier(), &msg, sizeof(message), 0) == -1) {
        printf("Error occured while sending a response\n");
    }
}

char* getClientStringIdentifierFromMessage(Message* msg) {
    char* stringClientId = calloc(64, sizeof(char));
    for (char* ptr = msg->data, i = 0; ptr != NULL && *ptr != '_'; ptr += 1, i++) {
        stringClientId[i] = (msg->data)[i];
    }

    return stringClientId;    
}