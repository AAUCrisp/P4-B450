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
#include "Headers/SocketFunctions.c"

int main() {
    printf("==================\nActuator Monitoring Process Started\n==================\n\n");

    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE_RECEIVER = 9004;
    uint PORT_WiFi_RECEIVER = 9005;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Misc */
    pthread_t T1, T2;

    /* Message char */
    char* msg[32];

    /* Create sockets */
    Sockets sock;
    Sockets_Receiver(&sock, PORT_LTE_RECEIVER, PORT_WiFi_RECEIVER, LTE, WiFi);
    printf("sockLTE_RECEIVER (OUTSIDE): %d\n", sock.sockLTE_RECEIVER);
    printf("sockWiFi_RECEIVER (OUTSIDE): %d\n\n", sock.sockWiFi_RECEIVER);

    pthread_create(&T1, NULL, receiveLTE, (void*)&sock);

    while (1) {
        receiveWiFi(&sock);
    }
}