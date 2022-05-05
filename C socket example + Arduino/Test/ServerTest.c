#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

/* Define buffer and PORT number */
#define BUFFER 128
char Message[BUFFER];
uint PORT_LTE, PORT_WiFi;
char curr_time[BUFFER];

/* Specify LTE / WiFi interface */
const char *LTE;
const char *WiFi;

/* Misc */
struct sockaddr_in ServerLTE;
struct sockaddr_in ServerWiFi;
int sockLTE, lenLTE = sizeof(ServerLTE);
int sockWiFi, lenWiFi = sizeof(ServerWiFi);
int bindLTE, bindWiFi;
int rc_LTE, rc_WiFi;
pthread_t T1, T2;

void Create_Bind_Sockets(uint PORT_LTE, uint PORT_WiFi)
{
    /* Create socket */
    sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    if (sockLTE == -1)
    {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sockWiFi == -1)
    {
        perror("Failed to create sockLTE");
        exit(0);
    }

    /* Configure settings to communicate with remote UDP server */
    ServerLTE.sin_family = AF_INET;
    ServerLTE.sin_port = htons(PORT_LTE);
    ServerLTE.sin_addr.s_addr = INADDR_ANY;

    ServerWiFi.sin_family = AF_INET;
    ServerWiFi.sin_port = htons(PORT_WiFi);
    ServerWiFi.sin_addr.s_addr = INADDR_ANY;

    /* Bind to socket */
    bindLTE = bind(sockLTE, (struct sockaddr *)&ServerLTE, sizeof(struct sockaddr));
    bindWiFi = bind(sockWiFi, (struct sockaddr *)&ServerWiFi, sizeof(struct sockaddr));
    if (bindLTE == -1)
    {
        perror("Failed to bind LTE socket");
        exit(0);
    }
    if (bindWiFi == -1)
    {
        perror("Failed to bind WiFi socket");
        exit(0);
    }
    printf("Bind succesful\n");
}

void *receiveshit()
{
    rc_LTE = recvfrom(sockLTE, Message, BUFFER, 0, (struct sockaddr *)&ServerLTE, &lenLTE);
    printf("LTE-Thread id = %ld\n", pthread_self());
    printf("%s\n", Message);
    printf("Message from LTE received at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

void *receiveshit2()
{
    rc_WiFi = recvfrom(sockWiFi, Message, BUFFER, 0, (struct sockaddr *)&ServerWiFi, &lenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("%s\n", Message);
    printf("Message from WiFi received at: %s\n\n", curr_time);
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

void Check_RSSI_Value(){
    /* 
    1. Check RSSI value from LTE/WiFi.
    2. If LTE/WiFi is better than the other.
    3. Update Global Signal Variable (GSV).
    4. Send GSV to client via the better signal. 
    */
}

/* Main running code */
int main()
{
    /* Initialize PORT & INTERFACE*/
    PORT_LTE = 9123;
    PORT_WiFi = 9124;
    LTE = "wwan0";
    WiFi = "wlan0";

    /* Create sockets */
    Create_Bind_Sockets(PORT_LTE, PORT_WiFi);

    while (1)
    {
        Timestamp();
        pthread_create(&T1, NULL, receiveshit, NULL);
        pthread_create(&T2, NULL, receiveshit2, NULL);
        // pthread_join(T1, NULL);
        // pthread_join(T2, NULL);
    }

    close(sockLTE && sockWiFi);
    exit(0);
}