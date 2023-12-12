#include "server.h"

char logFilename[256];
char configFilename[256];

int port;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect arguments input! It should be ./%s configurationsFilename\n");
        return 1;
    }

    int serverSocket;
    int clientSocket;

    int sharedMemoryId;
    MainData* sharedMemoryPointer;

    int semaphoreId;

    //====================================================

    configureSignalProcessing();

    strcpy(configFilename, argv[1]);
    parseConfig(configFilename, logFilename, &port);

    createAndConfigureSocket(&serverSocket, port);

    // becomeDaemon(logFilename);

    createSharedMemory(&sharedMemoryId);
    connectSharedMemory(&sharedMemoryPointer, sharedMemoryId);
    initializeSharedMemory(sharedMemoryPointer);

    createSemaphore(&semaphoreId);
    initializeSemaphore(semaphoreId);

    //core
    {
        printf("Waiting for connection\n");
        clientSocket = awaitForClientSocket(serverSocket);
        
        int gameStartedFlag = 0;
        int overallMisses;
        MainData data;
        while (1) {
            char buffer[256];
            int x = 0;
            int y = 0;
            Command command;

            clearMessage(data.message);
            read(clientSocket, &command, sizeof(command));
            switch (decypherCommand(command.body, command.arguments, &x, &y)) {
                case SHOT:
                    if (!gameStartedFlag) {
                        strcpy(data.message, "Field is not generated yet!\n"); 
                        break;
                    }

                    shot(&data, x, y);

                    if (   data.missesLeft <= 0
                        || checkIfWin(&data)) {
                        strcpy(data.message, "It is the end of the game! To start new game enter \"battle {x}\"\n");
                        gameStartedFlag = 0;
                        break;
                    }
                    break;
                
                case NEW_BATTLE:
                    startGame(&gameStartedFlag, &data);
                    data.missesLeft = x;
                    break;

                default:
                    fprintf(stderr, "Unknown command\n");
                    break;
            }

            write(clientSocket, &data, sizeof(MainData));
        }
    }

    const char *battleStarted = "SEABATTLE STARTED!";
    write(clientSocket, battleStarted, sizeof(battleStarted));

    turnOffSharedMemory(sharedMemoryPointer);

    deleteSemaphore(semaphoreId);
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

//Terminate and send result to client (TODO)
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

//Terminate and delete shared memory (TODO)
void processSIGINT() {
    // TODO
    // if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    //     perror("shmctl");
    //     return 1;
    // }

    // printf("\nSIGINT has been called\n");
    // return 0;
}

void createAndConfigureSocket(int *serverSocket, int port) {
    if ((*serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Socket establishment");
        exit(1);
    }

    InternetSocketAddress serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);       //convert to internet port correctly
    serverAddress.sin_addr.s_addr = INADDR_ANY; //bind to all local interfaces
    if (bind(*serverSocket, (AbstractSocketAddress*) &serverAddress, (socklen_t) sizeof(serverAddress)) == -1) {
        perror("Error on binding socket with address");
        exit(1);
    }

    if (listen(*serverSocket, MAX_PENDING_CONNECTIONS) == -1) {
        perror("Error on listening to upcoming connections");
        exit(1);
    }
}

void becomeDaemon(char* logFilename) {
    chdir("/");
    if (fork()) exit(0);
    setsid();

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int loggingDescriptor = open(logFilename, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (loggingDescriptor == -1) {
        perror("Error on open logging file");
        exit(1);
    }

    dup2(loggingDescriptor, STDIN_FILENO);
    dup2(loggingDescriptor, STDOUT_FILENO);
    dup2(loggingDescriptor, STDERR_FILENO);

    close(loggingDescriptor);
}

void createSharedMemory(int *sharedMemoryId) {
    *sharedMemoryId = shmget(IPC_PRIVATE, sizeof(MainData), IPC_CREAT | 0666);
    if (*sharedMemoryId == -1) {
        perror("Error on creating shared memory id");
        exit(1);
    }
}

void connectSharedMemory(MainData **sharedMemoryPointer, int sharedMemoryId) {
    //Gets random free segment
    *sharedMemoryPointer = (MainData*)shmat(sharedMemoryId, NULL, 0);
    if (*sharedMemoryPointer == (MainData*) - 1) {
        perror("Error on connecting shared memory");
        exit(1);
    }
}

void turnOffSharedMemory(MainData* sharedMemoryPointer) {
    if (shmdt(sharedMemoryPointer) == -1) {
        perror("Error on turning off shared memory");
        exit(1);
    }
}

void deleteSharedMemory(int sharedMemoryId) {
    if (shmctl(sharedMemoryId, IPC_RMID, NULL) == -1) {
        perror("Error on deleting shared memory");
        exit(1);
    }
}

void initializeSharedMemory(MainData* sharedMemoryPointer) {
    clearData(sharedMemoryPointer);
}

void clearData(MainData* data) {
    clearMessage(data->message);
    clearField(data->mask);
    clearField(data->field);

    data->missesLeft = 0;
}

void createSemaphore(int *semaphoreId) {
    *semaphoreId = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (*semaphoreId == -1) {
        perror("Error on creating semaphore id");
        exit(1);
    }
}

void initializeSemaphore(int semaphoreId) {
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
        exit(1);
    }
}

void deleteSemaphore(int semaphoreId) {
    if (semctl(semaphoreId, 0, IPC_RMID) == -1) {
        perror("Error on deleting semaphore");
        exit(1);
    }
}

int awaitForClientSocket(int serverSocket) {
    int clientSocket;
    InternetSocketAddress clientAddress;
    clientAddress.sin_family = AF_INET;
    socklen_t clientAddressLength = sizeof(clientAddress);
    if ((clientSocket = accept(serverSocket, (AbstractSocketAddress*) &clientAddress, &clientAddressLength)) == -1) {
        perror("Error occured on accepting upcoming connections");
        exit(1);
    }

    return clientSocket;
}

void startGame(int *gameStartedFlag, MainData *data) {
    clearData(data);

    strcpy(data->message, "The game was started!\n");
    generateNewField(data->field);
    *gameStartedFlag = 1;
}

void clearMessage(char message[MESSAGE_LENGTH]) {
    for (int i = 0; i < MESSAGE_LENGTH; i++) {
        message[i] = '\0';
    }
}

// void createWorkerProcess() {
//     pid_t workerPid = fork();
//     if (workerPid < 0) {
//         perror("Error on fork");
//         exit(1);
//     } else
//     if (workerPid == 0) {
//         //parent process

//     }
// }