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

#include "Sockets_TX_RX.h"
#include "shm_write_read.h"

/* Define buffers & PORT number */
#define BUFFER 1024
char message[BUFFER];
char *receive;
char curr_time[128];

/* Misc */
int bindLTE, bindWiFi;
int RX_LTE, RX_WiFi;
int TX_LTE, TX_WiFi;

/* Function to bind sockets */
void Create_Bind_Sockets(Sockets *sock, uint PORT_LTE, uint PORT_WiFi, uint PORT_LTE_TRANS, uint PORT_WiFi_TRANS, const char *LTE, const char *WiFi) {
    /* Create socket receiver */
    sock->sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Create socket transmitter */
    sock->sockLTE_transmit = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_transmit = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Setting up socket options & specifying interface for transmitter */
    setsockopt(sock->sockLTE_transmit, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_transmit, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking */
    if (sock->sockLTE == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sock->sockWiFi == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sock->sockLTE_transmit == -1) {
        perror("Failed to create sockLTE_transmit");
        exit(0);
    }
    if (sock->sockWiFi_transmit == -1) {
        perror("Failed to create sockLTE_transmit");
        exit(0);
    }

    /* Configure settings to communicate with remote UDP client for receiver */
    sock->ServerLTE.sin_family = AF_INET;
    sock->ServerLTE.sin_port = htons(PORT_LTE);
    sock->ServerLTE.sin_addr.s_addr = INADDR_ANY;

    sock->ServerWiFi.sin_family = AF_INET;
    sock->ServerWiFi.sin_port = htons(PORT_WiFi);
    sock->ServerWiFi.sin_addr.s_addr = INADDR_ANY;

    /* Configure settings to communicate with remote UDP client for transmitter */
    sock->ServerLTE_transmit.sin_family = AF_INET;
    sock->ServerLTE_transmit.sin_port = htons(PORT_LTE_TRANS);
    sock->ServerLTE_transmit.sin_addr.s_addr = INADDR_ANY;

    sock->ServerWiFi_transmit.sin_family = AF_INET;
    sock->ServerWiFi_transmit.sin_port = htons(PORT_WiFi_TRANS);
    sock->ServerWiFi_transmit.sin_addr.s_addr = INADDR_ANY;

    /* Bind to socket */
    bindLTE = bind(sock->sockLTE, (struct sockaddr *)&sock->ServerLTE, sizeof(struct sockaddr));
    bindWiFi = bind(sock->sockWiFi, (struct sockaddr *)&sock->ServerWiFi, sizeof(struct sockaddr));

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

/* Function to receive LTE packets */
void *receiveLTE(void *socket) {
    printf("I am here");
    time_struct time1;
    time_struct time2;
    int timediff;
    char voidChar[128];
    char messageRecieved[128];
    Sockets *sock = (Sockets *)socket;
    int LenLTE = sizeof(sock->ServerWiFi);
    RX_LTE = recvfrom(sock->sockLTE, message, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE, &LenLTE);
    printf("LTE || LTE-Thread id = %ld\n", pthread_self());
    printf("LTE || Message from LTE received at: %s\n", curr_time);
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
}

/* Function to receive WiFi packets */
void *receiveWiFi(void *socket) {
    time_struct time1;
    time_struct time2;
    int timediff;
    char voidChar[128];
    char messageRecieved[128];
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ServerWiFi);
    RX_WiFi = recvfrom(sock->sockWiFi, message, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi, &LenWiFi);
    printf("WIFI || WiFi-Thread id = %ld\n", pthread_self());
    printf("WIFI || Message from WiFi received at: %s \n", curr_time);
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
}

// Function to transmit GSV via LTE
void *transmitLTE(void *socket) {
    Sockets *sock = (Sockets *)socket;
    int LenLTE = sizeof(sock->ServerLTE_transmit);
    const char *GSV;
    const char *GSV_KEY = "GSV_KEY";
    GSV = shm_read(10, GSV_KEY);
    TX_LTE = sendto(sock->sockLTE_transmit, GSV, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_transmit, LenLTE);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    // printf("%s\n", GSV);
    printf("Message from LTE transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

// Function to transmit GSV via WiFi
void *transmitWiFi(void *socket) {
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ServerWiFi_transmit);
    const char *GSV;
    const char *GSV_KEY = "GSV_KEY";
    GSV = shm_read(10, GSV_KEY);
    TX_WiFi = sendto(sock->sockWiFi_transmit, GSV, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi_transmit, LenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    // printf("%s\n", GSV);
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