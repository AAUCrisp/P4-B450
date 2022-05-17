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

int main() {
    /* the size (in bytes) of shared memory object */
    const int SIZE = 4096;
    printf("This is size of shared memory buffer: %d\n", SIZE);

    /* name of the shared memory object */
    const char* name = "OS";
    printf("This is shared memory object name: %s\n", name);

    /* shared memory file descriptor */
    int shm_fd;
    printf("This is shm_fd: %d\n", shm_fd);

    /* pointer to shared memory object */
    void* ptr;
    printf("This is ptr %p\n", ptr);

    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDONLY, 0666);
    printf("This is shm_fd with shm_open: %d\n", shm_fd);

    /* memory map the shared memory object */
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    printf("This is ptr memory map: %p\n", ptr);

    /* read from the shared memory object */
    printf("%s", (char*)ptr);

    /* remove the shared memory object */
    shm_unlink(name);
    return 0;
}