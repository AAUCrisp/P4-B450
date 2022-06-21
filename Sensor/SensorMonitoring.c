#ifndef LIBRARIES
#define LIBRARIES
#include "Libraries.c"  // File with all our includes
#endif

#include "Headers/SocketFunctions.h"
#include "Headers/shm_read_write.h"

int main() {
    printf("==================\nSensor Monitoring Process Started\n==================\n\n");

    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE_RECEIVER = 9002;
    uint PORT_WiFi_RECEIVER = 9003;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Misc */
    pthread_t T1;

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