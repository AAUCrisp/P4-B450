#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#ifndef SOCKETS_RSSI
#define SOCKETS_RSSI
#include "Sockets.h"
#endif

int print = 0;

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
        if(print = 1) {
            printf("GSV || WiFi Strength: %d \n", signalWiFi);
        }

        return signalWiFi;
    }
    return -300;
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
        if(print = 1) {
            printf("GSV || LTE Strength: %d \n", signalLTE);
        }

        return signalLTE;
    }
    return -300;
}
