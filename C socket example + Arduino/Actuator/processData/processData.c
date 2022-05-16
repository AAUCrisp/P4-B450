#define buffer 1024
char string_coordinates[] = "499:2";
int bytes_read;
int x, y;
int arr[2];
int last_x_coordinate;
int last_y_coordinate;
char tempx [buffer];
char tempy [buffer];
int movement_x;
int movement_y;


void process_Data(char msg[buffer]) {
    bytes_read = sscanf(msg, "%[^:]:%s:%s", tempx, tempy);
    x = atoi(tempx);
    y = atoi(tempy);
    arr[0] = x;
    arr[1] = y;
    return arr;
}

