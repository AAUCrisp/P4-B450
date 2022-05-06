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



int main()
{
    pid_t child_pid;
    child_pid = fork();
    

    if(child_pid == 0){
    printf("PID of parent_pid: %d\n", getppid());
    printf("PID of child_pid: %d\n", getpid());
    char *args[] = {"HelloWorld","c"};
    execv("./HelloWorld&", args);
    int count;
    
    while (1){
        count++;
        printf("Main program/parent process: %d\n", count);
    }
    //exit(0);
    }
}