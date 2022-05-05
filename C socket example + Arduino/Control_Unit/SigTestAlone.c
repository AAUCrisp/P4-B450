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
    //int i;
    //int ii;
    /*
    char Buffer[50];
    int 50;
    int i = system(iwconfig $_[0]grep Signal| cut -d"=" -f3|cut -d " " -f1);
    int ii = popen(iwconfig $_[0]grep Signal| cut -d"=" -f3|cut -d " " -f1);
    if (fgets (Buffer, 50, ii)!=NULL){
        puts(Buffer);
        printf("Something happend...");
    }
    */

    FILE* pipe = popen("iwconfig wlan1 | grep -i --color quality", "r");
    if (pipe){
        char Buffer1[128];
        while(!feof(pipe)){
            if(fgets(Buffer1,128,pipe) !=NULL){}
        }
        pclose(pipe);
        Buffer1[strlen(Buffer1)-1] = '\0';
        printf("%s",Buffer1);
    }
    printf("%s",Buffer1);

}

