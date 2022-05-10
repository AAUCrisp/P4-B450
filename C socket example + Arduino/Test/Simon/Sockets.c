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
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "Sockets_TX_RX.h"

/* Define buffers & PORT number */
#define BUFFER 1024
char message[BUFFER];
char *receive;
// uint PORT_LTE, PORT_WiFi;
char curr_time[128];

/* Specify LTE & WiFi interface */
// const char *LTE = "wwan0";
// const char *WiFi = "wlan0";

/* Misc */
// int lenLTE = sizeof(ServerLTE);
// int lenWiFi = sizeof(ServerWiFi);
int bindLTE, bindWiFi;
int RX_LTE, RX_WiFi;
int TX_LTE, TX_WiFi;
pthread_t T1, T2;

/* Global Signal Variable & RSSI */
int GSV;
int RSSI = 1;


/* Function to bind sockets */
void Create_Bind_Sockets(Sockets *sock, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Create socket */
    sock->sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    /* Setting up socket options & specifying interface */
    setsockopt(sock->sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking */
    if (sock->sockLTE == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sock->sockWiFi == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }

    /* Configure settings to communicate with remote UDP client */
    sock->ServerLTE.sin_family = AF_INET;
    sock->ServerLTE.sin_port = htons(PORT_LTE);
    sock->ServerLTE.sin_addr.s_addr = INADDR_ANY;

    sock->ServerWiFi.sin_family = AF_INET;
    sock->ServerWiFi.sin_port = htons(PORT_WiFi);
    sock->ServerWiFi.sin_addr.s_addr = INADDR_ANY;

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
    time_struct time1;
    time_struct time2;
    int timediff;
    char voidChar[128];
    Sockets *sock = (Sockets *)socket;
    int LenLTE = sizeof(sock->ServerWiFi);
    RX_LTE = recvfrom(sock->sockLTE, message, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE, &LenLTE);
    printf("LTE-Thread id = %ld\n", pthread_self());
    Timestamp();
    printf("Message from LTE received at: %s\n", curr_time);
    sscanf(curr_time,"%c %d %c %d %c %d %c %d", voidChar, &time1.hour, voidChar, &time1.minute, voidChar, &time1.second, voidChar, &time1.millis);
    sscanf(message,"%c %d %c %d %c %d %c %d", voidChar, &time2.hour, voidChar, &time2.minute, voidChar, &time2.second, voidChar, &time2.millis);
    timediff = (((time1.hour * 3600000) + (time1.minute * 60000) + (time1.second * 1000) + (time1.millis)) - ((time2.hour * 3600000) + (time2.minute * 60000) + (time2.second * 1000) + (time2.millis)) );
    if (timediff >= 0)
    {
        printf("Time difference: %d milliseconds \n\n", timediff);
    } else{
        printf("Timediff not available?\n\n");
    }
    // pthread_exit(NULL);
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
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ServerWiFi);
    RX_WiFi = recvfrom(sock->sockWiFi, message, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi, &LenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    Timestamp();
    printf("Message from WiFi received at: %s \n", curr_time);
    sscanf(curr_time,"%c %d %c %d %c %d %c %d", voidChar, &time1.hour, voidChar, &time1.minute, voidChar, &time1.second, voidChar, &time1.millis);
    sscanf(message,"%c %d %c %d %c %d %c %d", voidChar, &time2.hour, voidChar, &time2.minute, voidChar, &time2.second, voidChar, &time2.millis);
    timediff = (((time1.hour * 3600000) + (time1.minute * 60000) + (time1.second * 1000) + (time1.millis)) - ((time2.hour * 3600000) + (time2.minute * 60000) + (time2.second * 1000) + (time2.millis)) );
    if (timediff >= 0)
    {
        printf("Time difference: %d milliseconds \n\n", timediff);
    } else{
        printf("Timediff not available?\n\n");
    }
    // pthread_exit(NULL);
    receive = malloc(sizeof(receive));
    receive = message;
    return (void *)receive;
    ;
}
/*
//Function to transmit GSV via LTE
void *transmitLTE(int data, void *sockLTE) {
    GSV = htonl(data);
    TX_LTE = sendto(sockLTE, &GSV, BUFFER, 0, (struct sockaddr *)&ServerLTE, lenLTE);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("%d\n", GSV);
    printf("Message from WiFi transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

//Function to transmit GSV via WiFi
void *transmitWiFi(int data, int sockWiFi) {
    GSV = htonl(data);
    TX_WiFi = sendto(sockWiFi, &GSV, BUFFER, 0, (struct sockaddr *)&ServerWiFi, lenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("%d\n", GSV);
    printf("Message from WiFi transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}
*/
/* Function to timestamp packets */
char *Timestamp() {
    /* Timestamp format : [hh:mm:ss dd/mm/yy] */
    struct tm *timeinfo;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    timeinfo = localtime(&tv.tv_sec);


    sprintf(curr_time, "[%d:%d:%d.%03ld %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,tv.tv_usec/1000, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    return curr_time;
}