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
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "Semaphore.h"
#include "shm_read_write.h"

int generate(int Min, int Max) {
    int number = (rand() % ((Max + 1) - Min)) + Min;
    return number;
}

int main() {
    printf("PID of Parent: %d\n", getppid());
    sleep(1);

    /* Bunch of variables */
    int count = 0;
    int counts = 100000;
    int gsv;
    const char* key = "gsv_key";
    char message[2];
    char* write;

    /* Creation of Child process */
    pid_t child_pid;
    child_pid = fork();

    if (child_pid == 0) {
        char cmd[] = "./Child";
        char* args[] = {"./Child", NULL};
        char* envVec[] = {NULL};

        execv(cmd, args);
    }

    /* Semaphore setup */
    sem_unlink(SemLockRead);
    sem_unlink(SemLockWrite);

    SemRead = sem_open(SemLockRead, O_CREAT, 0660, 0);
    if (SemRead == SEM_FAILED) {
        perror("Parent: [sem_open] Failed");
        exit(EXIT_FAILURE);
    }

    SemWrite = sem_open(SemLockWrite, O_CREAT, 0660, 1);
    if (SemWrite == SEM_FAILED) {
        perror("Child: [sem_open] Failed");
        exit(EXIT_FAILURE);
    }

    /* Initialize shm object */
    write = shm_write(2, key);

    /* Simulate receiving GSV */
    while (1) {
        /* Generate random integer 0-2 */
        gsv = generate(0, 2);
        sprintf(message, "%d", gsv);

        /* Used to test read/write and any synchronization problems */
        // usleep(10000);

        /* Write value to the shared memory object */
        sem_wait(SemWrite);
        printf("Parent gsv: %d\n", gsv);
        // sprintf(write, "%s", message); // Placing char in shared memory
        sprintf(write, "%d", gsv);  // Placing int in shared memory
        sem_post(SemRead);

        /* Breakout */
        count++;
        printf("Parent Count: %d\n", count);
        if (count == counts) {
            break;
        }
    }
    exit(0);
}