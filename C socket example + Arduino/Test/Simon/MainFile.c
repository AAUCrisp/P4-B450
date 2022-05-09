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
    PORT_LTE = 9123;
    PORT_WiFi = 9124;
    LTE = "wwan0";
    WiFi = "wlan0";

    /* Create sockets */
    Create_Bind_Sockets(PORT_LTE, PORT_WiFi);

    while (1) {
        Timestamp();
        pthread_create(&T1, NULL, receiveshit1, NULL);
        pthread_create(&T2, NULL, receiveshit2, NULL);
    }

    close(sockLTE && sockWiFi);
    exit(0);
}