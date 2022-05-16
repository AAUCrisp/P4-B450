#include "processData.h"
#include <mem.h>

int movement_x;
int movement_y;
int * process_Data(char msg[buffer]) {
    int * result = malloc(sizeof(int) * 2);
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

    result[0] = movement_x;
    result[1] = movement_y;

    return result;
}

