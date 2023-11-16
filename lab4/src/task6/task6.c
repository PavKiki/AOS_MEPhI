#include "task6.h"

int main() {
	struct stat st;
    char* file = "src/task6/fifo";
    if (mkfifo(file, 0777) == -1) perror("mkfifo");
    if (stat(file, &st) == -1) perror("fifo");
	if (!S_ISFIFO(st.st_mode)) printf("%s file is not a fifo\n", file);
    else printf("%s file is a fifo\n", file);

    int childPid = fork();
    switch (childPid) {
        case -1: {
            perror("Error on fork occured!");
            break;
        }
        case 0: {
            char letter;
            int readDescriptor = open(file, O_RDONLY | O_NDELAY);
            
            sleep(1);

            read(readDescriptor, &letter, 1);
            printf("Output from fifo is %c\n", letter);

            close(readDescriptor);
            break;
        }
        default: {
            //parent
            int writeDescriptor = open(file, O_WRONLY | O_NDELAY);
            
            // if O_NDELAY is set
            // perror("open");
            
            write(writeDescriptor, "A", 1);

            close(writeDescriptor);
            wait(NULL);
            break;
        }
    //Если установлен флаг O_NDELAY, то вызов open только на чтение завершается без задержки, а вызов open только для записи отрабатывает с сообщением об ошибке, если в данный момент нет процесса, открывшего файл для чтения;
    //Если не установлен флаг O_NDELAY, то вызов open только на чтение блокируется, пока какой-либо процесс не откроет файл для записи, а вызов open только на запись блокируется, пока какой либо процесс не откроет файл на чтение. 
    }
}