#include "../../lab6.h"
#include "config/read_config.h"


int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect arguments input! It should be ./%s configurationsFilename\n");
        return 1;
    }

    int serverSocket, port;
    char *configFilename = argv[1];
    char logFilename[256] = { 0 };

    readFromConfig(configFilename, logFilename, &port);
    printf("configFilename: %s\nloggingFilename: %s\nport: %d\n", configFilename, logFilename, port);

    createAndConfigureSocket(&serverSocket, port);
}

int createAndConfigureSocket(int *serverSocket, int port) {
    if ((*serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("[SERVER] Socket establishment");
        return 1;
    }

    InternetSocketAddress serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);       //convert to internet port correctly
    serverAddress.sin_addr.s_addr = INADDR_ANY; //bind to all local interfaces
    if (bind(*serverSocket, (AbstractSocketAddress*) &serverAddress, (socklen_t) sizeof(serverAddress)) == -1) {
        perror("[SERVER] Error on binding socket with address");
        return 1;
    }

    if (listen(*serverSocket, MAX_OF_PENDING_CONNECTIONS) == -1) {
        perror("[SERVER] Error on listening to upcoming connections");
        return 1;
    }

    int currentClientSocket;
    InternetSocketAddress clientAddress;
    clientAddress.sin_family = AF_INET;
    socklen_t clientAddressLength = sizeof(clientAddress);
    if ((currentClientSocket = accept(*serverSocket, (AbstractSocketAddress*) &clientAddress, &clientAddressLength)) == -1) {
        perror("[SERVER] Error occured on accepting upcoming connections");
        return 1;
    }

    printf("Connection has been established!");
}