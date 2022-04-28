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
#define IP "Insert IP here"

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
char message[MAXBUF];
struct sockaddr_in Client;
int sockfd, len = sizeof(Client);



int main() {

  /* Create socket */
  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, "Insert interface here", strlen("Insert interface here"));

  if (sockfd == -1) {
    perror("Failed to create socket");
    exit(0);
  }

  /* configure settings to communicate with remote UDP server */
  Client.sin_family = AF_INET;
  Client.sin_port = htons(Port);
  Client.sin_addr.s_addr = inet_addr(IP);

  /* Main running code */
  while (1) {
    char TestMsg[] = "Hello does this work?";
    sendto(sockfd, TestMsg, sizeof(TestMsg), 0, (struct sockaddr *)&Client, len); //send the data to server
  }
  exit(0);
}
