#ifndef ACTUATOR_FUNCTIONS_H
#define ACTUATOR_FUNCTIONS_H

#include <iostream>
#include <iomanip>
using namespace std;

void logData(int *Arr);

void update_last_coordinate(int number1, int number2);

int *processData(char msg[]);

string int_to_hex(int data, int back);

int hex_to_int(string data);

#endif