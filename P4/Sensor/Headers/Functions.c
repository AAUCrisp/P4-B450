void help() {
    printf("\n\n -- This works! -- \n\n");
    // exit(0);
}

void Argument_Setup(int argc, char* argv[]) {
    int axis_change = 0;
    char firstCharacter;
    const char* argument;
    argument = (char*) argv[1];

    char* test;
    test = "o\0";
    // test = {'o','\0'};

    printf("\n\n First Argument is: %s \n\n", argument);

    printf("\n\nInside Argument Function \n\n");
    if ( (char*)argument == "o") {
        printf("\nInside If statement\n\n");
        help();
    }

    // NÅR DU FATTER AT MÅLE PÅ CHARS, KAN DU "INDKOMMENTERE" LOOPET NEDENFOR
    // HUSK AT RETTE TIL... DEN MÅLER PÅ STRINGS FRA C++

    /*
    for (int i = 1; i < argc; i++) {
        string current = argv[i];
        firstCharacter = current.at(0);
        // cout << "First Character in Argument is: " << firstCharacter << endl;
        
        if(firstCharacter == '-') {
            // Signal Monitoring Process Argument
            if((string) argv[i] == "-m" || (string) argv[i] == "-monitor" || (string) argv[i] == "-monitoring") {
                if ( (string) argv[i+1] == "no" || (string) argv[i+1] == "0" || (string) argv[i+1] == "false") {
                    cout << "===== Signal Monitoring Process Creation Disabled =====" << endl;
                }
                else if ( (string) argv[i+1] == "yes" || (string) argv[i+1] == "1" || (string) argv[i+1] == "true") {
                    cout << "===== Signal Monitoring Process Creation Enabled =====" << endl;
                }
                else { cout << "===== Invalid argument for \"Signal Monitoring Process Start\" =====" << endl; }
            }

            // Verbose Argument
            if((string) argv[i] == "-v" || (string) argv[i] == "-verbose") {
                    if ( (string) argv[i+1] == "no" || (string) argv[i+1] == "0" || (string) argv[i+1] == "false") {
                    cout << "===== Hex Chars Disabled =====" << endl;

                }
                
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
                else { cout << "===== Invalid argument for \"Forced Both Technologies\"===== " << endl; 
                }
            }
        }
        
    }
    */
}

