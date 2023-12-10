#include "read_config.h"

int readFromConfig(char* filename, char* loggingFilename, int* port) {
    FILE* fd;
    if ((fd = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Cannot open the file %s\n", filename);
        return 1;
    }

    if (fscanf(fd, "log.filename=%s\nserver.port=%d", loggingFilename, port) != 2) {
        fprintf(stderr, "Cannot read properties from the file %s\n", filename);
        return 1;
    }

    fclose(fd);
}