#define buffer 1024
#include <string>
#include <fstream>
#include <sstream>
#include "hex.cpp"



//char string_coordinates[] = "499:2";
int bytes_read;
int x, y;
int last_x_coordinate;
int last_y_coordinate;
char tempx [buffer];
char tempy [buffer];

std::ofstream File;
//std::ofstream WiFi_File;


//std::string tempx;
//std::string tempy;
int movement_x;
int movement_y;




//int movement_x;
//int movement_y;

void update_last_coordinate(int number1, int number2)
{
    last_x_coordinate = number1;
    last_y_coordinate = number2;
}


int * process_Data(char msg[buffer]) {
    int * result = (int *)malloc(sizeof(int) * 2);

    bytes_read = sscanf(msg, "%[^:%s]:%s:", tempx, tempy);
    cout << "Temp X is: " << tempx << endl;
    cout << "Temp Y is: " << tempy << endl;
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
    
    return result;
}