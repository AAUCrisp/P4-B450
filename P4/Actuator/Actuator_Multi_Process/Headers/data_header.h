//#include "server_header.h"


/*Variables used to process data*/
int bytes_read;
int x, y;
int last_x_coordinate;
int last_y_coordinate;
int movement_x;
int movement_y;

char tempx [buffer];
char tempy [buffer];

std::ofstream File;


/*This function updates the last coordinates, for next computation of finding movement over the X and Y axis*/
void update_last_coordinate(int number1, int number2)
{
    last_x_coordinate = number1;
    last_y_coordinate = number2;
}


/* This functions Parses the received message into a specified format. And finds the difference between the current and last coordinates.*/
int * process_Data(char msg[buffer]) {
    int * result = (int *)malloc(sizeof(int) * 2);          //creates an int pointer to the array containing movement over the x and y axis. 

    bytes_read = sscanf(msg, "%[^:%s]:%s:", tempx, tempy); // Parses the received char array, into two seperate char arrays
    cout << "Temp X is: " << tempx << endl;                // prints out the char arrays containing x and y coordinates. 
    cout << "Temp Y is: " << tempy << endl;
    x = hex_to_int(tempx);                                 // Converts the hex-chars of the x coordinate to an int
    y = hex_to_int(tempy);                                 // Converts the hex-chars of the x coordinate to an int


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
    
    return result;                                      // returns the array containing movement over the x and y axis. 
}

