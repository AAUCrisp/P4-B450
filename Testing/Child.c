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

#define buffer 1000
#define SIZE 1000

int generate(int Min, int Max) {
    int number = (rand() % ((Max + 1) - Min)) + Min;
    return number;
}

int main() {
    printf("PID of Child.c: %d\n", getpid());
    int count = 0;
    int gsv;
    int received;
    int random;

    const char* key = "gsv_key";
    char message[2];
    char* KOMNU;
    char* TESTX;

    /* Semaphore setup */
    sem_unlink(SemLockRead);
    sem_unlink(SemLockWrite);

    sem_t* SemRead = sem_open(SemLockRead, O_CREAT, 0660, 0);
    if (SemRead == SEM_FAILED) {
        perror("Parent: [sem_open] Failed");
        exit(EXIT_FAILURE);
    }

    sem_t* SemWrite = sem_open(SemLockWrite, O_CREAT, 0660, 1);
    if (SemWrite == SEM_FAILED) {
        perror("Child: [sem_open] Failed");
        exit(EXIT_FAILURE);
    }

    TESTX = shm_create_writer(2, key);
    printf("TESTX: %p\n", TESTX);

    /* Simulate receiving GSV */
    while (1) {
        /* Generate random integer 0-2 */
        gsv = generate(0, 2);
        sprintf(message, "%d", gsv);

        /* Used to test read/write and any synchronization problems */
        //usleep(10000);

       
        /* write to the shared memory object */
        //printf("WRITER: Before sem_wait\n");
        sem_wait(SemWrite);
        //printf("WRITER: Got the SemWrite?\n");
        sprintf(TESTX, "%s", message);
        printf("gsv: %d\n", gsv);
        sem_post(SemRead);
        //printf("WRITER: Gave the SemRead?\n");
        //sem_close(SemWrite);
        //sem_close(SemRead);
       
        printf("Count: %d\n", count++);

        usleep(1000);
        if (count == 100000000) {
            break;
        }
    }
    exit(0);
}
