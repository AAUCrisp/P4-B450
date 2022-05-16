#include "../Libraries.c"

void* shm_read(const int SIZE, const char* name) {
    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDONLY, 0666);
    // printf("This is shm_fd with shm_open: %d\n", shm_fd);

    /* memory map the shared memory object */
    ptr = mmap(0 /*or NULL*/, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    // printf("This is ptr memory map: %p\n", ptr);

    /* read from the shared memory object */
    // printf("Read from shm_read.c: %s\n", (char*)ptr);

    /* remove the shared memory object */
    // shm_unlink(name);

    return ptr;
}