#include "ActuatorServer.h"

#define SERVERPORT 8000
#define SERVERIP "192.168.1.143"
#define LTE_SERVERPORT 8001
#define LTE_SERVERIP "10.20.0.13"
struct sockaddr_in ClientWiFi, *res3;
struct sockaddr_in ClientLTE;
int sockWiFi, lenWiFi = sizeof(ClientWiFi);
int sockLTE, lenLTE = sizeof(ClientLTE);
int tx_WiFi, rx_WiFi;
int tx_LTE; 
int bindWiFi;

int rv;
int client_wifi_sock;
int client_LTE_sock;
int num_bytes;

char Coordinates[] = "10:5000";
char feedback[1024];


int main(int argc, char *argv[])
{   
    client_wifi_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_wifi_sock == -1)
    {
        perror("error created socket");
        exit(0);
    }

    memset(&ClientWiFi, 0, sizeof(ClientWiFi));
    ClientWiFi.sin_family = AF_INET;
    ClientWiFi.sin_port = htons(SERVERPORT);
    ClientWiFi.sin_addr.s_addr = inet_addr(SERVERIP);

    //bindWiFi = bind(client_wifi_sock, (struct sockaddr *)&ClientWiFi, sizeof(struct sockaddr));
   
   //_______________________________
   //           initializing Client LTE socket

    client_LTE_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_LTE_sock == -1)
    {
        perror("error created LTE socket");
        exit(0);
    }

    memset(&ClientLTE, 0, sizeof(ClientLTE));
    ClientLTE.sin_family = AF_INET;
    ClientLTE.sin_port = htons(LTE_SERVERPORT);
    ClientLTE.sin_addr.s_addr = inet_addr(LTE_SERVERIP); 


   
    while (1)
    {
        tx_WiFi = sendto(client_wifi_sock, Coordinates , strlen(Coordinates) ,0, (struct sockaddr *)&ClientWiFi , sizeof(ClientWiFi));
        if (tx_WiFi == -1)
        {
            perror("error sending data to server");
            exit(0);
        }

        tx_LTE = sendto(client_LTE_sock, Coordinates, sizeof(Coordinates), 0,(struct sockaddr *)&ClientLTE, sizeof(ClientLTE));
        if (tx_LTE == -1)
        {
            perror("error sending data to server via LTE");
            exit(0);
        }

        /*
        rx_WiFi = recvfrom(client_wifi_sock, feedback, 1024, 0, (struct sockaddr *)&ClientWiFi, &lenWiFi);
        if (rx_WiFi == -1)
        {
            perror("failed to receive feedback");
            exit(0);
        }

        printf("The robot has moved: %s\n", feedback);
        */
    }
    //printf("client: send %d bytes to %s\n", num_bytes, ClientWiFi.sin_addr.s_addr);
    close(client_wifi_sock); 
    close(client_LTE_sock);
    return 0;
}