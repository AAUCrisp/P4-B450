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

#define IPC_RESULT_ERROR (-1)

/* Semaphore Names */
#define SEM_READ_FNAME "../SEM_READ"
#define SEM_WRITE_FNAME "../SEM_WRITE"

void shm_write(const char* message, const int SIZE, const char* name) {
    /* Semaphore variables */
    // sem_unlink(SEM_READ_FNAME);
    // sem_unlink(SEM_WRITE_FNAME);

    sem_t* SEM_WRITE = sem_open(SEM_WRITE_FNAME, O_CREAT, 0660, 0);
    if (SEM_WRITE == SEM_FAILED) {
        perror("shm_write = sem_open/SEM_WRITE");
        exit(EXIT_FAILURE);
    }

    sem_t* SEM_READ = sem_open(SEM_READ_FNAME, O_CREAT, 0660, 0);
    if (SEM_READ == SEM_FAILED) {
        perror("shm_write = sem_open/SEM_READ");
        exit(EXIT_FAILURE);
    }

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    /* create the shared memory object */
    // shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); ORIGINAL
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0644);

    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);

    sem_wait(SEM_WRITE);

    /* memory map the shared memory object */
    ptr = mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    /* write to the shared memory object */
    sprintf(ptr, "%s", message);
    sem_post(SEM_READ);
    sem_close(SEM_READ);
    sem_close(SEM_WRITE);

    // printf("Wrote from shm_write: %s\n", (char*)ptr);

    // printf("This is ptr: %p\n", ptr);
    // printf("This is ptr char*: %s\n", (char*)ptr);
    // printf("This is shm_fd: %d\n", shm_fd);
    // printf("This is size of shared memory buffer: %d\n", SIZE);
    // printf("This is shared memory object name: %s\n", name);
}
