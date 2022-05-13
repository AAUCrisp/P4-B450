#include "coordinates.h"

struct Coordinate parse_coordinates(char msg[buffer]) 
{
    
    bytes_read = sscanf(msg,"%[^:]:%s:%s", C.tempx, C.tempy);
    
    C.current_x_coordinate = atoi(C.tempx);
    
    C.current_y_coordinate = atoi(C.tempy);
    //C.last_y_coordinate = 5;
    //C.last_x_coordinate = 5;

    /*

    if (C.last_x_coordinate > -1 || C.last_y_coordinate > -1 ) {
        C.movement_x = C.current_x_coordinate - C.last_x_coordinate;
        C.movement_y = C.current_y_coordinate - C.last_y_coordinate;
    }
    */
   C.movement_x = C.current_x_coordinate - last_x_coordinate;
   C.movement_y = C.current_y_coordinate - last_y_coordinate;

   sprintf(C.tempx_feedback, "%d", C.movement_x);
   sprintf(C.tempy_feedback, "%d", C.movement_y);
   //strcat(C.feedback, C.tempx_feedback);
   //strcat(C.feedback, ":");
   //strcat(C.feedback,C.tempy_feedback);
   last_x_coordinate = C.current_x_coordinate;
   last_y_coordinate = C.current_y_coordinate; 

    return C;
   //fflush();
}
/*
int main() 
{
   //struct Coordinate result;

   result = parse_coordinates(string_coordinates);
   
   printf("the robot has moved this much on the x and y-axis: %s \n", result.feedback);

}
*/