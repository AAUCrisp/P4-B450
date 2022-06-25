#ifndef EXECUTIONVARIABLE_H
#define EXECUTIONVARIABLE_H

/* Execution timing variable */
class ExecutionVariables {
   public:
    static int STOP;
    static int packet_count_LTE;
    static int packet_count_WiFi;
    static int fail_count;
    static long double Execution_Sum;
};

#endif