#include "Data.h"

void *LTE_Socket(void *arg)
{
	sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sockfd1, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
	if (sockfd1 == -1)
	{
		perror("Failed to create socket");
		exit(0);
	}

	Client1.sin_family = AF_INET;
	Client1.sin_port = htons(PORT);
	Client1.sin_addr.s_addr = inet_addr(IP1);

	//char TestMsg1[] = "This is LTE";
	int TestMsg1 = Generate_Data();
	sendto(sockfd1, TestMsg1, sizeof(TestMsg1), 0, (struct sockaddr *)&Client1, len1);

	close(sockfd1);
	pthread_exit(0);
}

void *WiFi_Socket(void *arg)
{
	sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sockfd2, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));
	if (sockfd2 == -1)
	{
		perror("Failed to create socket");
		exit(0);
	}

	Client2.sin_family = AF_INET;
	Client2.sin_port = htons(PORT);
	Client2.sin_addr.s_addr = inet_addr(IP2);

	int TestMsg2 = Generate_Data();
	//"This is WiFi";
	sendto(sockfd2, TestMsg2, sizeof(TestMsg2), 0, (struct sockaddr *)&Client2, len2);

	close(sockfd2);
	pthread_exit(0);
}

int Generate_Data(){
	int data = rand();
	return data;
}

int main()
{

	/* Create socket */
	/*
	sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sockfd1, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
	setsockopt(sockfd2, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

	if (sockfd1 || sockfd2 == -1) {
	  perror("Failed to create socket");
	  exit(0);
	} /*

	/* configure settings to communicate with remote UDP server */
	/*
	Client1.sin_family = AF_INET;
	Client1.sin_port = htons(PORT);
	Client1.sin_addr.s_addr = inet_addr(IP1);

	Client2.sin_family = AF_INET;
	Client2.sin_port = htons(PORT);
	Client2.sin_addr.s_addr = inet_addr(IP2);
	*/

	/* Main running code */
	while (1)
	{

		puts("Emergency exit: CTRL+C");
		printf("Sending data...\n");

		pthread_create(&T1, NULL, LTE_Socket, NULL);
		pthread_join(T1, NULL);
		pthread_create(&T2, NULL, WiFi_Socket, NULL);
		pthread_join(T2, NULL);
	}
	// close(sockfd1 && sockfd2);
	return 1;
}
