#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
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

#include "Variables.h"

void* shm_create_writer(const char* name) {
    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        fprintf(stderr, "errno shm_open failed: %s\n", strerror(errno));
    } else {
        printf("shm_fd create_writer: %d\n", shm_fd);
    }
    if (shm_fd == 65528) {
        printf("shm_fd value was too big!");
    }

    /* configure the size of the shared memory object */
    //printf("Reaches here ftruncate\n");
    ftruncate(shm_fd, SIZE);

    /* memory map the shared memory object */
    /*printf("Reaches here ptr = mmap1\n");
    printf("Reaches here ptr = mmap2\n");
    */
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    /* write to the shared memory object */
    // sprintf(ptr, "%s", message);
    // close(shm_fd);

    printf("ptr: %p\n", ptr);

    return ptr;
}
