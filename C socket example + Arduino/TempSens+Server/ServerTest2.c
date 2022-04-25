#include <stdio.h>	
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <pthread.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

char curr_time[21];
char* format_time(){ //Funktion til at formatere tid: [dd/mm/yy hh:mm:ss]
    time_t rawtime;
    struct tm * timeinfo;time ( &rawtime );
    timeinfo = localtime (&rawtime);
    sprintf(curr_time, "[%d/%d/%d %d:%d:%d]", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    return curr_time;
}
/*
void* WriteFile(void* arg){ 
    FILE *fp;  
	char *data = arg;

    fp = fopen("/home/simon/WorkshopFolder/TempSens+Server/Data.txt","a");
    //fp = open("/home/simon/WorkshopTestFolder/SSFS/Mounting_point/Data.txt","a");

    fprintf(fp,"%s %s C\n", format_time(), data);
    fclose(fp); 
    pthread_exit(NULL); 
} */

#define MAXBUF 1024
#define PORT 8888

struct sockaddr_in Server;
int sockfd, len = sizeof(Server);
char Buffer[MAXBUF];
pthread_t T1;

int main(){
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1){
        perror("Failed to create socket");
        exit(0);
    }

    Server.sin_family = AF_INET;
    Server.sin_port = htons(PORT);
    Server.sin_addr.s_addr = INADDR_ANY;

    int b = bind(sockfd, (struct sockaddr*)&Server, sizeof(struct sockaddr));
    if(b == -1){
        perror("Failed to bind");
        close(sockfd);
        exit(0);
    }

    while(1)
	{
		puts("Emergency exit: CTRL+C");
		printf("Waiting for data...\n");
        

        int rc = recvfrom(sockfd, Buffer, MAXBUF, 0, (struct sockaddr*)&Server, &len);
        printf("%s\n \n", Buffer);

        
		//pthread_create(&T1, NULL, WriteFile, Buffer);
        //pthread_join(T1, NULL);
        
        

    }
    close(sockfd);
    return 0;
}