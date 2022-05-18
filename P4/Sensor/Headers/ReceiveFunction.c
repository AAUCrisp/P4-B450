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

#include "SocketFunctions.h"
#include "shm_write_read.h"

/* Misc */
pthread_t T1, T2;

void *receiveLTE(void *socket) {
    while (1) {
        Sockets *sock = (Sockets *)socket;
        const char *GSV_KEY = "GSV_KEY";
        int LenLTE = sizeof(sock->ServerLTE_RECEIVER);

        printf("receiveLTE socket: %d\n", sock->sockLTE_RECEIVER);
        RX_LTE = recvfrom(sock->sockLTE_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_RECEIVER, &LenLTE);
        Timestamp();

        // printf("LTE || LTE-Thread id = %ld\n", pthread_self());
        printf("LTE || Message from LTE received at: %s\n", curr_time);
        printf("LTE || Message: %s Sensor \n", message);
        shm_write(message, 32, GSV_KEY);
    }
}

void *receiveWiFi(void *socket) {
    while (1) {
        Sockets *sock = (Sockets *)socket;
        const char *GSV_KEY = "GSV_KEY";
        int LenWiFi = sizeof(sock->ServerWiFi_RECEIVER);

        printf("receiveWiFi socket: %d\n", sock->sockWiFi_RECEIVER);
        RX_WiFi = recvfrom(sock->sockWiFi_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, &LenWiFi);
        Timestamp();

        // printf("WiFi || WiFi-Thread id = %ld\n", pthread_self());
        printf("WiFi || Message from WiFi received at: %s \n", curr_time);
        printf("WiFi || Message: %s Sensor \n", message);
        shm_write(message, 32, GSV_KEY);
    }
}

void *transmitLTE(void *socket) {
    const char *RAND_INT_KEY = "RAND_INT_KEY";
    const char *RAND_INT;

    RAND_INT = shm_read(32, RAND_INT_KEY);
    printf("LTE || Random int from shm: %s\n", RAND_INT);

    // int LenLTE = sizeof(sock.ClientLTE_TRANSMITTER);
    printf("transmitLTE socket: %d\n", sock.sockLTE_TRANSMITTER);

    char sendLTE[BUFFER];
    curr_time = Timestamp();
    sprintf(sendLTE, "%d %s", RAND_INT, curr_time);
    printf("sendLTE: %s\n", sendLTE);

    sendto(sock.sockLTE_TRANSMITTER, sendLTE, BUFFER, 0, (struct sockaddr *)&sock.ClientLTE_TRANSMITTER, sizeof(sock.sockLTE_TRANSMITTER));
    printf("Message from LTE transmitted at: %s\n", curr_time);
}

void *transmitWiFi(void *socket) {
    const char *RAND_INT_KEY = "RAND_INT_KEY";
    const char *RAND_INT;

    RAND_INT = shm_read(32, RAND_INT_KEY);
    printf("LTE || Random int from shm: %s\n", RAND_INT);

    // int LenWiFi = sizeof(sock.ClientWiFi_TRANSMITTER);
    printf("transmitLTE socket: %d\n", sock.sockWiFi_TRANSMITTER);

    char sendWiFi[BUFFER];
    curr_time = Timestamp();
    sprintf(sendWiFi, "%d %s", RAND_INT, curr_time);
    printf("sendWiFi: %s\n", sendWiFi);

    sendto(sock.sockWiFi_TRANSMITTER, sendWiFi, BUFFER, 0, (struct sockaddr *)&sock.ClientWiFi_TRANSMITTER, sizeof(sock.ClientWiFi_TRANSMITTER));
    printf("Message from WiFi transmitted at: %s\n", curr_time);
}

void receiverLTE() {
    pthread_create(&T1, NULL, receiveLTE, (void *)&sock);
}

void receiverWiFi() {
    pthread_create(&T2, NULL, receiveWiFi, (void *)&sock);
}

void transmitterLTE() {
    pthread_create(&T1, NULL, transmitLTE, (void *)&sock);
}

void transmitterWiFi() {
    pthread_create(&T2, NULL, transmitWiFi, (void *)&sock);
}