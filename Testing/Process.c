
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
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

#include "Semaphore.h"
#include "shm_read_write.h"

int main() {
    pid_t child_pid;
    child_pid = fork();

    /*if (child_pid == 0) {
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
    }*/

    int count = 0;
    int gsv;
    const char *key = "gsv_key";
    char *message;

    SemRead = sem_open(SemLockRead, 0);
    if (SemRead == SEM_FAILED) {
        perror("Parent: [sem_open] Failed");
        exit(EXIT_FAILURE);
    }

    SemWrite = sem_open(SemLockWrite, 0);
    if (SemWrite == SEM_FAILED) {
        perror("Child: [sem_open] Failed");
        exit(EXIT_FAILURE);
    }

    message = shm_read(2, key);

    while (1) {
        // usleep(10000);
        // printf("READER: Before sem_wait\n");
        sem_wait(SemRead);
        // printf("READER: Got the SemWrite?\n");
        printf("gsv: %s\n", (char *)message);
        sem_post(SemWrite);
        // printf("READER: Gave the SemRead?\n");
        // sem_close(SemRead);
        // sem_close(SemWrite);
        //  sleep(100000);

        printf("Count: %d\n", count);
        // printf("PID of parent_pid: %d\n", getppid());
        // printf("PID of child_pid: %d\n", getpid());

        count++;
        usleep(1000);
        if (count == 100000000) {
            break;
        }
    }
    exit(0);
}