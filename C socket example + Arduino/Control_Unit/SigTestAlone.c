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

    FILE* pipe = popen("iwconfig wlan1 | grep -i --color quality", "r");
    if (pipe){
        char Buffer[128];
        while(!feof(pipe)){
            if(fgets(Buffer,128,pipe) !=NULL){}
        }
        pclose(pipe);
        Buffer[strlen(Buffer)-1] = '\0';
        printf("%s",Buffer);
    }

    FILE* pipe1 = popen("iwconfig wwan0 | grep -i --color quality", "r");
    if (pipe1){
        char Buffer[128];
        while(!feof(pipe1)){
            if(fgets(Buffer,128,pipe) !=NULL){}
        }
        pclose(pipe1);
        Buffer[strlen(Buffer)-1] = '\0';
        printf("%s",Buffer);
    }
    

}
