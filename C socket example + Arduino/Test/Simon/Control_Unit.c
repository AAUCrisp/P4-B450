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
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "Sockets_TX_RX.h"

/* Main running code */
int main() {
    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE = 9123;
    uint PORT_WiFi = 9124;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";
    int sockLTE;
    int sockWiFi;

    /* Create sockets */
    Create_Bind_Sockets(sockLTE, sockWiFi, PORT_LTE, PORT_WiFi, LTE, WiFi);

    while (1) {
        Timestamp();
        pthread_create(&T1, NULL, receiveshit1, NULL);
        pthread_create(&T2, NULL, receiveshit2, NULL);
    }

    close(sockLTE && sockWiFi);
    exit(0);
}