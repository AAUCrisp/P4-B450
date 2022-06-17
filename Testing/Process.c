
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

#include "shm_read_write.h"

#define buffer 1000

int main() {
    pid_t child_pid;
    child_pid = fork();

    if (child_pid == 0) {
        printf("PID of parent_pid: %d\n", getppid());
        printf("PID of child_pid: %d\n", getpid());
        printf("\n");
        char cmd[] = "./Child";

        printf("%s", cmd);
        char *args[] = {"./Child", NULL};
        char *envVec[] = {NULL};
        execv(cmd, args);
        printf("MULTIPROCESS FAIL! Didn't start the second process\n\n");
        exit(0);
    }

    int count = 0;
    int gsv;
    const char *gsv_key = "gsv_key";
    char *message;

    // shm_unlink(name);

    while (1) {
        usleep(1);
        count++;
        message = shm_read(buffer, gsv_key);
        printf("String message from child process: %s\n", (char *)message);
        printf("Parent! %d\n", count);
        // printf("PID of parent_pid: %d\n", getppid());
        // printf("PID of child_pid: %d\n", getpid());
        if (count == 10) {
            break;
        }
    }
    exit(0);
}