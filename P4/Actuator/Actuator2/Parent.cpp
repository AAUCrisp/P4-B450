#include "Headers/shm_write_read.h"
#include "server_function.cpp"


int main() 
{
    pid_t signal_monitor;
    signal_monitor = fork();
    if(signal_monitor = 0) {
        printf("parent process ID: %d \n", getppid());
        printf("Child process ID : %d \n", getpid());
        char path [] = "./child";
        char* args[] = {"/.child", NULL};
        execv(path, args);
        printf("ERROR: DID NOT START THE CHILD PROCESS \n \n");
    } else {
        initialize_Server();
        while (1) {
            pthread_create(&T1, NULL, ReceiveCoordinateLTE, NULL);
            pthread_create(&T2, NULL, ReceiveCoordinateWiFi, NULL);
        }
        close(sockWiFi);
        close(sockLTE);
    }
}