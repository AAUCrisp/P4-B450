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
#define MAXBUF 64
#define PORT 8888
#define IP "10.20.0.16"

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
char message[MAXBUF];
struct sockaddr_in Client;
int sockfd, len = sizeof(Client);



int main() {

  /* Create socket */
  sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  setsockopt(sockfd1, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
  setsockopt(sockfd2, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

  if (sockfd1 || sockfd2 == -1) {
    perror("Failed to create socket");
    exit(0);
  }

  /* configure settings to communicate with remote UDP server */
  Client.sin_family = AF_INET;
  Client.sin_port = htons(PORT);
  Client.sin_addr.s_addr = inet_addr(IP);

  /* Main running code */
  while (1) {
    char TestMsg1[] = "This is LTE";
    char TestMsg2[] = "This is WiFi";
    sendto(sockfd1, TestMsg1, sizeof(TestMsg1), 0, (struct sockaddr *)&Client, len); //send the data to server
    sendto(sockfd2, TestMsg2, sizeof(TestMsg2), 0, (struct sockaddr *)&Client, len); //send the data to server
  }
  close(sockfd);
  return 1;
}
