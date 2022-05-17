#include "Headers/server_header.h"


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

    received_message = ActuatorBuffer;
    shm_write(received_message, sizeof(ActuatorBuffer), GSV_KEY);
    printf("The buffer sent to shared memory: %s \n \n", received_message);
    
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
    received_message = ActuatorBuffer;
    shm_write(received_message, sizeof(ActuatorBuffer), GSV_KEY);
    printf("The buffer sent to shared memory: %s \n \n", received_message);
        
    pthread_exit(NULL);
}





