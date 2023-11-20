#ifndef TASK7_H
#define TASK7_H

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
