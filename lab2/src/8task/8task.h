#ifndef TASK8_H
#define TASK8_H

#include "../lab2.h"

char* createFileNameWithPostfix(char** argv, char* postfix);

int createReadOnlyFile(char* newFileName);

void readFileIntoFile(int srcFile, int destFile);

void ouputTextFromFile(char* fileName);

void readFromFileIntoConsole(int fd);

#endif