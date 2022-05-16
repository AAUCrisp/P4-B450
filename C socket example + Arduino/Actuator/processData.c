#include "processData.h"
int movement_x;
int movement_y;
void process_Data(char msg[buffer]) {
    bytes_read = sscanf(msg, "%[^:]:%s:%s", tempx, tempy);
    x = atoi(tempx);
    y = atoi(tempy);

    if (last_x_coordinate && last_y_coordinate == 0)
    {
        int movement_x = x;
        int movement_y = y;
    } else {
        int movement_x = x - last_x_coordinate;
        int movement_y = y - last_y_coordinate;
    }

    arr[0] = movement_x;
    arr[1] = movement_y;

    return arr;
}

void ()