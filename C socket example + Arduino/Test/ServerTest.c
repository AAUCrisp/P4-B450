#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

/* Define buffer and PORT number */
#define SENSBUF 1024
#define ACTBUF 1024
#define PORT1 8888
#define PORT2 8887

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan1";

/* Misc */
struct sockaddr_in ServerLTE;
struct sockaddr_in ServerWiFi;
int sockLTE, sockWiFi;
int bindLTE, bindWiFi;
int lenLTE = sizeof(ServerLTE);
int lenWiFi = sizeof(ServerWiFi);
char SensorBuffer[SENSBUF];
char ActuatorBuffer[ACTBUF];
int rc_LTE, rc_WiFi;
pthread_t T1, T2;


void Create_Bind_Socket_LTE() {
  /* Create socket */
  sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));

  if (sockLTE == -1) {
    perror("Failed to create socket: LTE");
  }
  
  /* Configure settings to communicate with remote UDP server */
  Server1.sin_family = AF_INET;
  Server1.sin_port = htons(PORT1);
  Server1.sin_addr.s_addr = INADDR_ANY;

  /* Bind to socket */
  bindLTE = bind(sockLTE, (struct sockaddr*)&ServerLTE, sizeof(struct sockaddr));

  if (bindLTE == -1) {
    perror("Failed to bind: LTE");
  }
}

void Receive_Data_LTE(){
  rc_LTE = recvfrom(sockLTE, SensorBuffer, SENSBUF, 0, (struct sockaddr*)&ServerLTE, &lenLTE);
    printf("%s\n \n", SensorBuffer);
}

void Create_Bind_Socket_WiFi() {
  /* Create socket */
  sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

  if (sockWiFi == -1) {
    perror("Failed to create socket: WiFi");
  }
  
  /* Configure settings to communicate with remote UDP server */
  Server2.sin_family = AF_INET;
  Server2.sin_port = htons(PORT2);
  Server2.sin_addr.s_addr = INADDR_ANY;

  /* Bind to socket */
  bindWiFi = bind(sockWiFi, (struct sockaddr*)&ServerWiFi, sizeof(struct sockaddr));

  if (bindWiFi == -1) {
    perror("Failed to bind: WiFi");
  }  
}

void Receive_Data_WiFi(){
  rc_WiFi = recvfrom(sockWiFi, ActuatorBuffer, ACTBUF, 0, (struct sockaddr*)&ServerWiFi, &lenWiFi);
    printf("%s\n \n", ActuatorBuffer);
}

int main() {
    /* Binding sockets to LTE/WiFi */
    Create_Bind_Socket_LTE();
    Create_Bind_Socket_WiFi();

  /* Main running code */
  while (1){
    puts("Emergency exit: CTRL+C");
    printf("Waiting for data...\n");
    /* Creating threads running receive data functions */
    pthread_create(&T1, NULL, Receive_Data_LTE, NULL);
    pthread_create(&T2, NULL, Receive_Data_WiFi, NULL);

    //pthread_join(T1, NULL);
    //pthread_join(T2, NULL);


  }
  close(sockLTE && sockWiFi);
  return 1;
}


/*
void* LTE_Socket(void* arg){
  sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  setsockopt(sockfd1, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
  if (sockfd1 == -1) {
    perror("Failed to create socket");
    exit(0);
  }

  Server1.sin_family = AF_INET;
  Server1.sin_port = htons(PORT1);
  Server1.sin_addr.s_addr = INADDR_ANY;
*/
  /* Bind to socket */
/*
  int a = bind(sockfd1, (struct sockaddr*)&Server1, sizeof(struct sockaddr));
  if (a == -1) {
    perror("Failed to bind");
  }

  rc1 = recvfrom(sockfd1, Buffer1, MAXBUF, 0, (struct sockaddr*)&Server1, &len1);
    printf("%s\n \n", Buffer1);
    close(sockfd1);
    pthread_exit(0);
}*/
/*
void* WiFi_Socket(void* arg){
  sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  setsockopt(sockfd2, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));
  if (sockfd2 == -1) {
    perror("Failed to create socket");
    exit(0);
  }

  Server2.sin_family = AF_INET;
  Server2.sin_port = htons(PORT2);
  Server2.sin_addr.s_addr = INADDR_ANY;
*/
  /* Bind to socket */
/*
  int b = bind(sockfd2, (struct sockaddr*)&Server2, sizeof(struct sockaddr));
  if (b == -1) {
    perror("Failed to bind");
  }

  
  rc2 = recvfrom(sockfd2, Buffer2, MAXBUF, 0, (struct sockaddr*)&Server2, &len2);
  printf("%s\n \n", Buffer2);
  close(sockfd2);
  pthread_exit(0);
}*/
