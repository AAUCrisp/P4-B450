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
#define MAXBUF 64
char Buffer[MAXBUF];
uint PORT;

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
struct sockaddr_in ServerLTE;
struct sockaddr_in ServerWiFi;
int sockLTE, lenLTE = sizeof(ServerLTE);
int sockWiFi, lenWiFi = sizeof(ServerWiFi);
int bindLTE, bindWiFi;
int rc_LTE, rc_WiFi;
pthread_t T1, T2;

void Create_Bind_Sockets(uint PORT)
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
    ServerLTE.sin_port = htons(PORT);
    ServerLTE.sin_addr.s_addr = INADDR_ANY;

    ServerWiFi.sin_family = AF_INET;
    ServerWiFi.sin_port = htons(PORT);
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
}

void *receiveshit()
{
    rc_LTE = recvfrom(sockLTE, Buffer, MAXBUF, 0, (struct sockaddr *)&ServerLTE, &lenLTE);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("%s\n \n", Buffer);
    pthread_exit(NULL);
}

void *receiveshit2()
{
    rc_WiFi = recvfrom(sockWiFi, Buffer, MAXBUF, 0, (struct sockaddr *)&ServerWiFi, &lenWiFi);
    printf("LTE-Thread id = %ld\n", pthread_self());
    printf("%s\n \n", Buffer);
    pthread_exit(NULL);
}

/* Main running code */
int main()
{
    /* Initialize variables */
    PORT = 6000;

    /* Create sockets */
    Create_Bind_Sockets(PORT);

    while (1)
    {
        pthread_create(&T1, NULL, receiveshit, NULL);
        pthread_create(&T2, NULL, receiveshit2, NULL);
        // pthread_join(T1, NULL);
        // pthread_join(T2, NULL);
    }

    close(sockLTE && sockWiFi);
    exit(0);
}
