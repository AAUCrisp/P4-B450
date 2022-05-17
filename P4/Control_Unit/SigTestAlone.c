#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>


int main() {

    FILE* pipe_WiFi = popen("iwconfig wlan1 | grep -i --color quality", "r");
    if (pipe){
        char Buffer[128];
        while(!feof(pipe_WiFi)){
            if(fgets(Buffer,128,pipe) !=NULL){}
        }
        pclose(pipe_WiFi);
        Buffer[strlen(Buffer)-1] = '\0';
        printf("%s",Buffer);
    }

    FILE* pipe_LTE = popen("mmcli -m 0 --signal-get", "r");
    if (pipe1){
        char Buffer[128];
        while(!feof(pipe_LTE)){
            if(fgets(Buffer,128,pipe) !=NULL){}
        }
        pclose(pipe_LTE);
        Buffer[strlen(Buffer)-1] = '\0';
        printf("%s",Buffer);
    }
    

}
