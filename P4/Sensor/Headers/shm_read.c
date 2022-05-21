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
// const char* SEM_READ_FNAME;
// const char* SEM_WRITE_FNAME;

void* shm_read(const int SIZE, const char* name) {
    printf("Do I reach here 1");
    /* Semaphore variables */
    sem_t* SEM_WRITE = sem_open(SEM_WRITE_FNAME, 1);
    printf("WHY NO WORK? 1");
    if (SEM_WRITE == SEM_FAILED) {
        perror("shm_read = sem_open/SEM_WRITE");
        exit(EXIT_FAILURE);
    }
    printf("Do I reach here 2");

    sem_t* SEM_READ = sem_open(SEM_READ_FNAME, 1);
    printf("WHY NO WORK? 2");
    if (SEM_READ == SEM_FAILED) {
        perror("shm_read = sem_open/SEM_READ");
        exit(EXIT_FAILURE);
    }
    printf("Do I reach here 3");

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    sem_wait(SEM_READ);
    printf("Do I reach here 4");
    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDONLY, 0666);
    // printf("This is shm_fd with shm_open: %d\n", shm_fd);
    printf("Do I reach here 5");

    /* memory map the shared memory object */
    ptr = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    printf("Read from shm_read: %s\n", (char*)ptr);

    sem_post(SEM_WRITE);
    printf("Do I reach here 6");
    sem_close(SEM_READ);
    printf("Do I reach here 7");
    sem_close(SEM_WRITE);
    printf("Do I reach here 8");

    // printf("This is ptr memory map: %p\n", ptr);

    /* read from the shared memory object */
    // printf("Read from shm_read.c: %s\n", (char*)ptr);

    /* remove the shared memory object */
    // shm_unlink(name);

    return ptr;
}