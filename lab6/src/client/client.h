#ifndef CLIENT_H
#define CLIENT_H

#include "../../lab6.h"
#include "../seabattle/seabattle.h"

int main(int argc, char** argv);

void createAndConfigureSocket(int *clientSocket, char *hostname, int port);

#endif