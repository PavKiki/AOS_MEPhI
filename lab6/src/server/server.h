#ifndef SERVER_H
#define SERVER_H

#include "../../lab6.h"
#include "config/read_config.h"

int main(int argc, char** argv);

void configureSignalProcessing();
void customSignalHandler(int signum);
void processSIGTERM();
void processSIGHUP();
void processSIGINT();

int createAndConfigureSocket(int *serverSocket, int port);

int becomeDaemon(char* logFilename);

int createSharedMemory(int *sharedMemoryId);
int connectSharedMemory(int sharedMemoryId);
void initializeSharedMemory(MainData* sharedMemoryPointer);

int createSemaphore(int *semaphoreId);
int initializeSemaphore(int semaphoreId);

#endif