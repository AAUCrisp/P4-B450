int print_unimportant = 0;      // Trouble Shooting Prints

void WiFi_command(Sockets sock) {
    void* message;
    char msgDump[32];
    int data;
    string coordinate;
    char* WiFimsg = (char*) malloc(9);

    printf("\n\n=====================\nWiFi Listener Started\n=====================\n\n");
    
    if(print_unimportant == 1) {
        cout << "WiFi Command Function || WiFi Sensor Socket: " <<sock.sockWiFi_RECEIVER << endl;
        cout << "WiFi Command Function || WiFi Actuator Socket: " <<sock.act_WiFi << endl;
        cout << "WiFi Command Function || LTE Actuator Socket: " <<sock.act_LTE << endl;
    }
    while(1) {
        message = (void*)receiveWiFi((void*)&sock);
        if(print_unimportant == 1) {
            cout << "WiFi Command Function || Message Parsed from Sockets (data & timestamp) is: " << (const char*)message << endl;
        }
        sscanf((const char*)message, "%d %[^\n]", &data, msgDump);
        cout << "WiFi Command Function || Message Parsed from Sockets as INT is: " << data << endl;
        cout << "msgDump in Command Function is: " << msgDump << endl;
        coordinate = convert_to_coordinate(data);
        cout << "WiFi Command Function || Coordinate for Actuator is: " << coordinate << "\n\n\n" << endl;
        strcpy(WiFimsg, coordinate.c_str());
        transmit_command(&sock, WiFimsg);
        sleep(1);
    }    
}

void* LTE_command(void* socket) {
    Sockets *sock = (Sockets *)socket;
    void* message;
    char msgDump[32];
    int data;
    string coordinate;
    char* LTEmsg = (char*) malloc(9);
    printf("\n\n====================\nLTE Listener Started\n====================\n\n");
    
    if(print_unimportant == 1) {
        cout << "LTE Command Function || LTE Sensor Socket: " << sock->sockLTE_RECEIVER << endl;
        cout << "LTE Command Function || WiFi Actuator Socket: " << sock->act_WiFi << endl;
        cout << "LTE Command Function || LTE Actuator Socket: " << sock->act_LTE << endl; 
    }
    while(1) {
        message = (void*)receiveLTE((void*)sock);
        if(print_unimportant == 1) {
           cout << "LTE Command Function || Message Parsed from Sockets (data & timestamp) is: " << (const char*)message << endl;
        }
        sscanf((const char*)message, "%d %[^\n]", &data, msgDump);
        cout << "LTE Command Function || Message Parsed from Sockets as INT is: " << data << endl;
        coordinate = convert_to_coordinate(data);
        cout << "LTE Command Function || Coordinate for Actuator is: " << coordinate << "\n\n\n" << endl;
        strcpy(LTEmsg, coordinate.c_str());
        transmit_command(sock, LTEmsg);
        sleep(1);
    }
}

