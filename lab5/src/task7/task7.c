#include "task7.h"

// 7. Осуществить при помощи программ п. 6. обмен сообщениями между несколькими пользователями, при условии, что каждому пользователю предназначены сообщения любого или определенного типа (по договоренности). 
// Будут ли примеры с очередями сообщений "приводить себя в порядок"? 
// Что произойдет, если прервать процесс-сервер с помощью клавиши CTRL-C?

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

    int counter = 0;
    while(1) {
        sleep(1);

        if (sendRequestToServer(counter++, serverIdentifier, clientIdentifier) == -1) {
            printf("Something went wrong on sending request to the server\n");
        }

        if (readResponseFromServer(clientIdentifier) == -1) {
            continue;
        }
    }
    //the queue will be undeleted, nothing good
}

int sendRequestToServer(int counter, int serverIdentifier, int clientIdentifier) {
    Message msg;
    msg.type = 1;
    char tmpBuf[64];
    sprintf(tmpBuf, "%d_", clientIdentifier);
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
    if (msgrcv(clientIdentifier, &msg, sizeof(msg.data), 2, 0) == -1) {
        perror("Error on reading response ");
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
        }
    }
    //the queue will be undeleted, nothing good
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

char* getClientStringIdentifierFromMessage(Message* msg) {
    char* stringClientId = calloc(64, sizeof(char));
    for (char* ptr = msg->data, i = 0; ptr != NULL && *ptr != '_'; ptr += 1, i++) {
        stringClientId[i] = (msg->data)[i];
    }

    return stringClientId;    
}