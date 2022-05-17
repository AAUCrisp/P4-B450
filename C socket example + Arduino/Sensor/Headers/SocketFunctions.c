
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

/* Define buffers & PORT number */
#define BUFFER 1024
char message[BUFFER];
char curr_time[128];
char *receive;

/* Misc */
int bindLTE, bindWiFi;
int RX_LTE, RX_WiFi;
int TX_LTE, TX_WiFi;

/* Function to bind sockets */
void Sockets_Receiver(Sockets *sock, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Create socket receiver */
    sock->sockLTE_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("Sockets_Receiver LTE (INSIDE): %d\n", sock->sockLTE_RECEIVER);
    printf("Sockets_Receiver WiFi (INSIDE): %d\n", sock->sockWiFi_RECEIVER);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking */
    if (sock->sockLTE_RECEIVER == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sock->sockWiFi_RECEIVER == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    printf("Sockets_RECEIVER sucessfully created\n");

    /* Configure settings to communicate with remote UDP client for receiver */
    sock->ServerLTE_RECEIVER.sin_family = AF_INET;
    sock->ServerLTE_RECEIVER.sin_port = htons(PORT_LTE);
    sock->ServerLTE_RECEIVER.sin_addr.s_addr = INADDR_ANY;

    sock->ServerWiFi_RECEIVER.sin_family = AF_INET;
    sock->ServerWiFi_RECEIVER.sin_port = htons(PORT_WiFi);
    sock->ServerWiFi_RECEIVER.sin_addr.s_addr = INADDR_ANY;

    /* Bind to socket */
    bindLTE = bind(sock->sockLTE_RECEIVER, (struct sockaddr *)&sock->ServerLTE_RECEIVER, sizeof(struct sockaddr));
    bindWiFi = bind(sock->sockWiFi_RECEIVER, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, sizeof(struct sockaddr));

    /* Error checking */
    if (bindLTE == -1) {
        perror("Failed to bind LTE socket");
        exit(0);
    }
    if (bindWiFi == -1) {
        perror("Failed to bind WiFi socket");
        exit(0);
    }
    printf("Bind was succesful!\n");
}

void Sockets_Transmitter(Sockets *sock, const char *IP_LTE, const char *IP_WiFi, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Create socket receiver */
    sock->sockLTE_TRANSMITTER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_TRANSMITTER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("Sockets_Transmitter LTE (INSIDE): %d\n", sock->sockLTE_TRANSMITTER);
    printf("Sockets_Transmitter WiFi (INSIDE): %d\n", sock->sockWiFi_TRANSMITTER);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_TRANSMITTER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_TRANSMITTER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking */
    if (sock->sockLTE_TRANSMITTER == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sock->sockWiFi_TRANSMITTER == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    printf("Sockets_TRANSMITTER sucessfully created\n");

    /* Configure settings to communicate with remote UDP client for receiver */
    sock->ClientLTE_TRANSMITTER.sin_family = AF_INET;
    sock->ClientLTE_TRANSMITTER.sin_port = htons(PORT_LTE);
    sock->ClientLTE_TRANSMITTER.sin_addr.s_addr = inet_addr(IP_LTE);

    sock->ClientWiFi_TRANSMITTER.sin_family = AF_INET;
    sock->ClientWiFi_TRANSMITTER.sin_port = htons(PORT_WiFi);
    sock->ClientWiFi_TRANSMITTER.sin_addr.s_addr = inet_addr(IP_WiFi);
}

/* Function to receive LTE packets */
void *receiveLTE(void *socket) {
    Sockets *sock = (Sockets *)socket;
    // const char *GSV_KEY = "GSV_KEY";
    int LenLTE = sizeof(sock->ServerLTE_RECEIVER);

    printf("receiveLTE socket: %d\n", sock->sockLTE_RECEIVER);
    RX_LTE = recvfrom(sock->sockLTE_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_RECEIVER, &LenLTE);

    // printf("LTE || LTE-Thread id = %ld\n", pthread_self());
    printf("LTE || Message from LTE received at: %s\n", curr_time);
    printf("LTE || Message: %s Sensor \n", message);
    receive = malloc(sizeof(receive));
    receive = message;
    // int test1 = atoi(message);
    // shm_write(test1, 32, GSV_KEY);
    printf("receiveLTE receive message: %s\n", receive);
    return (void *)receive;
    // pthread_exit(NULL);
}

/* Function to receive WiFi packets */
void *receiveWiFi(void *socket) {
    Sockets *sock = (Sockets *)socket;
    const char *GSV_KEY = "GSV_KEY";
    int LenWiFi = sizeof(sock->ServerWiFi_RECEIVER);

    printf("receiveWiFi socket: %d\n", sock->sockWiFi_RECEIVER);
    RX_WiFi = recvfrom(sock->sockWiFi_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, &LenWiFi);

    // printf("WiFi || WiFi-Thread id = %ld\n", pthread_self());
    printf("WiFi || Message from WiFi received at: %s \n", curr_time);
    printf("WiFi || Message: %s Sensor \n", message);
    int test2 = atoi(message);
    printf("test2 shmo: %d\n", test2);
    int *ptr2 = &test2;
    printf("*ptr2 shmo: %d\n", *ptr2);
    shm_write(ptr2, 32, GSV_KEY);
    /*receive = malloc(sizeof(receive));
    receive = message;
    printf("receiveWiFi receive message: %s\n", receive);
    return (void *)receive;*/
    pthread_exit(NULL);
}

void *transmitLTE(void *socket) {
    Sockets *sock = (Sockets *)socket;
    const char *RAND_KEY = "RAND_KEY";
    char *RAND_INT;
    int LenLTE = sizeof(sock->ClientLTE_TRANSMITTER);

    RAND_INT = shm_read(32, RAND_KEY);
    printf("RAND_INT LTE: %s\n", RAND_INT);

    printf("transmitLTE socket: %d\n", sock->sockLTE_TRANSMITTER);
    char test1[] = "test1";
    TX_LTE = sendto(sock->sockLTE_TRANSMITTER, test1, BUFFER, 0, (struct sockaddr *)&sock->ClientLTE_TRANSMITTER, LenLTE);

    // printf("LTE-Thread id = %ld\n", pthread_self());
    printf("Message from LTE transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

void *transmitWiFi(void *socket) {
    Sockets *sock = (Sockets *)socket;
    const char *RAND_KEY = "RAND_KEY";
    char *RAND_INT;
    int LenWiFi = sizeof(sock->ClientWiFi_TRANSMITTER);

    RAND_INT = shm_read(32, RAND_KEY);
    printf("RAND_INT WiFi: %s\n", RAND_INT);

    printf("transmitLTE socket: %d\n", sock->sockWiFi_TRANSMITTER);
    char test2[] = "test2";
    TX_WiFi = sendto(sock->sockWiFi_TRANSMITTER, test2, BUFFER, 0, (struct sockaddr *)&sock->ClientWiFi_TRANSMITTER, LenWiFi);

    // printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("Message from WiFi transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

/* Function to timestamp packets */
char *Timestamp() {
    /* Timestamp format : [hh:mm:ss dd/mm/yy] */
    struct tm *timeinfo;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    timeinfo = localtime(&tv.tv_sec);

    sprintf(curr_time, "[%d:%d:%d.%03ld %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tv.tv_usec / 1000, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    return curr_time;
}