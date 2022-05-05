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
        while(!feof(pipe_WiFi)){
            if(fgets(Buffer,128,pipe) !=NULL){
                fseek(f,33,SEEK_SET);
                char RSSI[10];
                memset(RSSI,33,4,f);
                fread(RSSI,33,4,f);

            }
        }
        pclose(pipe_WiFi);
        Buffer[strlen(Buffer)-1] = '\0';
        printf("%s\n",Buffer);
        printf("%s\n",RSSI);

    }
    //Get RSRP value from LTE 
    FILE* pipe_LTE = popen("mmcli -m 0 --signal-get", "r");
    if (pipe1){
        char Buffer[128];
        while(!feof(pipe_LTE)){
            if(fgets(Buffer,128,pipe) !=NULL){
                fseek(f,20,SEEK_SET);
                char RSRP[10];
                memset(RSRP,20,55,f);
                fread(RSRP,20,55,f);
            }
        }
        pclose(pipe_LTE);
        Buffer[strlen(Buffer)-1] = '\0';
        printf("%s",Buffer);
        printf("%s\n",RSRP);
    }
    

}

