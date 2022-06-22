void WiFi_command(Sockets sock) {
    void* message;
    char msgDump[32];
    int data;
    int ID;
    string coordinate;
    char* WiFimsg = (char*) malloc(9);

    printf("\n\n  =======================\n   WiFi Listener Started\n  =======================\n\n");
    
    if(troubleshooting_print == 1) {
        cout << "  WiFi Command Function || WiFi Sensor Socket: " <<sock.sockWiFi_RECEIVER << endl;
        cout << "  WiFi Command Function || WiFi Actuator Socket: " <<sock.act_WiFi << endl;
        cout << "  WiFi Command Function || LTE Actuator Socket: " <<sock.act_LTE << endl;
    }
    while(1) {
        message = (void*)receiveWiFi((void*)&sock);
        if(troubleshooting_print == 1) {
            cout << "  WiFi Command Function || Message Parsed from Sockets (data & timestamp) is: " << (const char*)message << endl;
        }
        sscanf((const char*)message, "%d: %d %[^\n]", &ID, &data, msgDump);
        if(use_grid == 1) {
            coordinate = grid[data];
        }
        else {
            coordinate = convert_to_coordinate(data, use_hex);
        }
        if(message_only == 1) {
            cout << "  WiFi Command Function || Message Parsed from Sockets as INT is: " << data << endl;
            cout << "  WiFi Command Function || Coordinate for Actuator is: " << coordinate << "\n\n\n" << endl;
        }
        strcpy(WiFimsg, coordinate.c_str());
        transmit_command(&sock, WiFimsg);
    }    
}

void* LTE_command(void* socket) {
    Sockets *sock = (Sockets *)socket;
    void* message;
    char msgDump[32];
    int data;
    int ID;
    string coordinate;
    char* LTEmsg = (char*) malloc(9);
    printf("\n\n  ======================\n   LTE Listener Started\n  ======================\n\n");
    
    if(troubleshooting_print == 1) {
        cout << "  LTE Command Function || LTE Sensor Socket: " << sock->sockLTE_RECEIVER << endl;
        cout << "  LTE Command Function || WiFi Actuator Socket: " << sock->act_WiFi << endl;
        cout << "  LTE Command Function || LTE Actuator Socket: " << sock->act_LTE << endl; 
    }
    while(1) {
        message = (void*)receiveLTE((void*)sock);
        if(troubleshooting_print == 1) {
           cout << "  LTE Command Function || Message Parsed from Sockets (data & timestamp) is: " << (const char*)message << endl;
        }
        sscanf((const char*)message, "%d: %d %[^\n]",&ID, &data, msgDump);
        if(use_grid == 1) {
            coordinate = grid[data];
        }
        else {
            coordinate = convert_to_coordinate(data, use_hex);
        }
        if(message_only == 1) {
            cout << "  LTE Command Function || Message Parsed from Sockets as INT is: " << data << endl;
            cout << "  LTE Command Function || Coordinate for Actuator is: " << coordinate << "\n\n\n" << endl;
        }
        strcpy(LTEmsg, coordinate.c_str());
        transmit_command(sock, LTEmsg);
    }
}

void help() {
    cout << "  |-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|" << endl;
    cout << "  |-|-|-|-|-|-  Main program for Control Unit  -|-|-|-|-|-|" << endl;
    cout << "  |-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|" << endl;
    cout << "  |-|-|                                               |-|-|" << endl;
    cout << "  |-|-|  For 4th semester project of ComTek Group 450 |-|-|" << endl;
    cout << "  |-|-|             Spring Semester 2022              |-|-|" << endl;
    cout << "  |-|-|                                               |-|-|" << endl;
    cout << "  |-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|" << endl << endl << endl << endl << endl;
    cout << "   Arguments Available for this program is:" << endl;
    cout << "  __________________________________________________________" << endl << endl;


    cout << "  ----------------------------------------------------------" << endl;
    cout << "  ----  Disable Start of the Signal Monitoring Process  ----" << endl;
    cout << "  ----                                                  ----" << endl;
    cout << "   Call Arguments:" << endl;
    cout << "      -m" << endl;
    cout << "      -monitor" << endl;
    cout << "      -monitoring" << endl << endl << endl;


    cout << "  ----------------------------------------------------------" << endl;
    cout << "  ----       Use RAM Grid for Command Conversion        ----" << endl;
    cout << "  ----                                                  ----" << endl;
    cout << "   Call Argument:" << endl;
    cout << "      -g" << endl;
    cout << "      -grid" << endl << endl << endl;


    cout << "  ----------------------------------------------------------" << endl;
    cout << "  ----          Force Communication Technology          ----" << endl;
    cout << "  ----                                                  ----" << endl;
    cout << "   Arguments market with * can be written sequencially" << endl;
    cout << "   without needing to call -v / -verbose again" << endl << endl;
    cout << "   Call Argument:" << endl;
    cout << "      -t <extra_1>" << endl;
    cout << "      -tech <extra_1>" << endl;
    cout << "      -technology <extra_1>" << endl << endl;
    cout << "   Accepted Extra Arguments:" << endl;
    cout << "      no extra              - Force use of both (incl. GSV)" << endl;
    cout << "      b / both            * - Force use of both (incl. GSV)" << endl;
    cout << "      w / wifi            * - Force use of WiFi (incl. GSV)" << endl;
    cout << "      l / lte             * - Force use of LTE (incl. GSV)" << endl << endl << endl;


    cout << "  ----------------------------------------------------------" << endl;
    cout << "  ----                Change Axis Sizes                 ----" << endl;
    cout << "  ----                                                  ----" << endl;
    cout << "   Call Argument:" << endl;
    cout << "      -x <int>              - For the X-axis" << endl;
    cout << "      -y <int>              - For the Y-axis" << endl << endl;
    cout << "   Accepted Arguments:" << endl;
    cout << "      Any int               - Int of the size you want" << endl << endl << endl;


    cout << "  ----------------------------------------------------------" << endl;
    cout << "  ----            Signal Monitor Sleep Time             ----" << endl;
    cout << "  ----                                                  ----" << endl;
    cout << "   Call Argument:" << endl;
    cout << "      -s <int>"<< endl;
    cout << "      -sleep <int>" << endl << endl;
    cout << "   Accepted Arguments:" << endl;
    cout << "      Any int               - Seconds of delay you want" << endl << endl << endl;


    cout << "  ----------------------------------------------------------" << endl;
    cout << "  ----    Setting for Disabling Hex-Char Coordinates    ----" << endl;
    cout << "  ----                                                  ----" << endl;
    cout << "   Call Argument:" << endl;
    cout << "      -h" << endl;
    cout << "      -hex" << endl << endl << endl;


    cout << "  ----------------------------------------------------------" << endl;
    cout << "  ---- Verbose, to write out everthing that is going on ----" << endl;
    cout << "  ----                                                  ----" << endl;
    cout << "   Arguments market with * can be written sequencially" << endl;
    cout << "   without needing to call -v / -verbose again" << endl << endl;
    cout << "   Call Argument:" << endl;
    cout << "      -v <extra_1> <extra_2> <extra_3> <extra_4>" << endl;
    cout << "      -verbose <extra_1> <extra_2> <extra_3> <extra_4>" << endl << endl;
    cout << "   Accepted Extra Arguments:" << endl;
    cout << "      no extra              - Prints everything" << endl;
    cout << "      m / msg / message   * - Prints \"important\" messages" << endl;
    cout << "      i / in              * - Prints from incoming traffic" << endl;
    cout << "      o / out             * - Prints from outgoing traffic" << endl;
    cout << "      g / gsv             * - Prints from the GSV system" << endl << endl << endl;
    
    exit(0);
}


void Argument_Setup(int argc, char* argv[]) {
    int axis_change = 0;
    char firstCharacter;

    if ((string) argv[1] == "-help" || (string) argv[1] == "-h") {
        help();
    }
    cout << "  ===================================" << endl;
    cout << "     Control Unit Process Started" << endl;
    cout << "  ===================================" << endl << endl << endl << endl;

    cout << "  ===================================" << endl;
    cout << "   ====  Argument(s) accepted  ====" << endl;
    cout << "    === Argument Setup Started ==="<< endl << endl << endl;

    for (int i = 1; i < argc; i++) {
        string current = argv[i];
        firstCharacter = current.at(0);
        // cout << "First Character in Argument is: " << firstCharacter << endl;
        
        if(firstCharacter == '-') {
            // RAM Grid Argument
            if((string) argv[i] == "-g" || (string) argv[i] == "-grid") {
                cout << "===== RAM Grid Enabled =====" << endl;
                use_grid = 1;
            }

            // Signal Monitoring Process Argument
            if((string) argv[i] == "-m" || (string) argv[i] == "-monitor" || (string) argv[i] == "-monitoring") {
                cout << "  ===== Signal Monitoring Process Creation Disabled =====" << endl;
                monitor = 0;
            }

            // Verbose Argument
            if((string) argv[i] == "-v" || (string) argv[i] == "-verbose") {
                if(argc > i+1) {
                    for (int j = i+1; j < argc; j++) {  // Check for sevaral Verbose arguments
                        string current = argv[j];
                        firstCharacter = current.at(0);

                        if(firstCharacter == '-' && j == i+1) {
                            cout << "  ===== Verbose Enabled =====" << endl;
                            message_only = 0;        // Print messages only
                            troubleshooting_print = 1;
                            print_sen_in = 1;       // Print incoming Sensor related things
                            print_act_out = 1;      // Print outgoing Actuator related things
                            print_GSV = 1;          // Print GSV related things
                            GSV_print = (char*) "-v";
                            GSV_arg_used = (char*) "-a";
                            break;
                        }
                        else if ( (string) argv[j] == "i" || (string) argv[j] == "in") {
                            cout << "  ===== Verbose For Incoming Traffic Enabled =====" << endl;
                            message_only = 0;        // Print messages only
                            print_sen_in = 1;       // Print incoming Sensor related things
                        }
                        else if ( (string) argv[j] == "o" || (string) argv[j] == "out") {
                            cout << "  ===== Verbose For Outgoing Traffic Enabled =====" << endl;
                            message_only = 0;        // Print messages only
                            print_act_out = 1;       // Print incoming Sensor related things
                        }
                        else if ( (string) argv[j] == "g" || (string) argv[j] == "gsv") {
                            cout << "  ===== Verbose For GSV Enabled =====" << endl;
                            message_only = 0;        // Print messages only
                            print_GSV = 1;       // Print incoming Sensor related things
                                // Signal Monitoring Arguments
                            GSV_print = (char*) "-v";
                            GSV_arg_used = (char*) "-a";
                        }
                        else if ( (string) argv[j] == "m" || (string) argv[j] == "msg" || (string) argv[j] == "message") {
                            cout << "  ===== Verbose For Messages Enabled =====" << endl;
                            message_only = 1;        // Print messages only
                                // Signal Monitoring Arguments
                            GSV_print = (char*) "-v";
                            GSV_print_arg = (char*) "m";
                            GSV_arg_used = (char*) "-a";
                        }
                        if(firstCharacter == '-' && j != i+1) {
                            break;
                        }
                    }
                }
                else {
                    cout << "  ===== Verbose Enabled =====" << endl;
                    message_only = 1;        // Print messages only
                    troubleshooting_print = 1;
                    print_sen_in = 1;       // Print incoming Sensor related things
                    print_act_out = 1;      // Print outgoing Actuator related things
                    print_GSV = 1;          // Print GSV related things
                    GSV_print = (char*) "-v";
                    GSV_arg_used = (char*) "-a";
                }
            }

            // Usage of Hex Char Argument
            if((string) argv[i] == "-h" || (string) argv[i] == "-hex") {
                cout << "  ===== Hex Chars Disabled =====" << endl;
                use_hex = 0;
            }

            // Force Technologies Argument
            if((string) argv[i] == "-t" || (string) argv[i] == "-tech" || (string) argv[i] == "-technology") {
                GSV_tech = (char*) "-t";
                GSV_arg_used = (char*) "-a";

                if( argc > i+1) {
                    string current = argv[i+1];
                    firstCharacter = current.at(0);

                    if( (firstCharacter == '-') || (string) argv[i+1] == "b" ||  (string) argv[i+1] == "both") {
                        cout << "  ===== Forced Use of Both Technologies Enabled =====" << endl;
                        GSV_tech_arg = (char*) "b";
                        force_tech = 1;
                    }
                    else if((string) argv[i+1] == "w" || (string) argv[i+1] == "wifi") {
                        cout << "  ===== Forced Use of WiFi =====" << endl;
                        GSV_tech_arg = (char*) "w";
                        force_tech = 2;
                    }
                    else if ((string) argv[i+1] == "l" || (string) argv[i+1] == "lte") {
                        cout << "  ===== Forced Use of LTE =====" << endl;
                        GSV_tech_arg = (char*) "l";
                        force_tech = 3;
                    } 
                }
                else {
                    cout << "  ===== Forced Use of Both Technologies Enabled =====" << endl;
                    GSV_tech_arg = (char*) "b";
                    force_tech = 1;
                }
            }

            // X-Axis Range Argument
            if((string) argv[i] == "-x") {
                
                x_axis = atoi(argv[i+1]);
                cout << "  ===== X-axis range set to " << x_axis << " =====" << endl;
                axis_change = 1;
            }

            // Y-Axis Range Argument
            if((string) argv[i] == "-y") {
                
                y_axis = atoi(argv[i+1]);
                cout << "  ===== Y-axis range set to " << y_axis << " =====" << endl;
                axis_change = 1;
            }

            // Signal Monitoring Sleep Delay
            if((string) argv[i] == "-s" || (string) argv[i] == "-sleep") {
                delay = atoi(argv[i+1]);
                cout << "  ===== Signal Monitoring Sleep Time set to " << delay << " seconds =====" << endl;

                char* temp_char = (char*) malloc(9);
                string temp_str = (string) argv[i] + " " + argv[i+1];
                strcpy(temp_char, temp_str.c_str());

                GSV_arg_used = (char*) "-a";
                GSV_sleep = (char*) argv[i];
                GSV_sleep_arg = (char*) argv[i+1];
            }
        }
    }
    if(axis_change >= 1) {
        coordinates = x_axis * y_axis;
    }
    if(use_grid == 1) {
        cout << "  X-axis size is: " << x_axis << "   &   Y-axis size is: " << y_axis << endl;
        grid = generate_grid(x_axis, y_axis, use_hex);
    }
    cout << "\n\n     ==== Argument Setup Done ====" << endl;
    cout << "  ===================================" << endl << endl << endl;

}

