#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

int main(){
    
    printf("PID of HelloWorld.c: %d\n", getpid());
    int count;

    while (1){
        sleep(1);
        count++;
        printf("Hello world! %d\n", count);
    }
    //exit(0);
}