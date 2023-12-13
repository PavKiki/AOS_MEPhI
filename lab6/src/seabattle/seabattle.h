#ifndef SEABATTLE_H
#define SEABATTLE_H

#include "../../lab6.h"
#include "random/random_int.h"

#define COMMAND_LENGTH 256
#define ARGUMENTS_LENGTH 256
#define SHOT_COMMAND_LENGTH 5
#define BATTLE_COMMAND_LENGTH 7

// int main();
void generateNewField(int field[FIELD_SIZE][FIELD_SIZE]);
int checkIfCellAvailable(int field[FIELD_SIZE][FIELD_SIZE], int x, int y);
int checkIfNotExistOrEmpty(int field[FIELD_SIZE][FIELD_SIZE], int x, int y);
void printPlacement(int field[FIELD_SIZE][FIELD_SIZE]);
void printShots(int mask[FIELD_SIZE][FIELD_SIZE]);
void printCombinedField(int field[FIELD_SIZE][FIELD_SIZE], int mask[FIELD_SIZE][FIELD_SIZE]);
int decypherCommand(char command[COMMAND_LENGTH], char arguments[ARGUMENTS_LENGTH], int *first, int *second);
int decypherShot(char arguments[ARGUMENTS_LENGTH], int *x, int *y);
int decypherGameLevel(char arguments[ARGUMENTS_LENGTH], int *quantity);
// void shot(MainData *data, int x, int y);
int checkHit(MainData *data, int x, int y);
void clearField(int field[FIELD_SIZE][FIELD_SIZE]);
int checkIfWin(MainData *data);

typedef struct {
    char body[COMMAND_LENGTH];
    char arguments[ARGUMENTS_LENGTH];
} Command;

#endif