
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
    printf("PID of Child: %d\n", getpid());
    sleep(1);

    /* Bunch of variables */
    int count = 0;
    int counts = 100000;
    int gsv;
    const char* key = "gsv_key";
    char* message;

    char* B = "0";
    char* W = "1";
    char* L = "2";

    /* Semaphore setup */
    SemRead = sem_open(SemLockRead, 0);
    if (SemRead == SEM_FAILED) {
        perror("Parent: [sem_open] Failed\n");
        exit(EXIT_FAILURE);
    }

    SemWrite = sem_open(SemLockWrite, 0);
    if (SemWrite == SEM_FAILED) {
        perror("Child: [sem_open] Failed\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize shm object */
    message = shm_read(2, key);

    while (1) {
        /* Read the value in the shared memory object */
        sem_wait(SemRead);
        count++;
        printf("Child gsv: %s\n", (char*)message);
        printf("Child Count: %d\n", count);
        sem_post(SemWrite);

        /* Breakout */
        if (count == counts) {
            break;
        }

        /* Dummy functions to simulate the transmission via WiFi or LTE */
        if (strcmp(message, B) == 0 || strcmp(message, W) == 0) {
            void dummyfunction();
            printf("\n========================================\n");
            printf("Inside first if statement\n");
            printf("========================================\n\n");
        }

        if (strcmp(message, B) == 0 || strcmp(message, L) == 0) {
            void dummyfunction();
            printf("\n========================================\n");
            printf("Inside second if statement\n");
            printf("========================================\n\n");
        }
    }
    exit(0);
}