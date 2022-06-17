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
#include <sys/shm.h>  //SHM thing
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "Variables.h"
#include "shm_read_write.h"

#define buffer 1000
#define SIZE 1000

int generate(int Min, int Max) {
    int number = (rand() % ((Max + 1) - Min)) + Min;
    return number;
}

int main() {
    printf("PID of Child.c: %d\n", getpid());
    int count = 0;
    int gsv;
    int shm_fd;
    const char* name = "gsv_key";
    char message[2];
    char* KOMNU;

    void* TESTX;

    TESTX = shm_create_writer(name);
    printf("TESTX: %p\n", TESTX);

    /* Simulate receiving GSV */
    while (1) {
        // printf("Reaches here before gsv generated\n");
        gsv = generate(0, 2);
        sprintf(message, "%d", gsv);
        // printf("Reaches here after gsv generated\n");
        //  message = converted;
        /*printf("gsv: %d\n", gsv);
        printf("char message: %s\n", message);
        printf("shm_fd: %d\n", shm_fd);
        */
        // usleep(100);
        //sleep(1);

        // shm_write(message, buffer, gsv_key);
        KOMNU = message;
        //printf("KOMNU: %s\n", (char*)KOMNU);
        // printf("append shit2\n");
        //  printf("ptr: %s\n", (char*)ptr);

        /* write to the shared memory object */
        sprintf(TESTX, "%s", message);
        //printf("TESTX char: %s\n", (char*)TESTX);
        // close(shm_fd);

        // shm_unlink(shm_fd);
        printf("Count: %d\n", count++);
        if (count == 5000000000000) {
            break;
        }
    }
    while (1) {
    }
}
