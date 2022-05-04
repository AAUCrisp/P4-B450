#include <soc_conf.cpp>

int main()
{
   socket_configuration();


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
