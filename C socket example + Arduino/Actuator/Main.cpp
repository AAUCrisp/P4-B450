#include <soc_conf.cpp>

int main()
{
   i = socket_configuration();
   if (i =! 0){
	   perror("failed to execute socket_configurations function");
	   exit(0);
   }


	/* Main running code */
	while (1)
	{

        std::thread T1(receive_LTE);
		std::thread T2(receive_WiFi);

	}
	close(sockLTE && sockWiFi);
	return 1;
}
