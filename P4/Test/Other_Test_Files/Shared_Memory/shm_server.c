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

    /* name of the shared memory object */
    const char* name = "OS";

    /* strings written to shared memory */
    const char* message_0 = "Helloasdfasdfasfdsadfadsfasdfasdf ";
    const char* message_1 = "World2!\n";

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    printf("This is shm_fd: %d\n", shm_fd);

    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);
    printf("This is shm_fd ftruncated: %d\n", shm_fd);
    /* memory map the shared memory object */
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    /* write to the shared memory object */
    sprintf(ptr, "%s", message_0);
    printf("This is ptr: %p\n", ptr);
    printf("This is ptr char*: %s\n", (char*)ptr);
    ptr += strlen(message_0);
    printf("This is ptr new: %p\n", ptr);
    printf("This is ptr char*2: %s\n", (char*)ptr);
    sprintf(ptr, "%s", message_1);
    printf("This is ptr before concatenate: %p\n", ptr);
    printf("This is ptr char*3: %s\n", (char*)ptr);
    ptr += strlen(message_1);
    printf("This is ptr concatenated: %p\n", ptr);
    printf("This is ptr char*4: %s\n", (char*)ptr);

    printf("This is size of shared memory buffer: %d\n", SIZE);
    printf("This is shared memory object name: %s\n", name);


    return 0;
}