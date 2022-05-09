#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int RSSI_VAL() {
    // Get RSSI value from WiFi
    FILE* pipe_WiFi = popen("iwconfig wlan0 | grep -i --color quality", "r");
    if (pipe) {
        char BufferWiFi[64];
        int signalWiFi;
        while (!feof(pipe_WiFi)) {
            if (fgets(BufferWiFi, 64, pipe_WiFi) != NULL) {
                sscanf(BufferWiFi, "%s %s %s %6c %d", BufferWiFi, BufferWiFi, BufferWiFi, BufferWiFi, &signalWiFi);
            }
        }

        pclose(pipe_WiFi);
        printf("Signal for WiFi: %d \n", signalWiFi);

        return signalWiFi;
    }
}

int RSRP_VAL() {
    // Get RSRP value from LTE
    FILE* pipe_LTE = popen("mmcli -m 0 --signal-get | grep -i rsrp", "r");
    if (pipe_LTE) {
        char BufferLTE[128];
        int signalLTE;
        while (!feof(pipe_LTE)) {
            if (fgets(BufferLTE, 128, pipe_LTE) != NULL) {
                sscanf(BufferLTE, "%s %s %d", BufferLTE, BufferLTE, &signalLTE);
            }
        }
        pclose(pipe_LTE);
        printf("Signal for LTE: %d \n", signalLTE);

        return signalLTE;
    }
}
