#include "Headers/server_header.h"
#include "Headers/shm_write_read.h"



//_______ FUNCTION extracting remote socket address___________
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return&(((struct sockaddr_in6*)sa)->sin6_addr);
}




 /*
    Function that Initializes both the LTE and WiFi socket    
    This function sets up an LTE and WiFi socket and binds them to their respective interfaces. 
*/

void initialize_Server() {

   
    
    
    //_________________________________________________________________________________________
    // Initializing WiFi socket: Defining IP4, Protocol and creating a socket pointer named res. 


    memset(&ServerWiFi, 0, lenWiFi);   //Creating a memory block for the structure with address information for the LTE-socket
    ServerWiFi.ai_family = AF_INET;
    ServerWiFi.ai_socktype = SOCK_DGRAM;
    ServerWiFi.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL,"8000", &ServerWiFi, &res)) != 0 ) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    // creating WiFi socket and binding it. Error handling, to ensure creating and binding of socket. 
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
    
    
    //_____________________________________________________________________________________________________________________
    //                  Initializing LTE socket

    memset(&ServerLTE, 0, lenLTE);   //Creating a memory block for the structure with address information for the LTE-socket
    ServerLTE.ai_family = AF_INET;
    ServerLTE.ai_socktype = SOCK_DGRAM;
    ServerLTE.ai_flags = AI_PASSIVE;
    if ((rl = getaddrinfo(NULL, "8001",&ServerLTE, &res1)) !=0 )    // A predefined Structure, that automates setting address info, and sets up a struct pointer. 
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



    freeaddrinfo(res);  // Clears the pointer to the stucture containing info for the WIFI socket
    freeaddrinfo(res1); // Clears the pointer to the structure containing info for the LTE socket

    printf("listener: waiting to recvfrom... \n");

}


//_____________ THREAD FUNCTION FOR RECEIVING VIA WIFI
void  *ReceiveCoordinateWiFi(void *) {
    fromlen = sizeof remote_addr;
    if(rc_WiFi = recvfrom(sockWiFi, ActuatorBuffer, sizeof(ActuatorBuffer), 0, (struct sockaddr *)&remote_addr, &fromlen) == -1)
    {
        perror("recvfrom failed");
        exit(1);
    }

    printf("we got the buffer from %s\n",inet_ntop(remote_addr.ss_family,get_in_addr((struct sockaddr *)&remote_addr), s, sizeof s)); // Prints out the remote sockets address
    printf("Actuator_WiFi: packet contains \"%s\"\n", ActuatorBuffer);

    
    received_message = ActuatorBuffer;                                  //updating the Shared memory variable. 
    shm_write(received_message, sizeof(ActuatorBuffer), GSV_KEY);       // Writing to shared memory 
    printf("The buffer sent to shared memory: %s \n \n", received_message);
    
    pthread_exit(NULL);
    
}




//_____________ THREAD FUNCTION FOR RECEIVING VIA LTE
void *ReceiveCoordinateLTE(void *) {
    fromlen = sizeof remote_addr;
    if(rc_LTE = recvfrom(sockLTE, ActuatorBuffer, sizeof(ActuatorBuffer), 0, (struct sockaddr *)&remote_addr, &fromlen) == -1)
    {
        perror("recvfrom failed");
        exit(1);
    }

    printf("we got the buffer from %s\n", inet_ntop(remote_addr.ss_family,get_in_addr((struct sockaddr *)&remote_addr), s, sizeof s)); // Prints out the remote sockets address
    printf("Actuator_LTE: packet contains \"%s\"\n", ActuatorBuffer);


    received_message = ActuatorBuffer;                                  //updating the Shared memory variable.
    shm_write(received_message, sizeof(ActuatorBuffer), GSV_KEY);       // Writing to shared memory 
    printf("The buffer sent to shared memory: %s \n \n", received_message);
        
    pthread_exit(NULL);
}





