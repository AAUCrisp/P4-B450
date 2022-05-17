#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int main() {
    // Get RSSI value from WiFi
    FILE* pipe_WiFi = popen("iwconfig wlan1 | grep -i --color quality", "r");
    if (pipe) {
        char BufferWiFi[64];
        int signalWiFi;
        // char RSSI[60]= {0};
        while (!feof(pipe_WiFi)) {
            if (fgets(BufferWiFi, 64, pipe_WiFi) != NULL) {
                sscanf(BufferWiFi, "%s %s %s %6c %d", BufferWiFi, BufferWiFi, BufferWiFi, BufferWiFi, &signalWiFi);
            }
        }

        pclose(pipe_WiFi);
        printf("Signal for WiFi: %d \n", signalWiFi);

        // RSSI[strlen(RSSI)-1] = '\0';
        // printf("%s\n",RSSI);
    }

    // Get RSRP value from LTE
    FILE* pipe_LTE = popen("mmcli -m 0 --signal-get | grep -i rsrp", "r");
    if (pipe_LTE) {
        char BufferLTE[128];
        int signalLTE;
        // char RSRP[20] = {0};
        while (!feof(pipe_LTE)) {
            if (fgets(BufferLTE, 128, pipe_LTE) != NULL) {
                sscanf(BufferLTE, "%s %s %d", BufferLTE, BufferLTE, &signalLTE);
            }
        }
        pclose(pipe_LTE);
        printf("Signal for LTE: %d \n", signalLTE);

        // RSRP[strlen(RSRP)-1] = '\0';
        // printf("%s\n",RSRP);
    }
}