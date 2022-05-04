#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define PORT1 8888
#define PORT2 8887
#define IP1 "10.20.0.16"
#define IP2 "192.168.1.131"


//SETUP SOCKET
const char *LTE = "wwan0";
const char *WiFi = "wlan0";
struct sockaddr_in ClientLTE;
struct sockaddr_in ClientWiFi;
int sockLTE;
int sockWiFi;
int lenLTE = sizeof(ClientLTE);
int lenWiFi = sizeof(ClientWiFi);
int rc_LTE, rc_WiFi;
pthread_t T1, T2, T3, T4;


int data = 0;
int converted_data = 0;


int generate(int Min, int Max){
  int number = (rand() % ((Max+1) - Min)) + Min;
  return number;
}

void Create_Socket_LTE()
{
	/* Create socket */
	sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));

	if (sockLTE == -1)
	{
		perror("Failed to create socket: LTE");
		exit(0);
	}
	else
	{
		printf("LTE socket was created successfully\n");
	}

	/* configure settings to communicate with remote UDP server */
	ClientLTE.sin_family = AF_INET;
	ClientLTE.sin_port = htons(PORT1);
	ClientLTE.sin_addr.s_addr = inet_addr(IP1);
}

void Create_Socket_WiFi()
{
	/* Create socket */
	sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

	if (sockWiFi == -1)
	{
		perror("Failed to create socket: WiFi");
		exit(0);
	}
	else
	{
		printf("WiFi socket was successful! \n");
	}

	/* configure settings to communicate with remote UDP server */
	ClientWiFi.sin_family = AF_INET;
	ClientWiFi.sin_port = htons(PORT2);
	ClientWiFi.sin_addr.s_addr = inet_addr(IP2);
}

  void *Send_Data_LTE(void *buffer)
  {
  	sendto(sockLTE, buffer, sizeof(buffer), 0, (struct sockaddr *)&ClientLTE, lenLTE); // send the data to server

  }

  void *Send_Data_WiFi(void *buffer)
  {
  	sendto(sockWiFi, buffer, sizeof(buffer), 0, (struct sockaddr *)&ClientWiFi, lenWiFi); // send the data to server

  }


int main() {
  srand(time(NULL));

  Create_Socket_LTE();
  Create_Socket_WiFi();

  char TestMsg1[] = "This is LTE something test write this shit";
	char TestMsg2[] = "This is WiFi something test write this shit";



  while(1){
    //data = generate(1, 25000000);
    //converted_data = htonl(data);

    pthread_create(&T3, NULL, Send_Data_LTE, &TestMsg1);
		pthread_join(T3, NULL);
    //free(T3);
		pthread_create(&T4, NULL, Send_Data_WiFi, &TestMsg2);
		pthread_join(T4, NULL);
    //free(T4);


    sleep(2);
  }
  close(sockLTE && sockWiFi);
  return 1;


}
