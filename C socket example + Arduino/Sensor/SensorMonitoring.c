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

#include "Headers/SocketFunctions.h"
#include "Headers/shm_write_read.h"

int main() {
    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE_RECEIVER = 9002;
    uint PORT_WiFi_RECEIVER = 9003;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Misc */
    pthread_t T1, T2;
    const char* GSV_KEY = "GSV_KEY";

    /* Message char */
    char* msg[32];

    /* Create sockets */
    Sockets sock;
    Sockets_Receiver(&sock, PORT_LTE_RECEIVER, PORT_WiFi_RECEIVER, LTE, WiFi);
    printf("sockLTE_RECEIVER (OUTSIDE): %d\n", sock.sockLTE_TRANSMITTER);
    printf("sockWiFi_RECEIVER (OUTSIDE): %d\n", sock.sockWiFi_TRANSMITTER);
    int count = 0;

    while (1) {
        count++;
        pthread_create(&T1, NULL, receiveLTE, (void*)&sock);
        pthread_join(T1, (void**)&msg);
        pthread_create(&T2, NULL, receiveWiFi, (void*)&sock);
        pthread_join(T2, (void**)&msg);
        int test2 = atoi(&msg);
        printf("msg = %s\n", (char*)msg);
        printf("test2 = %d\n", test2);
        shm_write(test2, 32, GSV_KEY);

        /*if (a != 0) {
            perror("pthread_create failed");
        }
        if (b != 0) {
            perror("pthread_create failed");
        }*/
        sleep(2);
        printf("Count: %d\n", count);
        if (count == 20) {
            exit(0);
        }
    }
}