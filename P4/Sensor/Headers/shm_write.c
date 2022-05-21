#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>  //IPC thing
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/shm.h>  //SHM thing
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "shm_write_read.h"

/* Semaphore Names */
// const char* SEM_READ_FNAME = "../";
// const char* SEM_WRITE_FNAME = "../";

void shm_write(const char* message, const int SIZE, const char* name) {
    printf("Do I reach here 1\n");
    /* Semaphore variables */
    // sem_unlink(SEM_READ_FNAME);
    // sem_unlink(SEM_WRITE_FNAME);

    sem_init(&SEM_WRITE, 1, 1);
    printf("WHY NO WORK? 1\n");
    /*
    if (SEM_WRITE == SEM_FAILED) {
        perror("shm_write = sem_open/SEM_WRITE");
        exit(EXIT_FAILURE);
    }*/
    printf("Do I reach here 2\n");
    sem_init(&SEM_READ, 1, 1);
    printf("WHY NO WORK? 2\n");
    /*
    if (SEM_READ == SEM_FAILED) {
        perror("shm_write = sem_open/SEM_READ");
        exit(EXIT_FAILURE);
    }*/
    printf("Do I reach here 3\n");

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    /* create the shared memory object */
    // shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); ORIGINAL
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0644);
    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);

    printf("Do I reach here 4\n");
    sem_wait(&SEM_WRITE);
    printf("Do I reach here 5\n");

    /* memory map the shared memory object */
    ptr = mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    printf("Do I reach here 6\n");
    /* write to the shared memory object */
    sprintf(ptr, "%s", message);
    printf("Wrote from shm_write: %s\n", (char*)ptr);

    sem_post(&SEM_READ);
    printf("Do I reach here 7\n");
    sem_close(&SEM_READ);
    printf("Do I reach here 8\n");
    sem_close(&SEM_WRITE);
    printf("Do I reach here 9\n");

    // printf("This is ptr: %p\n", ptr);
    // printf("This is ptr char*: %s\n", (char*)ptr);
    // printf("This is shm_fd: %d\n", shm_fd);
    // printf("This is size of shared memory buffer: %d\n", SIZE);
    // printf("This is shared memory object name: %s\n", name);
}
