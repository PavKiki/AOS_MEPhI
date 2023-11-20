#include "task9.h"

// 9. Модифицировать программы п. 6 так, чтобы использовались две очереди: одна для запросов к серверу и одна для всех ответов от сервера к любому клиенту.

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

    int clientIdentifier;
    if ((clientIdentifier = getClientIdentifier()) == -1) {
        return;
    }

    int counter = 0;
    while(!isTerminated) {
        sleep(1);

        if (sendRequestToServer(counter, serverIdentifier, clientIdentifier) == -1) {
            printf("Something went wrong on sending request to the server\n");
        } else {
            counter++;
        }

        if (readResponseFromServer(clientIdentifier) == -1) {
            continue;
        }
    }
    msgctl(clientIdentifier, IPC_RMID, NULL);
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

int getClientIdentifier() {
    key_t clientKey = ftok("/", 'C');

    int clientIdentifier;
    if ((clientIdentifier = msgget(clientKey, 0666 | IPC_CREAT)) == -1) {
        perror("Error on creating identifier");
    }

    return clientIdentifier;
}

void serverProcess() {
    int identifier;
    if ((identifier = getServerIdentifier()) == -1) {
        return;
    }
    
    while(!isTerminated) {
        sleep(1);

        Message msg;
        int bytesRead = msgrcv(identifier, &msg, sizeof(msg.data), 1, MSG_NOERROR | IPC_NOWAIT);
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

    if (msgsnd(getClientIdentifier(), &msg, sizeof(message), IPC_NOWAIT) == -1) {
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