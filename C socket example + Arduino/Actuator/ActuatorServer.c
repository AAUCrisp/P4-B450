#include "ActuatorServer.h"
#include "coordinates.c"


struct sockaddr_in ServerLTE;   // a socket struct design to be used with IPv4
struct addrinfo ServerWiFi, *res;  // a socket struct design to be used with IPv4
struct sockaddr_storage remote_addr;
socklen_t fromlen;
int rv;
int sockLTE, sockWiFi;
int bindLTE, bindWiFi;
int lenLTE = sizeof(ServerLTE);
int lenWiFi = sizeof(ServerWiFi);
int rc_LTE, rc_WiFi;
int tx_LTE, tx_WiFI;
pthread_t T1, T2;

char ActuatorBuffer[1024];
char feedback[1024];

const char *LTE = "wwan0";
const char *WiFi = "wlp7s0";
char s[INET_ADDRSTRLEN];

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return&(((struct sockaddr_in6*)sa)->sin6_addr);
}


int initialize_Server() {
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
        perror("Failed to create sockLTE");
        exit(0);
    }
    bindWiFi = bind(sockWiFi, res->ai_addr, res->ai_addrlen);
    if (bindWiFi == -1) 
    {
        close(sockWiFi);
        perror("bind");
        exit(0);
    }   

    freeaddrinfo(res);

    printf("listener: waiting to recvfrom... \n");


}

int ReceiveCoordinate() {
    fromlen = sizeof remote_addr;
    if(rc_WiFi = recvfrom(sockWiFi, ActuatorBuffer, 1024, 0, (struct sockaddr *)&remote_addr, &fromlen) == -1)
    {
        perror("recvfrom failed");
        exit(1);
    }
    printf("we got the buffer from %s\n",
    inet_ntop(remote_addr.ss_family,get_in_addr((struct sockaddr *)&remote_addr), s, sizeof s));
    //printf("listener: packet is %d bytes long\n", rc_WiFi);
    printf("listener: packet contains \"%s\"\n", ActuatorBuffer);
    result = parse_coordinates(ActuatorBuffer);
    printf("the coordinates are x = %d and y = %d\n",result.current_x_coordinate, result.current_y_coordinate);
    printf("Feedback is %s\n\n", result.feedback);
    tx_WiFI = sendto(sockWiFi, result.feedback,strlen(result.feedback),0,get_in_addr((struct sockaddr *)&remote_addr), sizeof remote_addr);
}




int main() {
    while (1)
    {
        initialize_Server();
        ReceiveCoordinate();
      
        close(sockWiFi);
    }
    
}