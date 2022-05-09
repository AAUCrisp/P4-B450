#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define buffer 1024
char string_coordinates[] = "499:2";
int bytes_read;
//int x_coordinate, y_coordinate;
//char tempx[buffer], tempy[buffer];
//int x, y;


struct Coordinate {
    char tempx[buffer]; char tempy[buffer];
    int last_x_coordinate; int last_y_coordinate;
    int current_x_coordinate; int current_y_coordinate;
    int movement_x; int movement_y;
    char tempx_feedback[buffer]; char tempy_feedback[buffer]; 
    char feedback[buffer];
    
};
struct Coordinate C;