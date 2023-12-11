#include "server.h"

int errorCode;

char logFilename[256];
char configFilename[256];

int port;

int serverSocket;
// int clientSocket;
int clientSockets[MAX_CLIENT_SOCKETS];

int sharedMemoryId;
MainData* sharedMemoryPointer;

int semaphoreId;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect arguments input! It should be ./%s configurationsFilename\n");
        return 1;
    }

    configureSignalProcessing();

    strcpy(configFilename, argv[1]);
    parseConfig(configFilename, logFilename, &port);

    if (errorCode = createAndConfigureSocket(&serverSocket, port) != 0) {
        return errorCode;
    }

    if (errorCode = becomeDaemon(logFilename) != 0) {
        return errorCode;
    }

    if (errorCode = createSharedMemory(&sharedMemoryId) != 0) {
        return errorCode;
    }
    if (errorCode = connectSharedMemory(sharedMemoryId) != 0) {
        return errorCode;
    }
    initializeSharedMemory(sharedMemoryPointer);

    if (errorCode = createSemaphore(&semaphoreId) != 0) {
        return errorCode;
    }
    if (errorCode = initializeSemaphore(semaphoreId) != 0) {
        return errorCode;
    }


}

void configureSignalProcessing() {
    signal(SIGTERM, customSignalHandler);
    signal(SIGHUP, customSignalHandler);
    signal(SIGINT, customSignalHandler);
}

void customSignalHandler(int signum) {
    switch (signum) {
    case SIGTERM:
        return processSIGTERM();
    case SIGHUP:
        return processSIGHUP();
    case SIGINT:
        return processSIGINT();
    }
}

//Terminate and send result to client
void processSIGTERM() {
    // TODO
    // for (int i = 0; i < 20; i++) {
    //     if (client_socks[i] != -1) {
    //         handle_prog(client_socks[i], server_sock, semid);
    //         client_socks[i] = -1;
    //         close(client_socks[i]);
    //     }
    // }

    // printf("\nSIGTERM has been called\n");
}

//Re-read config file
void processSIGHUP() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("Error on blocking SIGHUP signal");
    }

    parseConfig(configFilename, logFilename, &port);

    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
        perror("Error on unblocking SIGHUP signal");
    }
    
    printf("\nSIGHUP has been called\n");
}

//Terminate and delete shared memory
void processSIGINT() {
    // TODO
    // if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    //     perror("shmctl");
    //     return 1;
    // }

    // printf("\nSIGINT has been called\n");
    // return 0;
}

int createAndConfigureSocket(int *serverSocket, int port) {
    if ((*serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Socket establishment");
        return 1;
    }

    InternetSocketAddress serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);       //convert to internet port correctly
    serverAddress.sin_addr.s_addr = INADDR_ANY; //bind to all local interfaces
    if (bind(*serverSocket, (AbstractSocketAddress*) &serverAddress, (socklen_t) sizeof(serverAddress)) == -1) {
        perror("Error on binding socket with address");
        return 1;
    }

    if (listen(*serverSocket, MAX_PENDING_CONNECTIONS) == -1) {
        perror("Error on listening to upcoming connections");
        return 1;
    }

    // int currentClientSocket;
    // InternetSocketAddress clientAddress;
    // clientAddress.sin_family = AF_INET;
    // socklen_t clientAddressLength = sizeof(clientAddress);
    // if ((currentClientSocket = accept(*serverSocket, (AbstractSocketAddress*) &clientAddress, &clientAddressLength)) == -1) {
    //     perror("Error occured on accepting upcoming connections");
    //     return 1;
    // }

    return 0;
}

int becomeDaemon(char* logFilename) {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int loggingDescriptor = open(logFilename, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (loggingDescriptor == -1) {
        perror("Error on open logging file");
        return 1;
    }

    dup2(loggingDescriptor, STDIN_FILENO);
    dup2(loggingDescriptor, STDOUT_FILENO);
    dup2(loggingDescriptor, STDERR_FILENO);

    close(loggingDescriptor);

    return 0;
}

int createSharedMemory(int *sharedMemoryId) {
    *sharedMemoryId = shmget(IPC_PRIVATE, sizeof(MainData), IPC_CREAT | 0666);
    if (*sharedMemoryId == -1) {
        perror("Error on creating shared memory id");
        return 1;
    }

    return 0;
}

int connectSharedMemory(int sharedMemoryId) {
    //Gets random free segment
    sharedMemoryPointer = (MainData*)shmat(sharedMemoryId, NULL, 0);
    if (sharedMemoryPointer == (MainData*) - 1) {
        perror("Error on connecting shared memory");
        return 1;
    }

    return 0;
}

void initializeSharedMemory(MainData* sharedMemoryPointer) {
    sharedMemoryPointer->clients = 0;

    //Figure out why i need this
    for (int i = 0; i < MAX_CLIENT_SOCKETS; i++) {
        clientSockets[i] = -1;
    }
}

int createSemaphore(int *semaphoreId) {
    *semaphoreId = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (*semaphoreId == -1) {
        perror("Error on creating semaphore id");
        return 1;
    }

    return 0;
}

int initializeSemaphore(int semaphoreId) {
    /* according to X/OPEN we have to define it ourselves */
    union semun {
        int val;                  /* value for SETVAL */
        struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
        unsigned short *array;    /* array for GETALL, SETALL */
                                    /* Linux specific part: */
        struct seminfo *__buf;    /* buffer for IPC_INFO */
    } arg;

    //0 - free, 1 - locked
    arg.val = 1;
    if (semctl(semaphoreId, 0, SETVAL, arg) == -1) {
        perror("Error on initializing semaphore");
        return 1;
    }

    return 0;
}