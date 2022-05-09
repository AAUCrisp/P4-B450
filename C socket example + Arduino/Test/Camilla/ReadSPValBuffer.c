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

int main(){

    char Buffer[] = "       Link Quality=70/70     Signal Level =-27 dBm";
    //printf(Buffer);
    Buffer[strlen(Buffer)-1] = '\0';
    printf("%s\n",Buffer);
    char Len = sizeof(Buffer)/1;
    //for (int i = 0; i < Len; i++){
    //    printf("%d \n", Buffer[i]);
    //}
    size_t l = 0;
    int result;

    while(Buffer[l] != '\0'){
        char * Minus = "-";
        int i = 0;
        result = strcmp(Minus,&Buffer[l]);
        if(result == 0){
            printf("Works");
            printf("%c \n", Buffer[l]);
        }
        //result = strcmp(Minus,&Buffer[i]);
        //if(Buffer[l] == Minus)
            //printf("%c \n", Buffer[l]);
        l++;
        //printf("%ld \n",l);
    }
}


