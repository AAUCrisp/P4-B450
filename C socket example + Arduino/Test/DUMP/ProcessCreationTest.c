#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main() {
    pid_t child_pid;
    child_pid = fork();

    if (child_pid == 0) {
        printf("PID of parent_pid: %d\n", getppid());
        printf("PID of child_pid: %d\n", getpid());
        char *args[] = {"HelloWorld", "c"};
        execv("./HelloWorld&", args);
        int count;

        while (1) {
            sleep(1);
            count++;
            printf("Main program/parent process: %d\n", count);
            printf("PID of parent_pid: %d\n", getppid());
            printf("PID of child_pid: %d\n", getpid());
            if (count == 20) {
                break;
            } else{
            // do something
            }
        }
        exit(0);
    }
}