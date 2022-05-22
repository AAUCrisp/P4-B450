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
extern int errno;

void* shm_read(const int SIZE, const char* name) {
  
    /* Semaphore variables */
    int sem_write = sem_init(&SEM_WRITE, 1, 1);
   
    if (sem_write == -1) {
        perror("shm_read = sem_open/SEM_WRITE");
        exit(EXIT_FAILURE);
    }
   
    int sem_read = sem_init(&SEM_READ, 1, 1);
   
    if (sem_read == -1) {
        perror("shm_read = sem_open/SEM_READ");
        exit(EXIT_FAILURE);
    }
   

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;
/*
    if (sem_wait(&SEM_READ) == -1) {
        perror("SEM_READ sem_wait failed");
    }*/
    
    
    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDONLY, 0644);
    if (shm_fd == -1) {
        perror("shm_open failed");
        fprintf(stderr, "errno shm_open failed: %s\n", strerror(errno));
    }
   
  
    /* memory map the shared memory object */
    ptr = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Something went wrong: %d\n", errno);
        perror("mmap failed");
        strerror(errno);
    }

    printf("Read from shm_read: %s\n", (char*)ptr);
    // munmap(ptr, SIZE);
    printf("shm_fd value: %d\n", shm_fd);
    close(shm_fd);

    /* if (sem_post(&SEM_WRITE)) {
         perror("SEM_WRITE sem_post failed");
     } */
 
    // sem_close(&SEM_READ);
   
    // sem_close(&SEM_WRITE);
 

    // printf("This is ptr memory map: %p\n", ptr);

    /* read from the shared memory object */
    // printf("Read from shm_read.c: %s\n", (char*)ptr);

    /* remove the shared memory object */
    // shm_unlink(name);
    // close(shm_fd);

    return ptr;
}