#include "processData.h"
#include "hex.cpp"


//int movement_x;
//int movement_y;

int update_last_coordinate(int number1, int number2)
{
    last_x_coordinate = number1;
    last_y_coordinate = number2;
}


int * process_Data(char msg[buffer]) {
    int * result = (int *)malloc(sizeof(int) * 2);

    bytes_read = sscanf(msg, "%[^:]:%s:%s", tempx, tempy);
    x = hex_to_int(tempx);
    y = hex_to_int(tempy);

    //x = atoi(tempx);
    //y = atoi(tempy);

    if (last_x_coordinate && last_y_coordinate == 0)
    {
        int movement_x = x;
        int movement_y = y;
    } else {
        int movement_x = x - last_x_coordinate;
        int movement_y = y - last_y_coordinate;
    }

    result[0] = movement_x;
    result[1] = movement_y;
    update_last_coordinate(x, y);
    
    return result
}

