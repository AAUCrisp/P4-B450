void WiFi_command(Sockets sock) {
    void* message;
    char msgDump[32];
    int data;
    string coordinate;
    char* WiFimsg = (char*) malloc(9);

    printf("\n\n==================\nWiFi Listener Started\n==================\n\n");
    
    while(1) {
        message = (void*)receiveWiFi((void*)&sock);
        cout << "WiFi Receiver || WiFi Actuator Socket: " <<sock.act_WiFi << endl;
        cout << "WiFi Receiver || LTE Actuator Socket: " <<sock.act_LTE << endl;
        cout << "WiFi Receiver || Message Parsed from Sockets (data & timestamp) is: " << (const char*)message << endl;
        sscanf((const char*)message, "%d %[^\n]", &data, msgDump);
        cout << "WiFi Receiver || Message Parsed from Sockets as INT is: " << data << endl;
        coordinate = convert_to_coordinate(data);
        cout << "WiFi Receiver || Coordinate for Actuator is: " << coordinate << "\n\n\n" << endl;
        // cout << "WiFi Receiver || Size of Coordinate is: " << sizeof(coordinate) << endl;;
        strcpy(WiFimsg, coordinate.c_str());
        transmit_command(&sock, WiFimsg);
        sleep(1);
    }    
}

void* LTE_command(void* socket) {
    // Sockets sock;
    Sockets *sock = (Sockets *)socket;
    void* message;
    char msgDump[32];
    int data;
    string coordinate;
    char* LTEmsg = (char*) malloc(9);
    
    printf("\n\n==================\nLTE Listener Started\n==================\n\n");
    while(1) {
        message = (void*)receiveLTE((void*)&sock);
        cout << "LTE Receiver || WiFi Actuator Socket: " << sock->act_WiFi << endl;
        // cout << "LTE Receiver || WiFi Actuator Socket: " << sock->act_WiFi << endl;
        cout << "LTE Receiver || LTE Actuator Socket: " << sock->act_LTE << endl; 
        // cout << "LTE Receiver || LTE Actuator Socket: " << sock->act_LTE << endl; 
        cout << "LTE Receiver || Message Parsed from Sockets (data & timestamp) is: " << (const char*)message << endl;
        sscanf((const char*)message, "%d %[^\n]", &data, msgDump);
        cout << "LTE Receiver || Message Parsed from Sockets as INT is: " << data << endl;
        coordinate = convert_to_coordinate(data);
        cout << "LTE Receiver || Coordinate for Actuator is: " << coordinate << "\n\n\n" << endl;
        // cout << "LTE Receiver || Size of Coordinate is: " << sizeof(coordinate) << endl;;
        strcpy(LTEmsg, coordinate.c_str());
        transmit_command(sock, LTEmsg);
        sleep(1);
    }
}