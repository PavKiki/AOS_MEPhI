#include "random_int.h"

int getRandomInt() {
    int data;
    FILE *fp;
    fp = fopen("/dev/urandom", "r");
    fread(&data, 1, sizeof(int), fp);
    fclose(fp);

    return data;
}