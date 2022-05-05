#include <soc_conf.cpp>

int main()
{
   socket_configuration();
   if (i =! -1){
	   perror("failed to execute socket_configurations function");
	   exit(0);
   }
   


	/* Main running code */
	while (1)
	{

        std::thread T1(receive_LTE);
		std::thread T2(receive_WiFi);

		T1.join();
		T2.join();

	}
	close(sockLTE && sockWiFi);
	return 1;
}
