#ifndef LIBRARIES
#define LIBRARIES
#include "Libraries.c"  // File with all our includes
#endif

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