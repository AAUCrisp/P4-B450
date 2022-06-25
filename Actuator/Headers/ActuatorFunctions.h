#ifndef ACTUATOR_FUNCTIONS_H
#define ACTUATOR_FUNCTIONS_H

#include <iostream>
#include <iomanip>
#include <fstream>


//void logData(int *Arr);

void update_last_coordinate(int number1, int number2);

char processData(char msg[]);

//string int_to_hex(int data, int back);

/* Function to generate timestamp */
char *Timestamp();

//int hex_to_int(string data);

#endif