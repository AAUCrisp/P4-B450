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
#include <sys/shm.h>  //IPC thing
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "Semaphore.h"

void* shm_read(const int SIZE, const char* name) {
    /* Semaphore setup */
    // sem_unlink(SemLockRead);
    // sem_unlink(SemLockWrite);

    /*sem_t* SemRead = sem_open(SemLockRead, 0);
    if (SemRead == SEM_FAILED) {
        perror("Parent: [sem_open] Failed");
        exit(EXIT_FAILURE);
    }

    sem_t* SemWrite = sem_open(SemLockWrite, 0);
    if (SemWrite == SEM_FAILED) {
        perror("Child: [sem_open] Failed");
        exit(EXIT_FAILURE);
    }*/

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        fprintf(stderr, "errno shm_open failed: %s\n", strerror(errno));
    }

    /* memory map the shared memory object */
    // printf("READER: Before sem_wait\n");
    // sem_wait(SemWrite);
    // printf("READER: Got the SemWrite?\n");
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    // sem_post(SemRead);
    // printf("READER: Gave the SemRead?\n");
    //  sem_close(SemRead);
    //  sem_close(SemWrite);

    return ptr;
}