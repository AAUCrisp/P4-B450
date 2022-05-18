#include "server_function.cpp"
//#include "Headers/shm_write_read.h"
#include <cassert>




template <std::size_t N>
int execvp(const char* file, const char* const (&argv)[N])
{
    assert((N > 0) && (argv[N -1] == nullptr));

    return execvp(file, const_cast<char* const*>(argv));
}


int main() 
{
    pid_t signal_monitor;
    signal_monitor = fork();
    if(signal_monitor = 0) {
        printf("parent process ID: %d \n", getppid());
        printf("Child process ID : %d \n", getpid());
        

        char path[] = "./child";
        const char* argv[] = {"/.child", nullptr};
        execvp(path, argv);
        printf("ERROR: DID NOT START THE CHILD PROCESS \n \n");
    } else {
        initialize_Server();
        while (1) {
            pthread_create(&T1, NULL, ReceiveCoordinateLTE, NULL);
            pthread_create(&T2, NULL, ReceiveCoordinateWiFi, NULL);
        }
        kill(getpid(), SIGKILL);
        close(sockWiFi);
        close(sockLTE);
    }
}