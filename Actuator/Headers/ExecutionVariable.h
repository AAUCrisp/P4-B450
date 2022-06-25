#ifndef EXECUTIONVARIABLE_H
#define EXECUTIONVARIABLE_H

/* Execution timing variable */
static bool EXECUTION;
static int testvar;
static int STOP = 0;

extern int packet_count_LTE = 0;
extern int packet_count_WiFi = 0;
extern int fail_count = 0;
extern long double Execution_Sum = 0;

#endif