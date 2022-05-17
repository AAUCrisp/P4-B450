#include <stdio.h>
#include <string.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

char curr_time[20];
char *Timestamp()
{
    /* Timestamp format : [hh:mm:ss dd/mm/yy] */
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(curr_time, "[%d:%d:%d %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    return curr_time;
}

int main(){
    //Timestamp();
    
    //format_time();
    //printf("shit works?: %s\n", format_time());

    //format_time();

    //char testMsg[] = "Hello this is message!";
    //char testMsg2[] = "2Hello this is message!";
    //strcat(curr_time, testMsg);
    //printf("%s\n", curr_time);
}