#ifndef TASK8_H
#define TASK8_H

#include "../lab5.h"

void customHandler();

void serverProcess();
int getServerIdentifier();
char* getClientStringIdentifierFromMessage(Message* msg);
int sendResponse(int clientIdentifier, char* message);

void clientProcess();
int getClientIdentifier();
int sendRequestToServer(int counter, int serverIdentifier, int clientIdentifier);
int readResponseFromServer(int clientIdentifier);

#endif
