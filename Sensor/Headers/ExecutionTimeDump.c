/* NEW WORKS
                Execution_Time[i] = (double)(Clock_End - Clock_Start) / CLOCKS_PER_SEC;

            if (Execution_Time[i] > 10000) {
                fail_count++;
                // printf("Execution_Time[%d]: %f\n", i, (double)Execution_Time[i]);
                Execution_Time[i] = 0;
            } else {
                // printf("Execution_Time[%d]\n", i);
                // printf("Execution_Time[%d]: %Lf\n", i, Execution_Time[i]);
                Execution_Sum += Execution_Time[i];
            } */

/* Works */
/*
//clock_gettime(CLOCK_REALTIME, &end);

seconds = end.tv_sec - begin.tv_sec;
nanoseconds = end.tv_nsec - begin.tv_nsec;

elapsed = seconds + nanoseconds * 1e-9;
if (elapsed > 10000) {
    fail_count++;
   // printf("fail count: %d\n", fail_count);
   // printf("Failed elapsed time: %f\n", elapsed);
} else {
   // printf("\nelapsed time: %f\n", elapsed);
}
// sleep(5);
Execution_Time[i] = elapsed;
if (Execution_Time[i] > 10000) {
   // printf("Failed Execution_Time[%d]: %Lf\n", i, Execution_Time[i]);
    //sleep(1);
    Execution_Time[i] = 0;
   // printf("Forced Execution_Time[%d]: %Lf\n", i, Execution_Time[i]);
    sleep(1);
} else {
    printf("Execution_Time[%d]\n", i);
    //printf("Execution_Time[%d]: %Lf\n", i, Execution_Time[i]);
}
// sleep(5);
Execution_Sum += Execution_Time[i];
if (Execution_Sum > 10000) {
    fail_count++;
   // printf("fail count: %d\n", fail_count);
   // printf("Failed Execution_Sum: %Lf\n", Execution_Sum);
    //sleep(5);
} else {
    //printf("Execution_Sum: %Lf\n", Execution_Sum);
}*/

/* Old execution time thing */
// seconds = end_program.tv_sec - begin_program.tv_sec;
// nanoseconds = end_program.tv_nsec - begin_program.tv_nsec;

// double time_spent = seconds + (nanoseconds / 1000000000.0);

// clock_gettime(CLOCK_REALTIME, &end_program);

// time_spent = end - start
// long timestamp = (long)(Time_Ended - Time_Started);
// long milliseconds = (long)(timestamp / 1000) % 1000;
// long seconds = (((long)(timestamp / 1000) - milliseconds) / 1000) % 60;
// long minutes = (((((long)(timestamp / 1000) - milliseconds) / 1000) - seconds) / 60) % 60;
// long hours = ((((((long)(timestamp / 1000) - milliseconds) / 1000) - seconds) / 60) - minutes) / 60;