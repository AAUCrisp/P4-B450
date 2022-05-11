#include "ActuatorServer.h"

#define SERVERPORT 8000
#define SERVERIP "192.168.1.145"
struct sockaddr_in ClientWiFi, *res3;
int sockWiFi, lenWiFi = sizeof(ClientWiFi);
int tx_WiFi; rx_WiFi;
int bindWiFi;

int rv;
int client_wifi_sock;
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

    bindWiFi = bind(client_wifi_sock, (struct sockaddr *)&ClientWiFi, sizeof(struct sockaddr));
   
   
    while (1)
    {
        num_bytes = sendto(client_wifi_sock, Coordinates , strlen(Coordinates) ,0, (struct sockaddr *)&ClientWiFi , sizeof(ClientWiFi));
        if (num_bytes == -1)
        {
            perror("error sending data to server");
            exit(0);
        }

        rx_WiFi = recvfrom(client_wifi_sock, feedback, 1024, 0, (struct sockaddr *)&ClientWiFi, &lenWiFi);
        if (rx_WiFi == -1)
        {
            perror("failed to receive feedback");
            exit(0);
        }

        printf("The robot has moved: %s\n", feedback);

    }
    //printf("client: send %d bytes to %s\n", num_bytes, ClientWiFi.sin_addr.s_addr);
    close(client_wifi_sock); 
    return 0;
}