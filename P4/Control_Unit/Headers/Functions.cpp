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


    Time_Started = clock();
    while( int i < iter) {
        Clock_Start = clock(); // added function
        message = (void*)receiveWiFi((void*)&sock);
        if(troubleshooting_print == 1) {
            cout << "WiFi Command Function || Message Parsed from Sockets (data & timestamp) is: " << (const char*)message << endl;
        }
        sscanf((const char*)message, "%d %[^\n]", &data, msgDump);
        cout << "WiFi Command Function || Message Parsed from Sockets as INT is: " << data << endl;
        if(use_grid == 1) {
            coordinate = grid[data];
        }
        else {
            coordinate = convert_to_coordinate(data, use_hex);
        }
        cout << "WiFi Command Function || Coordinate for Actuator is: " << coordinate << "\n\n\n" << endl;
        strcpy(WiFimsg, coordinate.c_str());
        transmit_command(&sock, WiFimsg);
        
        Clock_End = clock(); // added function to time 

        // time executions code under
        Execution_Temp = (Clock_End - Clock_Start) / CLOCKS_PER_SEC;
        int shit = isnan(Execution_Time[i]); 
        if (shit == 0) {
            Execution_Time[i] += Execution_Temp;
            if (Execution_Time[i] >= -1000000000 || Execution_Time [i] <= 1000000000) {
                    Execution_Sum += Execution_Time[i];
            } else {
                    fail_count++;
                    printf("WiFi: Fail counter: %d\n", fail_count);
                    // printf("Execution_Sum exceeded 10000000\n");
                }
        } else {
            fail_count++;
            printf("WiFi: Fail counter: %d\n", fail_count);
        }  

        printf("WiFi: Execution_Time[%d]: %f\n", i, Execution_Time[i]);
        printf("WiFi: Execution_Sum = %f\n", Execution_Sum);
    }    
    Time_Ended = clock();
    long timestamp = (long)(Time_Ended - Time_Started);
    long milliseconds = (long)(timestamp / 1000) % 1000;
    long seconds = (((long)(timestamp / 1000) - milliseconds) / 1000) % 60;
    long minutes = (((((long)(timestamp / 1000) - milliseconds) / 1000) - seconds) / 60) % 60;
    long hours = ((((((long)(timestamp / 1000) - milliseconds) / 1000) - seconds) / 60) - minutes) / 60;

    Execution_Average = Execution_Sum / iter;
    printf("WiFi: Execution average: %f ms\n", Execution_Average);
    printf("WiFi: Total time: %ld\n", (Time_Ended - Time_Started));
    printf("WiFi: Total_Time_Elapsed [HH:MM:SS:MS]: %ld:%ld:%ld:%ld\n", hours, minutes, seconds, milliseconds);
    printf("WiFi: Total failed counts: %d\n", fail_count);
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
    Time_Started = clock();
    while(int j < iter) {
        Clock_Start = clock();

        message = (void*)receiveLTE((void*)sock);
        if(troubleshooting_print == 1) {
           cout << "LTE Command Function || Message Parsed from Sockets (data & timestamp) is: " << (const char*)message << endl;
        }
        sscanf((const char*)message, "%d %[^\n]", &data, msgDump);
        cout << "LTE Command Function || Message Parsed from Sockets as INT is: " << data << endl;
        if(use_grid == 1) {
            coordinate = grid[data];
        }
        else {
            coordinate = convert_to_coordinate(data, use_hex);
        }
        cout << "LTE Command Function || Coordinate for Actuator is: " << coordinate << "\n\n\n" << endl;
        strcpy(LTEmsg, coordinate.c_str());
        transmit_command(sock, LTEmsg);
        Clock_End = clock();

        // time executions code under
        Execution_Temp = (Clock_End - Clock_Start) / CLOCKS_PER_SEC;
        int shit = isnan(Execution_Time[i]); 
        if (shit == 0) {
            Execution_Time[i] += Execution_Temp;
            if (Execution_Time[i] >= -1000000000 || Execution_Time [i] <= 1000000000) {
                    Execution_Sum += Execution_Time[i];
            } else {
                    fail_count++;
                    printf("WiFi: Fail counter: %d\n", fail_count);
                    // printf("Execution_Sum exceeded 10000000\n");
                }
        } else {
            fail_count++;
            printf("WiFi: Fail counter: %d\n", fail_count);
        }  

        printf("WiFi: Execution_Time[%d]: %f\n", i, Execution_Time[i]);
        printf("WiFi: Execution_Sum = %f\n", Execution_Sum);
    }
    Time_Ended = clock();
    long timestamp = (long)(Time_Ended - Time_Started);
    long milliseconds = (long)(timestamp / 1000) % 1000;
    long seconds = (((long)(timestamp / 1000) - milliseconds) / 1000) % 60;
    long minutes = (((((long)(timestamp / 1000) - milliseconds) / 1000) - seconds) / 60) % 60;
    long hours = ((((((long)(timestamp / 1000) - milliseconds) / 1000) - seconds) / 60) - minutes) / 60;

    Execution_Average = Execution_Sum / iter;
    printf("WiFi: Execution average: %f ms\n", Execution_Average);
    printf("WiFi: Total time: %ld\n", (Time_Ended - Time_Started));
    printf("WiFi: Total_Time_Elapsed [HH:MM:SS:MS]: %ld:%ld:%ld:%ld\n", hours, minutes, seconds, milliseconds);
    printf("WiFi: Total failed counts: %d\n", fail_count);

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
    cout << "  ----      Start of the Signal Monitoring Process      ----" << endl;
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
    cout << "  ----      Force Communication on Both LTE & WiFi      ----" << endl;
    cout << "  ----                                                  ----" << endl;
    cout << "   Call Argument:" << endl;
    cout << "      -b" << endl;
    cout << "      -both" << endl << endl << endl;


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
    cout << "      -v <extra_1> <extra_2> <extra_3>" << endl;
    cout << "      -verbose <extra_1> <extra_2> <extra_3>" << endl << endl;
    cout << "   Accepted Extra Arguments:" << endl;
    cout << "      no extra              - Prints everything" << endl;
    cout << "      i / in              * - Prints from incoming traffic" << endl;
    cout << "      o / out             * - Prints from outgoing traffic" << endl;
    cout << "      g / gsv             * - Prints from the GSV system" << endl << endl << endl;
    
    exit(0);
}


void Argument_Setup(int argc, char* argv[]) {
    int axis_change = 0;
    char firstCharacter;

    if ((string) argv[1] == "-help") {
        help();
    }
    cout << "============================\nControl Unit Process Started\n============================\n\n" << endl;

    cout << "\nArgument(s) accepted." << endl << endl;

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
                cout << "===== Signal Monitoring Process Creation Disabled =====" << endl;
                monitor = 0;
            }

            // Verbose Argument
            if((string) argv[i] == "-v" || (string) argv[i] == "-verbose") {
                if(argc > i+1) {
                    for (int j = i+1; j < argc; j++) {  // Check for sevaral Verbose arguments
                        string current = argv[j];
                        firstCharacter = current.at(0);

                        if(firstCharacter == '-') {
                            cout << "===== Verbose Enabled =====" << endl;
                            message_only = 0;        // Print messages only
                            troubleshooting_print = 1;
                            print_sen_in = 1;       // Print incoming Sensor related things
                            print_act_out = 1;      // Print outgoing Actuator related things
                            print_GSV = 1;          // Print GSV related things
                            GSV_arg_print = (char*) "-v";
                            GSV_arg_used = (char*) "-a";
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
                                // Signal Monitoring Arguments
                            GSV_arg_print = (char*) "-v";
                            GSV_arg_used = (char*) "-a";
                        }
                        else { 
                            cout << "===== Invalid argument for \"Verbose\" =====" << endl; 
                        }
                    }
                }
                else {
                    cout << "===== Verbose Enabled =====" << endl;
                    message_only = 0;        // Print messages only
                    troubleshooting_print = 1;
                    print_sen_in = 1;       // Print incoming Sensor related things
                    print_act_out = 1;      // Print outgoing Actuator related things
                    print_GSV = 1;          // Print GSV related things
                    GSV_arg_print = (char*) "-v";
                    GSV_arg_used = (char*) "-a";
                }
            }

            // Usage of Hex Char Argument
            if((string) argv[i] == "-h" || (string) argv[i] == "-hex") {
                cout << "===== Hex Chars Disabled =====" << endl;
                use_hex = 0;
            }

            // Force Both Technologies Argument
            if((string) argv[i] == "-b" || (string) argv[i] == "-both") {
                cout << "===== Forced Use of Both Technologies Enabled =====" << endl;
                force_both = 1;
                GSV_arg_both = (char*) "-b";
                GSV_arg_used = (char*) "-a";
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

            // Signal Monitoring Sleep Delay
            if((string) argv[i] == "-s" || (string) argv[i] == "-sleep") {
                delay = atoi(argv[i+1]);
                cout << "===== Signal Monitoring Sleep Time set to " << delay << " seconds =====" << endl;

                char* temp_char = (char*) malloc(9);
                string temp_str = (string) argv[i] + " " + argv[i+1];
                strcpy(temp_char, temp_str.c_str());

                GSV_arg_used = (char*) "-a";
                GSV_sleep_arg = (char*) argv[i];
                GSV_sleep_delay = (char*) argv[i+1];
            }

        }
    }
    if(axis_change >= 1) {
        coordinates = x_axis * y_axis;
    }
    if(use_grid == 1) {
        cout << "X-axis size is: " << x_axis << "   &   Y-axis size is: " << y_axis << endl;
        grid = generate_grid(x_axis, y_axis, use_hex);
    }
    cout << "\n\n==== Argument Setup Done ====\n\n" << endl;

}

