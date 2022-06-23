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

void* shm_read(const int SIZE, const char* name) {
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
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    printf("\nread shm_fd: %d\n", shm_fd);
    //close(shm_fd);

    return ptr;
}

void* shm_write(const int SIZE, const char* name) {
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
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    printf("\nwrite shm_fd: %d\n", shm_fd);
    //close(shm_fd);

    return ptr;
}