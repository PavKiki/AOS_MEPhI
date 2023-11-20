#ifndef TASK6_H
#define TASK6_H

#include "../lab5.h"

void serverProcess();
int getServerIdentifier();
char* getClientStringIdentifierFromMessage(Message* msg);
int sendResponse(int clientIdentifier, char* message);

void clientProcess();
int getClientIdentifier();
int sendRequestToServer(int counter, int serverIdentifier, int clientIdentifier);
int readResponseFromServer(int clientIdentifier);

#endif
