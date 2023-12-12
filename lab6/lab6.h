#ifndef LAB6_H
#define LAB6_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>

#define MAX_CLIENTS 10

#define MAX_PENDING_CONNECTIONS 10

#define MESSAGE_LENGTH 256

#define PROBABILITY_OF_GENERATING_SHIP 5 //0 to 10

#define FIELD_SIZE 10
#define SHOT 2
#define NEW_BATTLE 3

typedef struct sockaddr AbstractSocketAddress;
typedef struct sockaddr_in InternetSocketAddress;

typedef struct {
    char message[256];
    int missesLeft;
    int field[FIELD_SIZE][FIELD_SIZE];
    int mask[FIELD_SIZE][FIELD_SIZE];
} MainData;

#endif