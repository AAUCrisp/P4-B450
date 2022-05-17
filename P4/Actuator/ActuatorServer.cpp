#include "ActuatorServer.h"
#include "processData.cpp"
#include <fstream>
#include <sstream>

struct addrinfo ServerLTE, *res1;   // a socket struct design to be used with IPv4
struct addrinfo ServerWiFi, *res;  // a socket struct design to be used with IPv4
struct sockaddr_storage remote_addr;
socklen_t fromlen;
int rv, rl;
int sockLTE, sockWiFi;
int bindLTE, bindWiFi;
int lenLTE = sizeof(ServerLTE);
int lenWiFi = sizeof(ServerWiFi);
int rc_LTE, rc_WiFi;
int tx_LTE, tx_WiFI;
pthread_t T1, T2;

std::ofstream LTE_File ("LTE-Log.txt", std::ofstream::out);
std::ofstream WiFi_File ("WiFi-Log.txt", std::ofstream::out);


char ActuatorBuffer[1024];
char feedback[1024];

const char *LTE = "wwan0";
const char *WiFi = "wlan0";
char s[INET_ADDRSTRLEN];

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return&(((struct sockaddr_in6*)sa)->sin6_addr);
}


void initialize_Server() {
    //_______________________________________
    //                  Initializing WiFi socket
    memset(&ServerWiFi, 0, lenWiFi);
    ServerWiFi.ai_family = AF_INET;
    ServerWiFi.ai_socktype = SOCK_DGRAM;
    ServerWiFi.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL,"8000", &ServerWiFi, &res)) != 0 ) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    sockWiFi = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));
    if (sockWiFi == -1) {
        perror("Failed to create sockWiFi");
        exit(0);
    }
    bindWiFi = bind(sockWiFi, res->ai_addr, res->ai_addrlen);
    if (bindWiFi == -1) 
    {
        close(sockWiFi);
        perror("bind");
        exit(0);
    }   
    
    
    //_______________________________________
    //                  Initializing LTE socket
    memset(&ServerLTE, 0, lenLTE);
    ServerLTE.ai_family = AF_INET;
    ServerLTE.ai_socktype = SOCK_DGRAM;
    ServerLTE.ai_flags = AI_PASSIVE;
    if ((rl = getaddrinfo(NULL, "8001",&ServerLTE, &res1)) !=0 )
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rl));
        exit(1);
    }

    sockLTE = socket(res1->ai_family, res1->ai_socktype, res1->ai_protocol);
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    if(sockLTE == -1)
    {
        perror("failed to create sockLTE");
        exit(0);
    }
    bindLTE = bind(sockLTE, res1->ai_addr, res1->ai_addrlen);
    if (bindLTE == -1)
    {
        close(sockLTE);
        perror("bind LTE socket");
        exit(0);
    }



    freeaddrinfo(res);
    freeaddrinfo(res1);

    printf("listener: waiting to recvfrom... \n");

}

void logData1(int *arr) 
{
    //std::ostringstream out;
    //out << "\n\n movement on the x-axi:" << arr[0] << "\n movement on the y-axis:" << arr[1];
    //sprintf(temp, "Movement on x and y axis:  x = %d, y = %d \n \n", arr[0], arr[1]);
    //fwrite(temp, sizeof(char), strlen(temp), out);
    LTE_File << "\n\n movement on the x-axi:" << arr[0] << "\n movement on the y-axis:" << arr[1];
}

void logData2(int *arr)
{
    //std::ostringstream movement;
    //movement << "\n\n Movement on the x-axis:" << arr[0] << "\n movement on the y-axis:" << arr[1];
    WiFi_File << "\n\n Movement on the x-axis:" << arr[0] << "\n movement on the y-axis:" << arr[1];
}


void  *ReceiveCoordinateWiFi(void *) {
    fromlen = sizeof remote_addr;
    if(rc_WiFi = recvfrom(sockWiFi, ActuatorBuffer, sizeof(ActuatorBuffer), 0, (struct sockaddr *)&remote_addr, &fromlen) == -1)
    {
        perror("recvfrom failed");
        exit(1);
    }
    printf("we got the buffer from %s\n",
    inet_ntop(remote_addr.ss_family,get_in_addr((struct sockaddr *)&remote_addr), s, sizeof s)); // Prints out the remote sockets address
    printf("Actuator_WiFi: packet contains \"%s\"\n", ActuatorBuffer);
    int* returnedArr = process_Data(ActuatorBuffer);
    logData2(returnedArr);
    printf("Logged WiFi data success");
    free (returnedArr);
    pthread_exit(NULL);
  
    
}

void *ReceiveCoordinateLTE(void *) {
    fromlen = sizeof remote_addr;
    if(rc_LTE = recvfrom(sockLTE, ActuatorBuffer, sizeof(ActuatorBuffer), 0, (struct sockaddr *)&remote_addr, &fromlen) == -1)
    {
        perror("recvfrom failed");
        exit(1);
    }
    printf("we got the buffer from %s\n",
    inet_ntop(remote_addr.ss_family,get_in_addr((struct sockaddr *)&remote_addr), s, sizeof s)); // Prints out the remote sockets address
    printf("Actuator_LTE: packet contains \"%s\"\n", ActuatorBuffer);
    
    int * returnedArr = process_Data(ActuatorBuffer);
    logData1(returnedArr);
    printf("Logged LTE data success");
    free(returnedArr);
    
    pthread_exit(NULL);
}



int main() {
    initialize_Server();
    //out = fopen("MovementCommands.txt", "w");   
    while (1)
    {
       pthread_create(&T1, NULL, ReceiveCoordinateLTE, NULL);
       pthread_create(&T2, NULL, ReceiveCoordinateWiFi, NULL); 
        //ReceiveCoordinateWiFi();
        //ReceiveCoordinateLTE();
        sleep(1);
    }
    //fclose(out);
    LTE_File.close();
    WiFi_File.close();
    close(sockWiFi);
    close(sockLTE);
}