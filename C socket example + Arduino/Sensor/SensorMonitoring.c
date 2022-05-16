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
    uint PORT_LTE = 9002;
    uint PORT_WiFi = 9003;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Misc */
    pthread_t T1, T2;

    /* Create sockets */
    Sockets sock;
    Sockets_Receiver(Sockets * sock, PORT_LTE, PORT_WiFi, LTE, WiFi);
    printf("sockLTE_RECEIVER: %d\n", sock.sockLTE_TRANSMITTER);
    printf("sockWiFi_RECEIVER: %d\n", sock.sockWiFi_TRANSMITTER);

    while (1) {
        pthread_create(&T1, NULL, receiveLTE, NULL);
        pthread_create(&T2, NULL, receiveWiFi, NULL);
    }
}