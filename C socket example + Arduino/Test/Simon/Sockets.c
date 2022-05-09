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

/* Define buffers & PORT number */
#define BUFFER 1024
char message[BUFFER];
char *receive[BUFFER];
// uint PORT_LTE, PORT_WiFi;
char curr_time[128];

/* Specify LTE & WiFi interface */
//const char *LTE = "wwan0";
//const char *WiFi = "wlan0";

/* Misc */
struct sockaddr_in ServerLTE;
struct sockaddr_in ServerWiFi;
int sockLTE, lenLTE = sizeof(ServerLTE);
int sockWiFi, lenWiFi = sizeof(ServerWiFi);
int bindLTE, bindWiFi;
int RX_LTE, RX_WiFi;
int TX_LTE, TX_WiFi;
pthread_t T1, T2;

/* Global Signal Variable & RSSI */
int GSV;
int RSSI = 1;

/* Function to bind sockets */
void Create_Bind_Sockets(int sockLTE, int sockWiFi, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Create socket */
    sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Setting up socket options & specifying interface */
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking */
    if (sockLTE == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sockWiFi == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }

    /* Configure settings to communicate with remote UDP client */
    ServerLTE.sin_family = AF_INET;
    ServerLTE.sin_port = htons(PORT_LTE);
    ServerLTE.sin_addr.s_addr = INADDR_ANY;

    ServerWiFi.sin_family = AF_INET;
    ServerWiFi.sin_port = htons(PORT_WiFi);
    ServerWiFi.sin_addr.s_addr = INADDR_ANY;

    /* Bind to socket */
    bindLTE = bind(sockLTE, (struct sockaddr *)&ServerLTE, sizeof(struct sockaddr));
    bindWiFi = bind(sockWiFi, (struct sockaddr *)&ServerWiFi, sizeof(struct sockaddr));

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
void *receiveLTE() {
    RX_LTE = recvfrom(sockLTE, message, BUFFER, 0, (struct sockaddr *)&ServerLTE, &lenLTE);
    printf("LTE-Thread id = %ld\n", pthread_self());
    printf("%s\n", message);
    printf("Message from LTE received at: %s\n\n", curr_time);
    //pthread_exit(NULL);
    receive = malloc(sizeof(receive));
    *receive = message;
    return (void *)receive;
}

/* Function to receive WiFi packets */
void *receiveWiFi() {
    RX_WiFi = recvfrom(sockWiFi, message, BUFFER, 0, (struct sockaddr *)&ServerWiFi, &lenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("%s\n", message);
    printf("Message from WiFi received at: %s\n\n", curr_time);
    //pthread_exit(NULL);
    receive = malloc(sizeof(receive));
    *receive = message;
    return (void *)receive;
}

/* Function to transmit GSV via LTE */
void *transmitLTE(int data, size_t buffer) {
    GSV = htonl(data);
    TX_LTE = sendto(sockLTE, GSV, buffer, 0, (struct sockaddr *)&ServerLTE, lenLTE);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("%d\n", GSV);
    printf("Message from WiFi transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

/* Function to transmit GSV via WiFi */
void *transmitWiFi(int data, size_t buffer) {
    GSV = htonl(data);
    TX_WiFi = sendto(sockWiFi, GSV, buffer, 0, (struct sockaddr *)&ServerWiFi, lenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("%d\n", GSV);
    printf("Message from WiFi transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

/* Function to timestamp packets */
char *Timestamp() {
    /* Timestamp format : [hh:mm:ss dd/mm/yy] */
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(curr_time, "[%d:%d:%d %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    return curr_time;
}