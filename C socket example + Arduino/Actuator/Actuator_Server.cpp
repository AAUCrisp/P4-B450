#include <soc_conf.cpp>

int main()
{
    /* configure settings to communicate with remote UDP server */
	Server_LTE.sin_family = AF_INET;
	Server_LTE.sin_port = htons(PORT);
	Server_LTE.sin_addr.s_addr = inet_addr(IP1);
	Server_WiFi.sin_family = AF_INET;
	Server_WiFi.sin_port = htons(PORT2);
	Server_WiFi.sin_addr.s_addr = inet_addr(IP2);


	/* Create socket and bind them to Interface*/
	sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	f = setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE_interface.c_str(), LTE_interface.length());
	g = setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi_interface.c_str(), WiFi_interface.length());

    /* Error Handling of socket creation, and interface binding*/
	if (sockLTE == -1)
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

	
    bindLTE = bind(sockLTE, (struct sockaddr *)&Server_LTE, sizeof(struct sockaddr));
    bindWiFi = bind(sockWiFi, (struct sockaddr *)&Server_WiFi, sizeof(struct sockaddr));


	/* Main running code */
	while (1)
	{

		//char TestMsg1[] = "This is LTE";
		//char TestMsg2[] = "This is WiFi";
		//sendto(sockfd1, TestMsg1, sizeof(TestMsg1), 0, (struct sockaddr *)&Client1, len1); // send the data to server
		//sendto(sockfd2, TestMsg2, sizeof(TestMsg2), 0, (struct sockaddr *)&Client2, len2); // send the data to server

        rc_LTE = recvfrom(sockLTE, ActuatorBuffer, MAXBUF, 0, (struct sockaddr*)&Server_LTE, &(lenLTE));

        rc_WiFi = recvfrom(sockWiFi, ActuatorBuffer, MAXBUF, 0,(struct sockaddr*)&Server_WiFi, &(lenWiFi));
    


	}
	close(sockLTE && sockWiFi);
	return 1;
}
