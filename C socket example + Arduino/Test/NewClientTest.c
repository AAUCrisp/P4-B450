#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

/* Define buffer size, PORT number and server IP */
const int BUFFER;
uint PORT1, PORT2;
const char *IP_LTE;
const char *IP_WiFi;

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
struct sockaddr_in ClientLTE;
struct sockaddr_in ClientWiFi;
int sockLTE, lenLTE = sizeof(ClientLTE);
int sockWiFi, lenWiFi = sizeof(ClientWiFi);
int rc_LTE, rc_WiFi;
pthread_t T1, T2;
char curr_time[BUFFER];

void Create_Sockets(uint PORT_LTE, uint PORT_WiFi, const char *IP1, const char *IP2)
{
    /* Defining sockets & specifying protocol */
    sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    /* Setting socket options & binding interfaces */
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    if (sockLTE == -1)
    {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sockWiFi == -1)
    {
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

void *sendshit1String()
{
    // char TestMsg[] = "Client says hello via LTE!";
    printf("%s\n", TestMsg);
    sendto(sockLTE, TestMsg, sizeof(TestMsg), 0, (struct sockaddr *)&ClientLTE, lenLTE);
    printf("LTE-Thread id = %ld\n", pthread_self());
    pthread_exit(NULL);
}

void *sendshit2String(char msg)
{
    // char TestMsg2[] = "Client says hello via WiFi!";
    strcat(curr_time, msg);
    printf("%s\n", msg);
    printf("%s\n", curr_time);
    sendto(sockWiFi, curr_time, sizeof(curr_time), 0, (struct sockaddr *)&ClientWiFi, lenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    pthread_exit(NULL);
}

char *Timestamp()
{
    /* Timestamp format : [hh:mm:ss dd/mm/yy] */
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(curr_time, "[%d:%d:%d %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    return curr_time;
}

/* Main running code */
int main()
{
    /* Initialize variables */
    PORT1 = 9123;
    PORT2 = 9124;
    IP_LTE = "10.20.0.16";
    IP_WiFi = "192.168.1.131";
    BUFFER = 128;

    /* Create sockets */
    Create_Sockets(PORT1, PORT2, IP_LTE, IP_WiFi);

    /* Messages to send */
    char TestMsg[] = "Client says hello via LTE!";
    char TestMsg2[] = "Client says hello via WiFi!";

    while (1)
    {
        sleep(1);
        pthread_create(&T1, NULL, sendshit1String, NULL);
        pthread_create(&T2, NULL, sendshit2String, TestMsg2);
        pthread_join(T1, NULL);
        pthread_join(T2, NULL);
    }
    close(sockLTE && sockWiFi);
    exit(0);
}