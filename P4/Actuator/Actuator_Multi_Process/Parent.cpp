#include "server_function.cpp"
#include <cassert>




// THIS FUNCTION OPENS CHILD PROGRAM!!!
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
    if(signal_monitor == 0) {
        printf("parent process ID: %d \n", getppid()); // get parent id
        printf("Child process ID : %d \n", getpid()); // get child id
        char path[] = "./Child"; //specify path to child program
        const char* argv[] = {"./Child", NULL}; // list over programs and their parameters
        execvp("./Child", argv); // opens child program
        printf("ERROR: DID NOT START THE CHILD PROCESS \n \n");
    } else {
        initialize_Server();
        while (1) {
            pthread_create(&T1, NULL, ReceiveCoordinateLTE, NULL);
            pthread_create(&T2, NULL, ReceiveCoordinateWiFi, NULL);
            usleep(100000);
        }
        close(sockWiFi);
        close(sockLTE);
        exit(1);
    }
}