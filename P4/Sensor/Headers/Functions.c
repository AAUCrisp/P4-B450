void help() {
    printf("\n\n -- This works! -- \n\n");
    // exit(0);
}

void Argument_Setup(int argc, char* argv[]) {
    // int axis_change = 0;
    // char firstCharacter;
    // char* argument;
    // argument = argv[1];

    // char* test = "cancer";
    for (int i = 1; i < argc; i++) { 
        // Iteration Setup
        if ( strcmp(argv[i], (char*) "-i") == 0 || strcmp(argv[i], (char*) "-iterations") == 0) {
            // printf("Iter before augmentation is: %d", iter);
            iter = atoi(argv[i+1]);
            // printf("Argument before augmentation is: %d", iter);

            printf("\nNumber of Iterations is set to: %d\n", iter);
            // printf("Iter before augmentation is: %d", iter);
        }
        if ( strcmp(argv[i], (char*) "-b") == 0 || strcmp(argv[i], (char*) "-both") == 0) {
            both_tech = 1;
            printf("\nForced both technologies\n");
        }
        if ( strcmp(argv[i], (char*) "-m") == 0 || strcmp(argv[i], (char*) "-monitor") == 0 || strcmp(argv[i], (char*) "-monitoring") == 0) {
            monitor = 0;
            printf("\nMonitoring Disabled\n");
        }
    }


    // int test1 = scanf("%10i", *argument);

    printf("\n\nInside Argument Function \n\n");
    /*
    if (argument == test) {
        printf("\nInside If statement\n\n");
        help();
    }*/

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
