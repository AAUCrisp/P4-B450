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

void* shm_create_writer(const int SIZE, const char* name) {
    /* Semaphore setup */
    /*sem_unlink(SemLockRead);
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
    }*/

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        fprintf(stderr, "errno shm_open failed: %s\n", strerror(errno));
    }

    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);

    /* memory map the shared memory object */
    //printf("WRITER: Before sem_wait\n");
    //sem_wait(SemWrite);
    //printf("WRITER: Got the SemWrite?\n");
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    //sem_post(SemRead);
    //printf("WRITER: Gave the SemRead?\n");
    //sem_close(SemWrite);
    //sem_close(SemRead);

    return ptr;
}
