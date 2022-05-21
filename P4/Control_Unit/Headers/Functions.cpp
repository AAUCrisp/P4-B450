void WiFi_command(Sockets sock) {
    void* message;
    char msgDump[32];
    int data;
    string coordinate;
    char* WiFimsg = (char*) malloc(9);

    printf("\n\n=====================\nWiFi Listener Started\n=====================\n\n");
    
    if(troubleshooting_print == 1) {
        cout << "WiFi Command Function || WiFi Sensor Socket: " <<sock.sockWiFi_RECEIVER << endl;
        cout << "WiFi Command Function || WiFi Actuator Socket: " <<sock.act_WiFi << endl;
        cout << "WiFi Command Function || LTE Actuator Socket: " <<sock.act_LTE << endl;
    }
    while(1) {
        message = (void*)receiveWiFi((void*)&sock);
        if(troubleshooting_print == 1) {
            cout << "WiFi Command Function || Message Parsed from Sockets (data & timestamp) is: " << (const char*)message << endl;
        }
        sscanf((const char*)message, "%d %[^\n]", &data, msgDump);
        cout << "WiFi Command Function || Message Parsed from Sockets as INT is: " << data << endl;
        coordinate = convert_to_coordinate(data, use_hex);
        cout << "WiFi Command Function || Coordinate for Actuator is: " << coordinate << "\n\n\n" << endl;
        strcpy(WiFimsg, coordinate.c_str());
        transmit_command(&sock, WiFimsg);
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
    
    if(troubleshooting_print == 1) {
        cout << "LTE Command Function || LTE Sensor Socket: " << sock->sockLTE_RECEIVER << endl;
        cout << "LTE Command Function || WiFi Actuator Socket: " << sock->act_WiFi << endl;
        cout << "LTE Command Function || LTE Actuator Socket: " << sock->act_LTE << endl; 
    }
    while(1) {
        message = (void*)receiveLTE((void*)sock);
        if(troubleshooting_print == 1) {
           cout << "LTE Command Function || Message Parsed from Sockets (data & timestamp) is: " << (const char*)message << endl;
        }
        sscanf((const char*)message, "%d %[^\n]", &data, msgDump);
        cout << "LTE Command Function || Message Parsed from Sockets as INT is: " << data << endl;
        coordinate = convert_to_coordinate(data, use_hex);
        cout << "LTE Command Function || Coordinate for Actuator is: " << coordinate << "\n\n\n" << endl;
        strcpy(LTEmsg, coordinate.c_str());
        transmit_command(sock, LTEmsg);
    }
}

void help() {
    cout << " |-|-|-|-|-|-  Main program for Control Unit  -|-|-|-|-|-|" << endl;
    cout << "     For 4th semester project of ComTek Group 450" << endl;
    cout << "     Spring Semester 2022" << endl << endl << endl;
    cout << "Arguments Available for this program is:" << endl;
    cout << "_____________________________________________________" << endl << endl;

    cout << "---- Start of the Signal Monitoring Process ----" << endl;
    cout << "Call Arguments:" << endl;
    cout << "   -m" << endl;
    cout << "   -monitor" << endl;
    cout << "   -monitoring" << endl << endl;
    cout << "Accepted Arguments:" << endl;
    cout << "   true / yes / 1   - For enabling" << endl;
    cout << "   false / no / 0   - For disabling" << endl << endl << endl;

    cout << " - Use RAM Grid for Command Conversion - " << endl;
    cout << "Call Argument: -g or -grid" << endl;
    cout << "Accepted Arguments: true / yes / 1   &   false / no / 0   is accepted" << endl << endl << endl;
    exit(0);
}


void Argument_Setup(int argc, char* argv[]) {
    int axis_change = 0;
    char firstCharacter;

    if ((string) argv[1] == "-help") {
        help();
    }
    cout << "============================\nControl Unit Process Started\n============================\n\n" << endl;

    cout << "\nArgument(s) accepted." << endl;

    cout << "Number of arguments combinations: " << (argc - 1)/2 << endl;

    for (int i = 1; i < argc; i++) {
        string current = argv[i];
        firstCharacter = current.at(0);
        // cout << "First Character in Argument is: " << firstCharacter << endl;
        
        if(firstCharacter == '-') {
            // RAM Grid Argument
            if((string) argv[i] == "-g" || (string) argv[i] == "-grid") {
                if ( (string) argv[i+1] == "no" || (string) argv[i+1] == "0" || (string) argv[i+1] == "false") {
                    cout << "===== RAM Grid Disabled =====" << endl;
                    use_grid = 0;
                }
                else if ( (string) argv[i+1] == "yes" || (string) argv[i+1] == "1" || (string) argv[i+1] == "true") {
                    cout << "===== RAM Grid Enabled =====" << endl;
                    use_grid = 1;
                }
                else { cout << "===== Invalid argument for \"RAM Grid Usage\" =====" << endl; }
            }

            // Signal Monitoring Process Argument
            if((string) argv[i] == "-m" || (string) argv[i] == "-monitor" || (string) argv[i] == "-monitoring") {
                if ( (string) argv[i+1] == "no" || (string) argv[i+1] == "0" || (string) argv[i+1] == "false") {
                    cout << "===== Signal Monitoring Process Creation Disabled =====" << endl;
                    monitor = 0;
                }
                else if ( (string) argv[i+1] == "yes" || (string) argv[i+1] == "1" || (string) argv[i+1] == "true") {
                    cout << "===== Signal Monitoring Process Creation Enabled =====" << endl;
                    monitor = 1;
                }
                else { cout << "===== Invalid argument for \"Signal Monitoring Process Start\" =====" << endl; }
            }

            // Verbose Argument
            if((string) argv[i] == "-v" || (string) argv[i] == "-verbose") {

                for (int j = i+1; j < argc; j++) {  // Check for sevaral Verbose arguments
                    string current = argv[j];
                    firstCharacter = current.at(0);

                    if(firstCharacter == '-') {
                        break;
                    }
                    else if ( (string) argv[j] == "no" || (string) argv[j] == "0" || (string) argv[i+1] == "false") {
                        cout << "===== Verbose Disabled =====" << endl;
                        message_only = 1;        // Print messages only
                        // troubleshooting_print = 0;
                        troubleshooting_print = 0;
                        print_sen_in = 0;       // Print incoming Sensor related things
                        print_act_out = 0;      // Print outgoing Actuator related things
                        print_GSV = 0;          // Print GSV related things
                        break;
                    }
                    else if ( (string) argv[j] == "yes" || (string) argv[j] == "1" || (string) argv[i+1] == "true") {
                        cout << "===== Verbose Enabled =====" << endl;
                        message_only = 0;        // Print messages only
                        // troubleshooting_print = 1;
                        troubleshooting_print = 1;
                        print_sen_in = 1;       // Print incoming Sensor related things
                        print_act_out = 1;      // Print outgoing Actuator related things
                        print_GSV = 1;          // Print GSV related things
                        break;
                    }
                    else if ( (string) argv[j] == "i" || (string) argv[j] == "in") {
                        cout << "===== Verbose For Incoming Traffic Enabled =====" << endl;
                        message_only = 0;        // Print messages only
                        print_sen_in = 1;       // Print incoming Sensor related things
                    }
                    else if ( (string) argv[j] == "o" || (string) argv[j] == "out") {
                        cout << "===== Verbose For Outgoing Traffic Enabled =====" << endl;
                        message_only = 0;        // Print messages only
                        print_act_out = 1;       // Print incoming Sensor related things
                    }
                    else if ( (string) argv[j] == "g" || (string) argv[j] == "gsv") {
                        cout << "===== Verbose For GSV Enabled =====" << endl;
                        message_only = 0;        // Print messages only
                        print_GSV = 1;       // Print incoming Sensor related things
                    }
                    else { 
                        cout << "===== Invalid argument for \"Verbose\" =====" << endl; 
                    }
                }
            }

            // Usage of Hex Char Argument
            if((string) argv[i] == "-h" || (string) argv[i] == "-hex") {
                if ( (string) argv[i+1] == "no" || (string) argv[i+1] == "0" || (string) argv[i+1] == "false") {
                    cout << "===== Hex Chars Disabled =====" << endl;
                    use_hex = 0;
                }
                else if ( (string) argv[i+1] == "yes" || (string) argv[i+1] == "1" || (string) argv[i+1] == "true") {
                    cout << "===== Hex Chars Enabled =====" << endl;
                    use_hex = 1;
                }
                else { cout << "===== Invalid argument for \"Hex Conversion\" =====" << endl; }
            }

            // Force Both Technologies Argument
            if((string) argv[i] == "-b" || (string) argv[i] == "-both") {
                if ( (string) argv[i+1] == "no" || (string) argv[i+1] == "0" || (string) argv[i+1] == "false") {
                    cout << "===== Forced Use of Both Technologies Disabled =====" << endl;
                    force_both = 0;
                    force_both = 0;
                }
                else if ( (string) argv[i+1] == "yes" || (string) argv[i+1] == "1" || (string) argv[i+1] == "true") {
                    cout << "===== Forced Use of Both Technologies Enabled =====" << endl;
                    force_both = 1;
                    // force_both = 1;
                }
                else { cout << "===== Invalid argument for \"Forced Both Technologies\"===== " << endl; }
            }

            // X-Axis Range Argument
            if((string) argv[i] == "-x") {
                
                x_axis = atoi(argv[i+1]);
                cout << "===== X-axis range set to " << x_axis << " =====" << endl;
                axis_change = 1;
            }

            // Y-Axis Range Argument
            if((string) argv[i] == "-y") {
                
                y_axis = atoi(argv[i+1]);
                cout << "===== Y-axis range set to " << y_axis << " =====" << endl;
                axis_change = 1;
            }

        }
    }
    if(axis_change >= 1) {
        coordinates = x_axis * y_axis;
    }

}

