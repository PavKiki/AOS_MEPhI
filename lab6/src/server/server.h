#ifndef SERVER_H
#define SERVER_H

#include "../../lab6.h"
#include "config/read_config.h"
#include "../seabattle/seabattle.h"

int main(int argc, char **argv);

void configureSignalProcessing();
void customSignalHandler(int signum);
void processSIGTERM();
void processSIGHUP();
void processSIGINT();

void createAndConfigureSocket(int *serverSocket, int port);

void becomeDaemon(char *logFilename);

void createSharedMemory(int *sharedMemoryId);
void connectSharedMemory(MainData **sharedMemoryPointer, int sharedMemoryId);
void initializeSharedMemory(MainData* sharedMemoryPointer);
void turnOffSharedMemory(MainData *sharedMemoryPointer);
void deleteSharedMemory(int sharedMemoryId);

void createSemaphore(int *semaphoreId);
void initializeSemaphore(int semaphoreId);
void deleteSemaphore(int semaphoreId);

int awaitForClientSocket(int serverSocket);

void startGame(int *gameStartedFlag, MainData *data);

void clearData(MainData* data);
void clearMessage(char message[MESSAGE_LENGTH]);

#endif