#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>  //IPC thing
#include <sys/mman.h>
#include <sys/shm.h>  //SHM thing
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "shm_write_read.h"

int main() {
    pid_t child_pid;
    child_pid = fork();
    char charvar;
    char *msg = "123";
    const int buffer = 64;
    const char *name = "shm0";

    shm_write(msg, buffer, name);

    if (child_pid == 0) {
        printf("PID of parent_pid: %d\n", getppid());
        printf("PID of child_pid: %d\n", getpid());
        printf("\n");
        char *args[] = {"HelloWorld", NULL};
        execv("./HelloWorld", args);
        int count;

        while (1) {
            sleep(1);
            count++;
            printf("Main program/parent process: %d\n", count);
            // printf("PID of parent_pid: %d\n", getppid());
            // printf("PID of child_pid: %d\n", getpid());
            if (count == 5) {
                break;
            }
        }
        exit(0);
    }
    exit(0);
}