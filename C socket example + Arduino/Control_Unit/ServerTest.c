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
#define PORT1 8888
#define PORT2 8887

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
struct sockaddr_in Server1, sockaddr_in Server2;
int sockfd1, sockfd2;
int len1 = sizeof(Server1), len2 = sizeof(Server2);
char Buffer[MAXBUF];
int rc1, rc2;

void* LTE_Socket(void*){

  pthread_exit(0);
}

void* WiFi_Socket(void*){
  pthread_exit(0);
}

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

  /* Configure settings to communicate with remote UDP server */
  Server1.sin_family = AF_INET;
  Server1.sin_port = htons(PORT1);
  Server1.sin_addr.s_addr = INADDR_ANY;

  Server2.sin_family = AF_INET;
  Server2.sin_port = htons(PORT2);
  Server2.sin_addr.s_addr = INADDR_ANY;

  /* Bind to socket */
  int a = bind(sockfd1, (struct sockaddr*)&Server1, sizeof(struct sockaddr));
  int b = bind(sockfd2, (struct sockaddr*)&Server2, sizeof(struct sockaddr));
  if (a || b == -1) {
    perror("Failed to bind");
    close(sockfd1 && sockfd2);
    exit(0);
  }

  /* Main running code */
  while (1)
  {
    puts("Emergency exit: CTRL+C");
    printf("Waiting for data...\n");

    rc1 = recvfrom(sockfd1, Buffer, MAXBUF, 0, (struct sockaddr*)&Server1, &len1);
    rc2 = recvfrom(sockfd2, Buffer, MAXBUF, 0, (struct sockaddr*)&Server2, &len2);
    printf("%s\n \n", Buffer);

  }
  close(sockfd1 && sockfd2);
  return 1;
}
