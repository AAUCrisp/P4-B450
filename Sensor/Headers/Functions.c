void help() {
    printf("\n\n -- This works! -- \n\n");
    // exit(0);
}

void Argument_Setup(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        // Iteration Setup
        if (strcmp(argv[i], (char*)"-i") == 0 || strcmp(argv[i], (char*)"-iterations") == 0) {
            iter = atoi(argv[i + 1]);

            printf("\nNumber of Iterations is set to: %d\n", iter);
        }
        if (strcmp(argv[i], (char*)"-b") == 0 || strcmp(argv[i], (char*)"-both") == 0) {
            both_tech = 1;
            printf("\nForced both technologies\n");
        }
        if (strcmp(argv[i], (char*)"-m") == 0 || strcmp(argv[i], (char*)"-monitor") == 0 || strcmp(argv[i], (char*)"-monitoring") == 0) {
            monitor = 0;
            printf("\nMonitoring Disabled\n");
        }
    }
}
