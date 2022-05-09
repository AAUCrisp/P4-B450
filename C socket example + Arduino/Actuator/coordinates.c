#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define buffer 1024

char string_coordinates[] = "499:2";
int x_coordinate, y_coordinate;
char tempx[buffer], tempy[buffer];
int bytes_read;
int x, y;

int parse_coordinates(char msg[]) 
{
    bytes_read = sscanf(msg,"%[^:]:%s:%s", tempx, tempy);
    x_coordinate = atoi(tempx);
    y_coordinate = atoi(tempy);
    //printf ("X coordinate is:%d \n", x_coordinate);
    //printf ("y coordinate is: %d \n", y_coordinate);
    return x_coordinate, y_coordinate;
}

int main() {

   x, y = parse_coordinates(string_coordinates);
    printf ("X coordinate is:%d \n", x);
    printf ("y coordinate is: %d \n", y);
}