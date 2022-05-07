#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>  //IPC thing
#include <sys/shm.h>  //IPC thing
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "shm_write_read.h"

int main() {
    printf("PID of HelloWorld.c: %d\n", getpid());
    int count;
    const int buffer = 32;
    const char* name = "shm0";
    shm_read(buffer, name);
    printf("%s", (char*)shm_read);

    while (1) {
        sleep(1);
        count++;
        printf("Hello world! %d\n", count);
        // printf("PID of parent_pid: %d\n", getppid());
        // printf("PID of child_pid: %d\n", getpid());
        if (count == 4) {
            break;
        }
    }
    exit(0);
}
