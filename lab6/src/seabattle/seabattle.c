#include "seabattle.h"

// //just for tests
// int main() {
//     int field[FIELD_SIZE][FIELD_SIZE] = { 0 };
//     int mask[FIELD_SIZE][FIELD_SIZE] = { 0 };
    
//     generateNewField(field);
    
//     int x, y;
//     while (1) {
//         printPlacement(field);
//         printShots(mask);
//         printCombined(field, mask);

//         x = 0;
//         y = 0;
//         char command[COMMAND_LENGTH] = { 0 };
//         char arguments[ARGUMENTS_LENGTH] = { 0 };
//         printf("Please enter a command:\n");
//         scanf("%s\t%s", command, arguments);

//         switch (decypherCommand(command, arguments, &x, &y))
//         {
//         case SHOT:
//             shot(mask, x, y);
//             break;
        
//         case NEW_BATTLE:
//             clearField(mask);
//             clearField(field);
//             generateNewField(field);
//             break;

//         default:
//             fprintf(stderr, "main seabattle\n");
//             break;
//         }
//     }
// }

//There will be two arrays:
// 1. Which is stands for ship placement
// 2. Shows shots of the user

void clearField(int field[FIELD_SIZE][FIELD_SIZE]) {
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            field[i][j] = 0;
        }
    }
}

void generateNewField(int field[FIELD_SIZE][FIELD_SIZE]) {
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            
            if (!checkIfCellAvailable(field, i, j)) {
                continue;
            }

            //Probability 50%
            int randomNumber = getRandomInt(); 
            if ((randomNumber % 10) < PROBABILITY_OF_GENERATING_SHIP) {
                field[i][j] = 1;
            } else {
                field[i][j] = 0;
            }
        }
    }
}

//      -------> y
//      |
//      |
//      v
//      x
//
int checkIfCellAvailable(int field[FIELD_SIZE][FIELD_SIZE], int x, int y) {
    if (   checkIfNotExistOrEmpty(field, x-1, y-1) == 1     //top-left
        && checkIfNotExistOrEmpty(field, x+1, y-1) == 1     //bottom-left
        && checkIfNotExistOrEmpty(field, x-1, y+1) == 1     //top-right
        && checkIfNotExistOrEmpty(field, x+1, y+1) == 1) {  //bottom-right

        return 1;
    }

    return 0;
}

int checkIfNotExistOrEmpty(int field[FIELD_SIZE][FIELD_SIZE], int x, int y) {
    if (   x < 0
        || y < 0
        || x >= FIELD_SIZE
        || y >= FIELD_SIZE
        || field[x][y] != 1) {
        
        return 1;
    }

    return 0;
}

void printPlacement(int field[FIELD_SIZE][FIELD_SIZE]) {
    printf("Ship placement:\n");
    printf(" 0 1 2 3 4 5 6 7 8 9 \n");
    for (int i = 0; i < FIELD_SIZE; i++) {
        printf(" _ _ _ _ _ _ _ _ _ _ \n|");
        for (int j = 0; j < FIELD_SIZE; j++) {
            printf(field[i][j] == 1 ? "X|" : " |");
        }
        printf(" %d\n", i);
    }
    printf("\n");
}

void printShots(int mask[FIELD_SIZE][FIELD_SIZE]) {
    printf("Shots:\n");
    printf(" 0 1 2 3 4 5 6 7 8 9 \n");
    for (int i = 0; i < FIELD_SIZE; i++) {
        printf(" _ _ _ _ _ _ _ _ _ _ \n|");
        for (int j = 0; j < FIELD_SIZE; j++) {
            printf(mask[i][j] == 0 ? " |" : ".|");
        }
        printf(" %d\n", i);
    }
    printf("\n");
}

void printCombinedField(int field[FIELD_SIZE][FIELD_SIZE], int mask[FIELD_SIZE][FIELD_SIZE]) {
    printf("Battle field:\n");
    printf(" 0 1 2 3 4 5 6 7 8 9 \n");
    for (int i = 0; i < FIELD_SIZE; i++) {
        printf(" _ _ _ _ _ _ _ _ _ _ \n|");
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (!mask[i][j]) {
                printf(" |");
            } else {
                printf(field[i][j] == 1 ? "X|" : ".|");
            }
            
        }
        printf(" %d\n", i);
    }
    printf("\n");
}

void shot(MainData *data, int x, int y) {
    (data->mask)[x][y] = 1;

    if (!(data->mask[x][y] & data->field[x][y])) {
        data->missesLeft--;
    } 
}

//if "battle" command, argument is written in the first
int decypherCommand(char command[COMMAND_LENGTH], char arguments[ARGUMENTS_LENGTH], int *first, int *second) {
    char shotCommand[SHOT_COMMAND_LENGTH];
    char battleCommand[BATTLE_COMMAND_LENGTH];

    for (int i = 0; i < SHOT_COMMAND_LENGTH - 1; i++) {
        shotCommand[i] = command[i];
    }
    shotCommand[SHOT_COMMAND_LENGTH - 1] = '\0';

    for (int i = 0; i < BATTLE_COMMAND_LENGTH - 1; i++) {
        battleCommand[i] = command[i];
    }
    battleCommand[BATTLE_COMMAND_LENGTH - 1] = '\0';

    if (strcmp(shotCommand, "shot") == 0) {
        return decypherShot(arguments, first, second);
    } else 
    if (strcmp(battleCommand, "battle") == 0) {
        return decypherGameLevel(arguments, first);
    } else {
        fprintf(stderr, "Unknown command\n");
        return 1;
    }
} 

int decypherShot(char arguments[ARGUMENTS_LENGTH], int *x, int *y) {
    if (sscanf(arguments, "(%d, %d)\n", x, y) != 2) {
        fprintf(stderr, "Cannot recognize coordinates\n");
        return 1;
    }
    return SHOT;
}

int decypherGameLevel(char arguments[ARGUMENTS_LENGTH], int *quantity) {
    if (sscanf(arguments, "{%d}\n", quantity) != 1) {
        fprintf(stderr, "Cannot recognize quantity\n");
        return 1;
    }
    return NEW_BATTLE;
}

int checkIfWin(MainData *data) {
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (data->field[i][j] && data->mask[i][j]) {
                continue;
            } else {
                return 0;
            }
        }
    }

    return 1;
}