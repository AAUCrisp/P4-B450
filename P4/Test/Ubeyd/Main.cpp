#include "soc_conf.cpp"
void *receive_LTE(void *){
   // const char loading = "LTE here.. i am receiving: ";
    printf("LTE loadning... message is: \n\n");
    rc_LTE = recvfrom(sockLTE, ActuatorBufferLTE, MAXBUF, 0, (struct sockaddr*)&Server_LTE, &(lenLTE)); 
	printf("LTE After Receiving\n\n");
    printf("%s\n", ActuatorBufferLTE);
    pthread_exit(NULL);
}

void *receive_WiFi(void * ) {
    printf("i am inside the wifi functions \n\n");
    rc_WiFi = recvfrom(sockWiFi, ActuatorBufferWiFi, MAXBUF, 0,(struct sockaddr*)&Server_WiFi, &(lenWiFi));
	printf("WiFi After Receiving\n\n");
    printf("%d\n",rc_WiFi);
    printf("%s\n", ActuatorBufferWiFi);
    pthread_exit(NULL);
}
/*
void create_threads() {
        pthread_create(&T1, NULL, receive_LTE, NULL);
	pthread_create(&T2, NULL, receive_LTE, NULL);
	pthread_join(T1, NULL);
	pthread_join(T2, NULL);
}
*/

int main()
{

  socket_configuration();
  if (i == -1){
	   perror("failed to execute socket_configurations function");
	   exit(0);
  }

//printf("FED LORT 1");

	/* Main running code */
	while(1)
	{
	//	receive_WiFi();
	//	receive_LTE();
	//	printf("FED LORT\n\n");
		//LORT();
	pthread_create(&T1, NULL, receive_LTE, NULL);
        pthread_create(&T2, NULL, receive_WiFi, NULL);
       // pthread_join(T1, NULL);
       // pthread_join(T2, NULL);

        //std::thread T1(receive_LTE);
	//std::thread T2(receive_WiFi);

		//T1.join();
		//T2.join();

	// rc_LTE = recvfrom(sockLTE, ActuatorBuffer, MAXBUF, 0, (struct sockaddr*)&Server_LTE, &(lenLTE));
    //std::string msg = "Alley oop form kyrie";
    //printf("%s\n",msg);
   	// printf("%s\n", ActuatorBuffer);
   	 




	}
	close(sockLTE && sockWiFi);
	return 1;
}
