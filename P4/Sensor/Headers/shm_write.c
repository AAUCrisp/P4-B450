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
extern int errno;

void shm_write(const char* message, const int SIZE, const char* name) {
    printf("Do I reach here 1\n");
    /* Semaphore variables */
    // sem_unlink(SEM_READ_FNAME);
    // sem_unlink(SEM_WRITE_FNAME);

    int sem_write = sem_init(&SEM_WRITE, 1, 1);
    printf("WHY NO WORK? 1\n");
    if (sem_write == -1) {
        perror("shm_write = sem_open/SEM_WRITE");
        exit(EXIT_FAILURE);
    }
    printf("Do I reach here 2\n");
    int sem_read = sem_init(&SEM_READ, 1, 1);
    printf("WHY NO WORK? 2\n");
    if (sem_read == -1) {
        perror("shm_write = sem_open/SEM_READ");
        exit(EXIT_FAILURE);
    }
    printf("Do I reach here 3\n");

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    /* create the shared memory object */
    // shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); ORIGINAL
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0644);
    if (shm_fd == -1) {
        perror("shm_open failed");
        fprintf(stderr, "errno shm_open failed %s\n", strerror(errno));
    }
    if (shm_fd == 1023) {
        printf("Success on overwrite!");
    }

    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);

    printf("Do I reach here 4\n");
    if (sem_wait(&SEM_WRITE) == -1) {
        perror("SEM_WRITE sem_wait failed");
    }
    printf("Do I reach here 5\n");

    printf("SHM fd in WRITE: %d\n", shm_fd);
    printf("SHM name in WRITE: %s\n", (char*)name);

    /* memory map the shared memory object */
    ptr = mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
    }
    printf("Do I reach here 6\n");
    /* write to the shared memory object */
    sprintf(ptr, "%s", message);
    printf("Wrote from shm_write: %s\n", (char*)ptr);

    munmap(ptr, SIZE);

    if (sem_post(&SEM_READ)) {
        perror("SEM_WRITE sem_post failed");
    }
    printf("Do I reach here 7\n");
    // sem_close(&SEM_READ);
    printf("Do I reach here 8\n");
    // sem_close(&SEM_WRITE);
    printf("Do I reach here 9\n");

    // printf("This is ptr: %p\n", ptr);
    // printf("This is ptr char*: %s\n", (char*)ptr);
    // printf("This is shm_fd: %d\n", shm_fd);
    // printf("This is size of shared memory buffer: %d\n", SIZE);
    // printf("This is shared memory object name: %s\n", name);
    // close(shm_fd);
}
