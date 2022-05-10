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

    /* Misc */


    pthread_t T1, T2;

    /* Struct for message & buffer size */
    char* msg;

    /* Create sockets */
    Sockets sock;
    Create_Bind_Sockets(&sock, PORT_LTE, PORT_WiFi, LTE, WiFi);
    printf("sockLTE control_unit: %d\n", sock.sockLTE);
    printf("sockWiFi control_unit: %d\n", sock.sockWiFi);

    while (1) {
        Timestamp();
        //RSSI_VAL();
        //RSRP_VAL();
        pthread_create(&T1, NULL, receiveLTE, (void*)&sock);
        pthread_join(T1, (void**)&msg);
        pthread_create(&T2, NULL, receiveWiFi, (void*)&sock);
        pthread_join(T2, (void**)&msg);
        
        //printf("%s\n", msg);
        sleep(1);
    }

    close(sock.sockLTE && sock.sockWiFi);
    exit(0);
}