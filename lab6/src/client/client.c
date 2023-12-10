#include "../../lab6.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Incorrect arguments input! It should be ./%s hostname port");
        return 1;
    }

    char* hostname = argv[1];
    int clientSocket, port = atoi(argv[2]);

    createAndConfigureSocket(&clientSocket, hostname, port);
}

int createAndConfigureSocket(int *clientSocket, char *hostname, int port) {
    if ((*clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("[SERVER] Socket establishment");
        return 1;
    }

    InternetSocketAddress serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);                                   //convert to internet port correctly
    if (inet_pton(AF_INET, hostname, &(serverAddress.sin_addr)) == -1) {    //convert to internet address and write into sin_addr 
        perror("[CLIENT] Error on creating inernet address");
        return 1;
    }

    if (connect(*clientSocket, &serverAddress, (socklen_t) sizeof(serverAddress)) == -1) {
        perror("[CLIENT] Error on connecting to the server");
        return 1;
    }

    printf("Connection has been established!");
}