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
#define MAXBUF 1024
#define PORT 8888

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
struct sockaddr_in Server;
int sockfd, setsock;
unsigned int len = sizeof(Server);
char Buffer[MAXBUF];



int main() {

  /* Create socket */
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  setsock = setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));

  if (sockfd == -1) {
    perror("Failed to create socket");
    exit(0);
  }

  /* Configure settings to communicate with remote UDP server */
  Server.sin_family = AF_INET;
  Server.sin_port = htons(PORT);
  Server.sin_addr.s_addr = INADDR_ANY;

  /* Bind to socket */
  int b = bind(sockfd, (struct sockaddr*)&Server, sizeof(struct sockaddr));
  if (b == -1) {
    perror("Failed to bind");
    close(sockfd);
    exit(0);
  }

  /* Main running code */
  while (1)
  {
    puts("Emergency exit: CTRL+C");
    printf("Waiting for data...\n");

    int rc = recvfrom(sockfd, Buffer, MAXBUF, 0, (struct sockaddr*)&Server, &len);
    printf("%s\n \n", Buffer);

  }
  close(sockfd);
  return 1;
}
