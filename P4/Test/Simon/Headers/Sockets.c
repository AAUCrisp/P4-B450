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

#include "Sockets.h"
#include "shm_write_read.h"

/* Define buffers & PORT number */
#define BUFFER 1024
char message_LTE[BUFFER];
char message_WiFi[BUFFER];
char *receive_LTE;
char *receive_WiFi;
char curr_time[128];

/* Misc */
int bindLTE, bindWiFi;
int RX_LTE, RX_WiFi;
int TX_LTE, TX_WiFi;

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

void Sockets_Receiver(Sockets *sock, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Time struct for socket timeout */
    struct timeval tv2;
    tv2.tv_sec = 0;
    tv2.tv_usec = 500000;
    
    /* Create socket receiver */
    sock->sockLTE_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("Sockets_Receiver LTE (INSIDE): %d\n", sock->sockLTE_RECEIVER);
    printf("Sockets_Receiver WiFi (INSIDE): %d\n", sock->sockWiFi_RECEIVER);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));
    setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));
    setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));

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
    int LenLTE = sizeof(sock->ServerLTE_RECEIVER);

    printf("receiveLTE socket: %d\n", sock->sockLTE_RECEIVER);
    RX_LTE = recvfrom(sock->sockLTE_RECEIVER, message_LTE, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_RECEIVER, &LenLTE);
    Timestamp();

    // printf("LTE || LTE-Thread id = %ld\n", pthread_self());
    printf("LTE || Message from LTE received at: %s\n", curr_time);
    printf("LTE || Message: %s Control Unit\n", message_LTE);
    receive_LTE = malloc(sizeof(receive_LTE));
    receive_LTE = message_LTE;
    printf("receiveLTE receive message: %s\n", receive_LTE);
    return (void *)receive_LTE;
}
/*void *receiveLTE(void *socket) {
    time_struct time1;
    time_struct time2;
    int timediff;
    char voidChar[128];
    char messageRecieved[128];
    Sockets *sock = (Sockets *)socket;
    int LenLTE = sizeof(sock->ServerLTE_RECEIVER);
    RX_LTE = recvfrom(sock->sockLTE_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_RECEIVER, &LenLTE);
    printf("LTE || LTE-Thread id = %ld\n", pthread_self());
    printf("LTE || Message from LTE received at: %s\n", curr_time);
    printf("LTE || Message from LTE: %s\n", message);

    sscanf(curr_time, "%c %d %c %d %c %d %c %d", voidChar, &time1.hour, voidChar, &time1.minute, voidChar, &time1.second, voidChar, &time1.millis);
    sscanf(message, "%c %d %c %d %c %d %c %d", voidChar, &time2.hour, voidChar, &time2.minute, voidChar, &time2.second, voidChar, &time2.millis);
    sscanf(message, "%s %s %[^\n]", voidChar, voidChar, messageRecieved);
    printf("LTE || The message received was: %s \n", messageRecieved);
    timediff = (((time1.hour * 3600000) + (time1.minute * 60000) + (time1.second * 1000) + (time1.millis)) - ((time2.hour * 3600000) + (time2.minute * 60000) + (time2.second * 1000) + (time2.millis)));
    if (timediff >= 0) {
        printf("LTE || Time difference: %d milliseconds \n\n", timediff);
    } else {
        printf("LTE || Timediff not available? %d\n\n", timediff);
    }

    receive = malloc(sizeof(receive));
    receive = message;
    return (void *)receive;
    ;
}*/

/* Function to receive WiFi packets */
void *receiveWiFi(void *socket) {
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ServerWiFi_RECEIVER);

    printf("receiveWiFi socket: %d\n", sock->sockWiFi_RECEIVER);
    RX_WiFi = recvfrom(sock->sockWiFi_RECEIVER, message_WiFi, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, &LenWiFi);
    Timestamp();

    // printf("WiFi || WiFi-Thread id = %ld\n", pthread_self());
    printf("WiFi || Message from WiFi received at: %s \n", curr_time);
    printf("WiFi || Message: %s Control Unit\n", message_WiFi);
    receive_WiFi = malloc(sizeof(receive_WiFi));
    receive_WiFi = message_WiFi;
    printf("receiveWiFi receive message: %d\n", receive_WiFi);
    return (void *)receive_WiFi;
}
/*
void *receiveWiFi(void *socket) {
    time_struct time1;
    time_struct time2;
    int timediff;
    char voidChar[128];
    char messageRecieved[128];
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ServerWiFi_RECEIVER);
    RX_WiFi = recvfrom(sock->sockWiFi_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, &LenWiFi);
    printf("WIFI || WiFi-Thread id = %ld\n", pthread_self());
    printf("WIFI || Message from WiFi received at: %s \n", curr_time);
    printf("WIFI || Message from WiFi: %s \n", message);

    sscanf(curr_time, "%c %d %c %d %c %d %c %d", voidChar, &time1.hour, voidChar, &time1.minute, voidChar, &time1.second, voidChar, &time1.millis);
    sscanf(message, "%c %d %c %d %c %d %c %d", voidChar, &time2.hour, voidChar, &time2.minute, voidChar, &time2.second, voidChar, &time2.millis);
    sscanf(message, "%s %s %[^\n]", voidChar, voidChar, messageRecieved);
    printf("WIFI || The message recieved was: %s \n", messageRecieved);
    timediff = (((time1.hour * 3600000) + (time1.minute * 60000) + (time1.second * 1000) + (time1.millis)) - ((time2.hour * 3600000) + (time2.minute * 60000) + (time2.second * 1000) + (time2.millis)));
    if (timediff >= 0) {
        printf("WIFI || Time difference: %d milliseconds \n\n", timediff);

    } else {
        printf("WIFI || Timediff not available? %d\n\n", timediff);
    }

    receive = malloc(sizeof(receive));
    receive = message;
    return (void *)receive;
    ;
}*/

// Function to transmit GSV via LTE
void *transmitLTE(void *socket) {
    Sockets *sock = (Sockets *)socket;
    int LenLTE = sizeof(sock->sockLTE_TRANSMITTER);
    const char *GSV;
    const char *GSV_KEY = "GSV_KEY";
    GSV = shm_read(32, GSV_KEY);
    printf("transmitLTE socket: %d\n", sock->sockLTE_TRANSMITTER);
    TX_LTE = sendto(sock->sockLTE_TRANSMITTER, GSV, BUFFER, 0, (struct sockaddr *)&sock->ClientLTE_TRANSMITTER, LenLTE);
    // printf("LTE-Thread id = %ld\n", pthread_self());
    printf("Shared memory object LTE: %s\n", GSV);
    printf("Message from LTE transmitted at: %s\n", curr_time);
    pthread_exit(NULL);
}

// Function to transmit GSV via WiFi
void *transmitWiFi(void *socket) {
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ClientWiFi_TRANSMITTER);
    const char *GSV;
    const char *GSV_KEY = "GSV_KEY";
    GSV = shm_read(32, GSV_KEY);
    printf("transmitWiFi socket: %d\n", sock->sockWiFi_TRANSMITTER);
    TX_WiFi = sendto(sock->sockWiFi_TRANSMITTER, GSV, BUFFER, 0, (struct sockaddr *)&sock->ClientWiFi_TRANSMITTER, LenWiFi);
    // printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("Shared memory object WiFi: %s\n", GSV);
    printf("Message from WiFi transmitted at: %s\n", curr_time);
    pthread_exit(NULL);
}

