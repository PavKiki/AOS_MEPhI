#include "server.h"

char logFilename[256];
char configFilename[256];

int port;

int serverSocket;
int clientSocket;

int sharedMemoryId;

int semaphoreId;

int messageQueueId;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect arguments input! It should be ./%s configurationsFilename\n");
        return 1;
    }

    MainData* sharedMemoryPointer;


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

    createMessageQueue(&messageQueueId);

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
                case SHOT: {
                        if (!gameStartedFlag) {
                        strcpy(data.message, "Field is not generated yet!\n"); 
                        break;
                    }

                    makeShot(&data, x, y);

                    if (   data.missesLeft <= 0
                        || checkIfWin(&data)) {

                        strcpy(data.message, "It is the end of the game! To start new game enter \"battle {x}\"\n");
                        gameStartedFlag = 0;
                        break;
                    }

                    break;
                }

                case NEW_BATTLE:
                    startGame(&gameStartedFlag, &data, clientSocket);
                    data.missesLeft = x;
                    break;

                default:
                    strcpy(data.message, "Unknown command");
                    fprintf(stderr, "Unknown command");
                    break;
            }

            write(clientSocket, &data, sizeof(MainData));
        }
    }

    const char *battleStarted = "SEABATTLE STARTED!";
    write(clientSocket, battleStarted, sizeof(battleStarted));

    turnOffSharedMemory(sharedMemoryPointer);

    deleteSemaphore(semaphoreId);

    deleteMessageQueue(messageQueueId);
}

void configureSignalProcessing() {
    signal(SIGTERM, customSignalHandler);
    signal(SIGHUP, customSignalHandler);
}

void customSignalHandler(int signum) {
    switch (signum) {
    case SIGTERM:
        processSIGTERM();
    case SIGHUP:
        processSIGHUP();
    }
}

//Terminate and send result to client
void processSIGTERM() {
    MainData* data = malloc(sizeof(MainData));
    connectSharedMemory(&data, sharedMemoryId);

    write(clientSocket, data, sizeof(MainData));

    if (shmctl(sharedMemoryId, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    close(clientSocket);
    close(serverSocket);
    
    printf("\nSIGTERM has been called\n");
    exit(0);
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
    if (fork()) {
        // wait(NULL);
        exit(0);
    }

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

void captureSemaphore(int semaphoreId) {
    struct sembuf op = {0, -1, 0};
    if (semop(semaphoreId, &op, 1) == -1) {
        perror("Error on capture semaphore");
        exit(1);
    }
}

void freeSemaphore(int semaphore) {
    struct sembuf op = {0, 1, 0};
    if (semop(semaphoreId, &op, 1) == -1) {
        perror("Error on free semaphore");
        exit(1);
    }
}

void deleteSemaphore(int semaphoreId) {
    if (semctl(semaphoreId, 0, IPC_RMID) == -1) {
        perror("Error on deleting semaphore");
        exit(1);
    }
}

void createMessageQueue(int *messageQueueId) {
    *messageQueueId = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (*messageQueueId == -1) {
        perror("Error on creating message queue");
        exit(1);
    }
}

void sendMessage(int x, int y, int misses, int type) {
    Message msg;
    msg.type = type;
    msg.info.x = x;
    msg.info.y = y;
    msg.info.misses = misses;
    if (msgsnd(messageQueueId, &msg, sizeof(CoordinatesAndMisses), 0) == -1) {
        perror("Error on sending a message\n");
        exit(1);
    }
}

void recieveMessage(Message *msg, int type) {
    if (msgrcv(messageQueueId, msg, sizeof(CoordinatesAndMisses), type, 0) == -1) {
        perror("Error on recieving a message\n");
        exit(1);
    }
}

void deleteMessageQueue(int messageQueueId) {
    if (msgctl(messageQueueId, IPC_RMID, NULL) == -1) {
        perror("Error on deleting message queue");
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

void startGame(int *gameStartedFlag, MainData *data, int clientSocket) {
    clearData(data);

    strcpy(data->message, "The game was started!\n");

    createWorkerToGenerateField(data, clientSocket);
    
    *gameStartedFlag = 1;
}

void makeShot(MainData *data, int x, int y) {
    sendMessage(x, y, data->missesLeft, TO_PROCESSING);

    createWorkerToProcessShot(data);

    Message recievedMessage;
    recieveMessage(&recievedMessage, FROM_PROCESSING);

    (data->mask)[x][y] = 1;
    data->missesLeft = recievedMessage.info.misses;
}

void createWorkerToGenerateField(MainData *data, int clientSocket) {
    pid_t childPid = fork();
    
    if (childPid) {
    //master process
        waitpid(childPid, NULL, 0);
        captureSemaphore(semaphoreId);
        
        MainData* ptr;
        connectSharedMemory(&ptr, sharedMemoryId);
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                data->field[i][j] = ptr->field[i][j];
            }
        }

        freeSemaphore(semaphoreId);
    } else {
    //worker process
        int newField[FIELD_SIZE][FIELD_SIZE];
        generateNewField(newField);

        captureSemaphore(semaphoreId);
        
        MainData* ptr;
        connectSharedMemory(&ptr, sharedMemoryId);
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                ptr->field[i][j] = newField[i][j];
            }
        }

        freeSemaphore(semaphoreId);

        close(clientSocket);
        close(serverSocket);
        exit(0);
    }
}

void createWorkerToProcessShot(MainData *data) {
    pid_t childPid = fork();
    if (childPid) {
    //master process
        waitpid(childPid, NULL, 0);
    } else {
    //worker process
        Message recievedMsg;
        recieveMessage(&recievedMsg, TO_PROCESSING);

        Message msgToSend;
        int newMisses = recievedMsg.info.misses;
        if (!checkHit(data, recievedMsg.info.x, recievedMsg.info.y)) {
            newMisses--;
        }

        sendMessage(recievedMsg.info.x, recievedMsg.info.y, newMisses, FROM_PROCESSING);

        close(serverSocket);
        close(clientSocket);
        exit(0);
    }
}

void clearMessage(char message[MESSAGE_LENGTH]) {
    for (int i = 0; i < MESSAGE_LENGTH; i++) {
        message[i] = '\0';
    }
}
