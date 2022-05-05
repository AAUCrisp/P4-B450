#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

char *Timestamp()
{
    // Timestamp format : [hh:mm:ss dd/mm/yy]
    char curr_time[1];
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(curr_time, "[%d:%d:%d %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    printf("reference &: %s\n", &curr_time);
    printf("pointer *: %s\n", *curr_time);
    printf("normal call?: %s\n", curr_time);

    return curr_time;
}

int main(){
    Timestamp();
}