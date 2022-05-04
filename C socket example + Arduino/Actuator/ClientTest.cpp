#include <Actuator_header.h>

int main()
{

	/* Create socket */
	sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	f = setsockopt(sockfd1, SOL_SOCKET, SO_BINDTODEVICE, LTE, str.length());
	g = setsockopt(sockfd2, SOL_SOCKET, SO_BINDTODEVICE, WiFi, str2.length());

	if (sockfd1 == -1)
	{
		perror("Failed to create socket");
		exit(0);
	}

	if (f == -1) 
	{
		perror("Error with LTE...the error is");
		return(-1);
		exit(0);
	} 

	if (g == -1)

	{
		perror("Error with WiFi... the error is");
		return(-1);
		exit(0);
	}

	/* configure settings to communicate with remote UDP server */
	Client1.sin_family = AF_INET;
	Client1.sin_port = htons(PORT);
	Client1.sin_addr.s_addr = inet_addr(IP1);
	Client2.sin_family = AF_INET;
	Client2.sin_port = htons(PORT2);
	Client2.sin_addr.s_addr = inet_addr(IP2);

	/* Main running code */
	while (1)
	{
		//char TestMsg1[] = "This is LTE";
		//char TestMsg2[] = "This is WiFi";
		//sendto(sockfd1, TestMsg1, sizeof(TestMsg1), 0, (struct sockaddr *)&Client1, len1); // send the data to server
		//sendto(sockfd2, TestMsg2, sizeof(TestMsg2), 0, (struct sockaddr *)&Client2, len2); // send the data to server




	}
	close(sockfd1 && sockfd2);
	return 1;
}
