#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

//#define MAXBUF 64
//GET REKT NOOB

int main() {
  int Port = 8888;
  const char *IP = ""; // IP address of server
  int MAXBUF;

  char message[MAXBUF];
  struct sockaddr_in Client;
  int sockfd, len = sizeof(Client);

  const char *LTE = "wwan0";
  const char *WiFi = "wlan0";

  sockfd = socket(PF_INET, SOCK_DGRAM, 0); // create a UDP socket
  setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi)); // Change last two parameters for either LTE / WiFi
  
  if (sockfd == -1) {
      perror("Failed to create socket");
      exit(0);
  }

  /* configure settings to communicate with remote UDP server */
  Client.sin_family = AF_INET;
  Client.sin_port = htons(Port);
  Client.sin_addr.s_addr = inet_addr(IP);

  while (1) {
    char TestMsg[] = "Hello does this work?";
    sendto(sockfd, TestMsg, sizeof(TestMsg), 0, (struct sockaddr *)&Client, len); //send the data to server
  }
 exit(0);
}
