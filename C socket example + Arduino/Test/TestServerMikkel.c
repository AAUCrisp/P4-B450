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
const char *LTE = "wlp2s0";

/* Misc */
struct sockaddr_in Server;
int sockfd1;
int len = sizeof(Server);
int rc;
int Buffer;



void Create_Bind_Socket_LTE() {
  /* Create socket */
  sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  setsockopt(sockfd1, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));

  if (sockfd1 == -1) {
    perror("Failed to create socket: LTE");
  }

  /* Configure settings to communicate with remote UDP server */
  Server.sin_family = AF_INET;
  Server.sin_port = htons(PORT1);
  Server.sin_addr.s_addr = INADDR_ANY;

  /* Bind to socket */
  int a = bind(sockfd1, (struct sockaddr *)&Server, sizeof(struct sockaddr));

  if (a  == -1) {
    perror("Failed to bind: LTE");
  }

}

int main() {
  Create_Bind_Socket_LTE();
  /* Main running code */
  while (1){

    puts("Emergency exit: CTRL+C");
    printf("Waiting for data...\n");

    rc = recvfrom(sockfd1, &Buffer, sizeof(Buffer), 0, (struct sockaddr *)&Server, &len);

    fprintf(stdout, "Sensor data: %d\n \n", ntohl(Buffer));

  }
  close(sockfd1);
  return 1;
}
