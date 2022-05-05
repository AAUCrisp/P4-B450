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
        char BufferWiFi[128];
        char RSSI[10]= {0};
        while(!feof(pipe_WiFi)){
            if(fgets(BufferWiFi,128,pipe_WiFi) !=NULL){
                fseek(pipe_WiFi,33,SEEK_SET);
                fread(RSSI,33,4,pipe_WiFi);
            }
        }
        pclose(pipe_WiFi);
        BufferWiFi[strlen(BufferWiFi)-1] = '\0';
        printf("%s\n",BufferWiFi);
        RSSI[strlen(RSSI))-1] = '\0';
        printf("%s\n",RSSI);

    }
    //Get RSRP value from LTE 
    FILE* pipe_LTE = popen("mmcli -m 0 --signal-get", "r");
    if (pipe_LTE){
        char BufferLTE[128];
        char RSRP[10] = {0};
        while(!feof(pipe_LTE)){
            if(fgets(BufferLTE,128,pipe_LTE) !=NULL){
                fseek(pipe_LTE,50,SEEK_SET);
                fread(RSRP,50,55,pipe_LTE);
            }
        }
        pclose(pipe_LTE);
        BufferLTE[strlen(BufferLTE)-1] = '\0';
        printf("%s",BufferLTE);
        RSRP[strlen(RSRP)-1] = '\0';
        printf("%s\n",RSRP);
    }
    

}



