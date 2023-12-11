#include "../../lab6.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Incorrect arguments input! It should be ./%s hostname port");
        return 1;
    }

    char* hostname = argv[1];
    int clientSocket, port = atoi(argv[2]);

    createAndConfigureSocket(&clientSocket, hostname, port);
    startGame(clientSocket);
    close(clientSocket);
}

int createAndConfigureSocket(int *clientSocket, char *hostname, int port) {
    if ((*clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Socket establishment");
        return 1;
    }

    InternetSocketAddress serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);                                   //convert to internet port correctly
    if (inet_pton(AF_INET, hostname, &(serverAddress.sin_addr)) == -1) {    //convert to internet address and write into sin_addr 
        perror("Error on creating inernet address");
        return 1;
    }

    if (connect(*clientSocket, &serverAddress, (socklen_t) sizeof(serverAddress)) == -1) {
        perror("Error on connecting to the server");
        return 1;
    }

    return 0;
}

int startGame(int clientSocket) {
    char* command = "battle {50}"; 
    if (write(clientSocket, command, strlen(command) + 1) != strlen(command) + 1) {
        perror("Error on starting the game");
        return 1;
    }
}