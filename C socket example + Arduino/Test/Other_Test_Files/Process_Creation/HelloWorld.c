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
    printf("PID of HelloWorld.c: %d\n", getpid());
    int count;
    const int buffer = 64;
    const char* name = "shm0";
    const char* msg;
    msg = shm_read(buffer, name);
    count = atoi(msg);
    printf("String message from parent process: %s\n", (char*)msg);
    printf("String converted to int: %d\n", count);

    int a = 1;
    int b = 2;
    int sum = a + b + count;
    printf("Sum of a+b+count: %d+%d+%d = %d\n", a, b, count, sum);
    shm_unlink(name);

    while (1) {
        sleep(1);
        count++;
        printf("Hello world! %d\n", count);
        // printf("PID of parent_pid: %d\n", getppid());
        // printf("PID of child_pid: %d\n", getpid());
        if (count == 130) {
            break;
        }
    }
    exit(0);
}
