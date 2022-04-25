#include <stdio.h>	
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <pthread.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXBUF 1024
#define PORT 8888

const char *LTE_interface = "wwan0";
const char *WiFi_interface = "wlan1";

struct sockaddr_in Server;
int sockfd, len = sizeof(Server);
char Buffer[MAXBUF];



int main(){
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, WiFi_interface, strlen(WiFi_interface));
    if (sockfd == -1){
        perror("Failed to create socket");
        exit(0);
    }

    Server.sin_family = AF_INET;
    Server.sin_port = htons(PORT);
    Server.sin_addr.s_addr = INADDR_ANY;

    int b = bind(sockfd, (struct sockaddr*)&Server, sizeof(struct sockaddr));
    if(b == -1){
        perror("Failed to bind");
        close(sockfd);
        exit(0);
    }

    while(1)
	{
		puts("Emergency exit: CTRL+C");
		printf("Waiting for data...\n");

        int rc = recvfrom(sockfd, Buffer, MAXBUF, 0, (struct sockaddr*)&Server, &len);
        printf("%s\n \n", Buffer);

    }
    close(sockfd);
    return 0;
}
