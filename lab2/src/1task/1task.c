#include "1task.h"

int main() {
    putandCheck("LAB2", "hello");
    putandCheck("PATH", "aboba");
    putandCheck("HOME", "gege");
    putandCheck("NOTHING", "nothing");
}

void putandCheck(char* name, char* value) {
    char* variable = makeVariable(name, value);

    char *tmp = getenv(name);
    printf("Old value of %s is %s\n", name, tmp);
    
    int code = putenv(variable);
    printf("%s\n", !code ? "New variable successfully added!" : "Something went wrong during putting the variable");

    tmp = getenv(name);
    printf("New value of %s is %s\n\n", name, tmp);

    free(variable);
}

char* makeVariable(char* name, char* value) {
    int resultLen = strlen(name) + strlen(value) + 1;

    char* result = calloc(resultLen, sizeof(char));
    strcat(result, name);
    strcat(result, "=");
    strcat(result, value);

    return result;
}