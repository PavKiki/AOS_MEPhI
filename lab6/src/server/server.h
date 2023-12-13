#ifndef SERVER_H
#define SERVER_H

#include "../../lab6.h"
#include "config/read_config.h"
#include "../seabattle/seabattle.h"

#define TO_PROCESSING 1
#define FROM_PROCESSING 2

int main(int argc, char **argv);

void configureSignalProcessing();
void customSignalHandler(int signum);
void processSIGHUP();
void processSIGTERM();

void createAndConfigureSocket(int *serverSocket, int port);

void becomeDaemon(char *logFilename);

void createSharedMemory(int *sharedMemoryId);
void connectSharedMemory(MainData **sharedMemoryPointer, int sharedMemoryId);
void initializeSharedMemory(MainData* sharedMemoryPointer);
void turnOffSharedMemory(MainData *sharedMemoryPointer);
void deleteSharedMemory(int sharedMemoryId);

void createSemaphore(int *semaphoreId);
void initializeSemaphore(int semaphoreId);
void captureSemaphore(int semaphoreId);
void freeSemaphore(int semaphore);
void deleteSemaphore(int semaphoreId);

void createMessageQueue(int *messageQueueId);
void sendMessage(int x, int y, int misses, int type);
void recieveMessage(Message *message, int type);
void deleteMessageQueue(int messageQueueId);

int awaitForClientSocket(int serverSocket);

void createWorkerToGenerateField(MainData *data, int clientSocket);
void createWorkerToProcessShot(MainData *data);

void startGame(int *gameStartedFlag, MainData *data, int clientSocket);
void makeShot(MainData *data, int x, int y);

void clearData(MainData* data);
void clearMessage(char message[MESSAGE_LENGTH]);

#endif