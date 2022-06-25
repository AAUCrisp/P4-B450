#ifndef EXECUTIONVARIABLE_H
#define EXECUTIONVARIABLE_H

/* Function to receive LTE packets */
void *receiveLTE(void *socket) {
    Sockets *sock = (Sockets *)socket;
    unsigned int LenLTE = sizeof(sock->ServerLTE_RECEIVER);

    /* Execution time variables */
    struct timespec begin, end;
    unsigned long seconds = 0;
    unsigned long nanoseconds = 0;
    double elapsed = 0;

    sock->packet_count_LTE = 0;
    sock->fail_count_LTE = 0;
    sock->Execution_Sum_LTE = 0;
    sock->STOP_LTE = 0;

    while (sock->STOP_LTE != 1) {
        // printf("receiveLTE socket: %d\n", sock->sockLTE_RECEIVER);
        RX_LTE = recvfrom(sock->sockLTE_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_RECEIVER, &LenLTE);
        printf("RX_LTE: %d\n", RX_LTE);
        if (RX_LTE == -1) {
            sock->STOP_LTE++;
        } else {
            sock->STOP_LTE = 0;
        }
        Timestamp();

        fp1 = fopen("Logs/log.txt", "a+");
        fprintf(fp1, "%s %s %s\n", message, curr_time, "LTE");
        fclose(fp1);

        if (print_COMMANDS == 1) {
            // printf("LTE || LTE-Thread id = %ld\n", pthread_self());
            printf("LTE || Message from LTE received at: %s\n", curr_time);
            printf("LTE || Message: %s from Control Unit \n\n", message);
        }

        /* Start timing code execution of code */
        clock_gettime(CLOCK_REALTIME, &begin);

        processData(message);

        /* Stop timing code execution of code */
        clock_gettime(CLOCK_REALTIME, &end);

        seconds = end.tv_sec - begin.tv_sec;
        nanoseconds = end.tv_nsec - begin.tv_nsec;

        /* Calculation of elapsed time sum */
        elapsed = seconds + nanoseconds * 1e-9;
        if (elapsed > 10000) {
            sock->fail_count_LTE++;
            elapsed = 0;
        }
        sock->Execution_Sum_LTE += elapsed;
        sock->packet_count_LTE++;
    }
    cout << "\nLTE : Execution_Sum: " << sock->Execution_Sum_LTE << endl;
    printf("LTE : Total failed counts: %d\n", sock->fail_count_LTE);
    printf("LTE : Total packets received via LTE: %d\n\n", sock->packet_count_LTE);
    sleep(10);
    return 0;
}

#endif