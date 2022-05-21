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
    printf("Do I reach here 1\n");
    /* Semaphore variables */
    sem_t* SEM_WRITE = sem_init(&SEM_WRITE_FNAME, 1, 1);
    printf("WHY NO WORK? 1\n");
    int sem_getvalue1(SEM_WRITE, int* valp);
    printf("SEM_WRITE Semaphore value: %d\n", sem_getvalue1);
    if (SEM_WRITE == SEM_FAILED) {
        perror("shm_read = sem_open/SEM_WRITE");
        exit(EXIT_FAILURE);
    }
    printf("Do I reach here 2\n");

    sem_t* SEM_READ = sem_init(&SEM_READ_FNAME, 1, 1);
    printf("WHY NO WORK? 2\n");
    int sem_getvalue2(SEM_READ, int* valp);
    printf("SEM_READ Semaphore value: %d\n", sem_getvalue2);
    if (SEM_READ == SEM_FAILED) {
        perror("shm_read = sem_open/SEM_READ");
        exit(EXIT_FAILURE);
    }
    printf("Do I reach here 3\n");

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    sem_wait(SEM_READ);
    printf("Do I reach here 4\n");
    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDONLY, 0666);
    // printf("This is shm_fd with shm_open: %d\n", shm_fd);
    printf("Do I reach here 5\n");

    /* memory map the shared memory object */
    ptr = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    printf("Read from shm_read: %s\n", (char*)ptr);

    sem_post(SEM_WRITE);
    printf("Do I reach here 6\n");
    sem_close(SEM_READ);
    printf("Do I reach here 7\n");
    sem_close(SEM_WRITE);
    printf("Do I reach here 8\n");

    // printf("This is ptr memory map: %p\n", ptr);

    /* read from the shared memory object */
    // printf("Read from shm_read.c: %s\n", (char*)ptr);

    /* remove the shared memory object */
    // shm_unlink(name);

    return ptr;
}