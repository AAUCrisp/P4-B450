#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


int main() {
    //Get RSSI value from WiFi
    FILE* pipe_WiFi = popen("iwconfig wlan1 | grep -i --color quality", "r");
    if (pipe){
        char Buffer[128];
        char RSSI[10]= {0};
        while(!feof(pipe_WiFi)){
            if(fgets(Buffer,128,pipe_WiFi) !=NULL){
                fseek(pipe_WiFi,33,SEEK_SET);
                fread(RSSI,33,4,pipe_WiFi);
            }
        }
        pclose(pipe_WiFi);
        Buffer[strlen(Buffer)-1] = '\0';
        printf("%s\n",Buffer);
        printf("%s\n",RSSI);

    }
    //Get RSRP value from LTE 
    FILE* pipe_LTE = popen("mmcli -m 0 --signal-get", "r");
    if (pipe_LTE){
        char Buffer[128];
        char RSRP[10] = {0};
        while(!feof(pipe_LTE)){
            if(fgets(Buffer,128,pipe_LTE) !=NULL){
                fseek(pipe_LTE,20,SEEK_SET);
                fread(RSRP,20,55,pipe_LTE);
            }
        }
        pclose(pipe_LTE);
        Buffer[strlen(Buffer)-1] = '\0';
        printf("%s",Buffer);
        printf("%s\n",RSRP);
    }
    

}


