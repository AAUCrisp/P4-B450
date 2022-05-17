#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>


// WiFi network name and password:
const char * networkName = "Bamses_Hytte";
const char * networkPswd = "abekattefisk";

//Port number and IP address to send UDP data to:
const char * udpAddress = "192.168.1.199";
const int udpPort = 8888;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

#define MAXBUF 15
#define CONFIG_FREERTOS_HZ 1000

char message[MAXBUF];
struct sockaddr_in Client;
int sockfd;
int len = sizeof(Client);


void setup() {
  Serial.begin(115200);
  ConnectToWiFi(networkName, networkPswd);
  sockfd = socket(PF_INET, SOCK_DGRAM, 0); // create a UDP socket

  if (sockfd == -1) {
    if (sockfd == -1) {
      perror("Failed to create socket");
      exit(0);
    }
  }
  /* configure settings to communicate with remote UDP server */
  Client.sin_family = AF_INET;
  Client.sin_port = htons(udpPort); // server port
  Client.sin_addr.s_addr = inet_addr(udpAddress); //

  /*long rssi = WiFi.RSSI();
    Serial.print("RSSI:");
    Serial.println(rssi);*/


}

void loop() {
  // put your main code here, to run repeatedly:
  char TestMsg[] = "Hello shithead";
  sendto(sockfd, TestMsg, sizeof(TestMsg), 0, (struct sockaddr *)&Client, len);
  Serial.print(WiFi.SSID());
  Serial.println(WiFi.RSSI());

}

void ConnectToWiFi(const char * ssid, const char * pwd) {
  Serial.println("Connecting to WiFi network: " + String(ssid));

  //register event handler
  WiFi.onEvent(WiFiEvent);

  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      //When connected set
      Serial.print("WiFi connected! IP address: ");
      Serial.println(WiFi.localIP());
      printf("\n");
      //initializes the UDP state
      //This initializes the transfer buffer
      udp.begin(udpPort);
      connected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      connected = false;
      break;
    default: break;
  }
}
