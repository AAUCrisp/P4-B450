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
    printf("PID of HelloWorld.c: %d\n", getpid());
    int count;

    while (1) {
        sleep(1);
        count++;
        printf("Hello world! %d\n", count);
        printf("PID of parent_pid: %d\n", getppid());
        printf("PID of child_pid: %d\n", getpid());
        if (count == 49) {
            break;
        }
    }
exit(0);
}
