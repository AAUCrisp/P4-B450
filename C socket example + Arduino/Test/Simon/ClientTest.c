#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

/* Define buffer size, PORT number and server IP */
#define BUFFER 128
uint PORT_LTE, PORT_WiFi;
const char *IP_LTE;
const char *IP_WiFi;
char curr_time[BUFFER];

/* Specify LTE / WiFi interface */
const char *LTE;
const char *WiFi;

/* Misc */
struct sockaddr_in ClientLTE;
struct sockaddr_in ClientWiFi;
int sockLTE, lenLTE = sizeof(ClientLTE);
int sockWiFi, lenWiFi = sizeof(ClientWiFi);
int RX_LTE, RX_WiFi;
int TX_LTE, TX_WiFi;
pthread_t T1, T2;

/* Global Signal Variable & RSSI*/
int GSV;

void Create_Sockets(uint PORT_LTE, uint PORT_WiFi, const char *IP1, const char *IP2) {
    /* Defining sockets & specifying protocol */
    sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    /* Setting socket options & binding interfaces */
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    if (sockLTE == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sockWiFi == -1) {
        perror("Failed to create sockWiFi");
        exit(0);
    }

    ClientLTE.sin_family = AF_INET;
    ClientLTE.sin_port = htons(PORT_LTE);
    ClientLTE.sin_addr.s_addr = inet_addr(IP1);

    ClientWiFi.sin_family = AF_INET;
    ClientWiFi.sin_port = htons(PORT_WiFi);
    ClientWiFi.sin_addr.s_addr = inet_addr(IP2);
}

void *sendshit1String(void *msg1) {
    char TimestampLTE[BUFFER];
    char *newTextLTE = msg1;
    snprintf(TimestampLTE, BUFFER + 1, "%s %s", curr_time, newTextLTE);
    TX_LTE = sendto(sockLTE, TimestampLTE, BUFFER, 0, (struct sockaddr *)&ClientLTE, lenLTE);
    printf("LTE-Thread id = %ld \n", pthread_self());
    printf("%s\n\n", TimestampLTE);
    pthread_exit(NULL);
}

void *sendshit2String(void *msg2) {
    char TimestampWiFi[BUFFER];
    char *newTextWiFi = msg2;
    snprintf(TimestampWiFi, BUFFER + 1, "%s %s", curr_time, newTextWiFi);
    TX_WiFi = sendto(sockWiFi, TimestampWiFi, BUFFER, 0, (struct sockaddr *)&ClientWiFi, lenWiFi);
    printf("WiFi-Thread id = %ld \n", pthread_self());
    printf("%s\n\n", TimestampWiFi);

    pthread_exit(NULL);
}

char *Timestamp() {
    /* Timestamp format : [hh:mm:ss dd/mm/yy] */
    struct tm *timeinfo;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    timeinfo = localtime(&tv.tv_sec);


    sprintf(curr_time, "[%d:%d:%d.%03ld %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,tv.tv_usec/1000, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    return curr_time;
}
/*
void Update_GSV() {
    /*
    1. Receive message/GSV from Control Unit.
    2. Update local GSV.
    3. Notify to send via LTE/WiFi.

    if (recvfrom(sockLTE, &GSV, sizeof(GSV), 0, (struct sockaddr *)&ServerLTE, &lenLTE)) {
        printf("LTE-Thread id = %ld\n", pthread_self());
        printf("%s\n", Message);
        printf("Message from LTE received at: %s\n\n", curr_time);
    }

    if (recvfrom(sockWiFi, &GSV, sizeof(GSV), 0, (struct sockaddr *)&ServerWiFi, &lenWiFi)) {
        printf("WiFi-Thread id = %ld\n", pthread_self());
        printf("%s\n", Message);
        printf("Message from WiFi received at: %s\n\n", curr_time);
    }*/

/* Main running code */
int main() {
    /* Initialize PORT, IP & INTERFACE*/
    PORT_LTE = 9123;
    PORT_WiFi = 9124;
    IP_LTE = "10.20.0.16";
    IP_WiFi = "192.168.1.136";
    LTE = "wwan0";
    WiFi = "wlan0";
    int r;
    /* Create sockets */
    Create_Sockets(PORT_LTE, PORT_WiFi, IP_LTE, IP_WiFi);

    /* Messages to send */
    char TestMsg[] = "Client says hello via LTE!";
    char TestMsg2[] = "Client says hello via WiFi!";
    char SendMsg[128];
    char SendMsg2[128];
    srand(time(NULL));
    while (1) {
        sleep(1);
        r = rand() % 100;
        sprintf(SendMsg, "%s %d", TestMsg, r);
        sprintf(SendMsg2,"%s %d", TestMsg2, r);

        Timestamp();
        pthread_create(&T1, NULL, sendshit1String, SendMsg);
        pthread_join(T1, NULL);
        Timestamp();
        pthread_create(&T2, NULL, sendshit2String, SendMsg2);
        pthread_join(T2, NULL);
    }

    close(sockLTE && sockWiFi);
    exit(0);
}