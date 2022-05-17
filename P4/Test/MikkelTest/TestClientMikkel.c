#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define PORT 8888
#define IP "192.168.1.45"


//SETUP SOCKET
const char *LTE = "wwan0";
struct sockaddr_in Client;
int sockfd1;
int len = sizeof(Client);




int generate(int Min, int Max){
  int number = (rand() % ((Max+1) - Min)) + Min;
  return number;
}


int main() {
  srand(time(NULL));
  sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  setsockopt(sockfd1, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));

  Client.sin_family = AF_INET;
  Client.sin_port = htons(PORT);
  Client.sin_addr.s_addr = inet_addr(IP);

  while(1){
      int data = 0;
      int converted_data = 0;
      data = generate(1, 25000000);
      converted_data = htonl(data);

      sendto(sockfd1, &converted_data, sizeof(converted_data), 0, (struct sockaddr *)&Client, len);

      sleep(2);
  }
  close(sockfd1);
  return 1;


}