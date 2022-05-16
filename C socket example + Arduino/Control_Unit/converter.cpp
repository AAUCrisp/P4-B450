#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include<iomanip>
#include <cstdio>
#include<iostream>
using namespace std;


// -- Setup Area --
int x_axis = 5;      // Length of the x-axis of the grid
int y_axis = 5;      // Length of the y-axis of the grid
int coordinates = x_axis*y_axis;
string* grid = new string[coordinates]; // Don't forget to delete [] a; when you're done!


/* Option to store the grid in memory and save processing power */
void generate_grid(int x_axis, int y_axis) {
    int x_count = 1;
    int y_count = 1;
    
    cout << "  --- BUILDING GRID ---  " << endl;
    printf("\n0 Rows Done.");

    for (int i = 0; i < (x_axis * y_axis); i++) {

        string x = to_string(x_count);
        string y = to_string(y_count);
        grid[i] = x + ":" + y;      // Write coodinate to grid array
        

        if(x_count == (x_axis)){        // When the end of the x-axis has been reached
            printf("\r%d Rows Done", y_count);      // Delete last output line and replace with current process
            fflush(stdout);     // Forces the print to go through before sleeping
            // cout << y_count <<  " row done. " << endl;
            x_count = 1;    // Start at the beginning again
            y_count++;      // Increase the y-axis coordinate
            // sleep(1);       // For troubleshooting only, program is too fast i smaller grids
        }
        else {
            x_count++;      // Next x-axis position
        }
    }
    printf("\rAll Rows Done.\n\n");


    return;
}
  

/* Option for processing the coordinates instead of using memory */
string convert_to_coordinate(int number) {
    int x = number % x_axis;
    int y = ((number) / x_axis)+1; 


    string  str_x = to_string(x);
    string str_y = to_string(y);
    string coordinate = str_x + ":" + str_y;

    return coordinate;
}


int main(int argc, char *argv[]){
    // Test Variables
    int data_int = 9;       // Static Test variable
    string data_str = to_string(data_int);     // For "Firm" data

    if(argc == 2) {
        cout << "\nArgument accepted." << endl;
        data_str = argv[1];
        data_int = atoi(argv[1]);
    }

    else {
        cout << "\nNo arguments inserted, running staticly." << endl;
    }
    
    generate_grid(x_axis, y_axis);
    
    for (int i = 0; i < coordinates; i++) {
            cout << "Sensor Data: " << i+1 << "   Becomes GRID Position   " << grid[i] << endl;
        }

    string cpu_command = convert_to_coordinate(data_int);

    // -- Print out results -- 
    cout << "\nSensor Data is: " << data_str << endl;
    cout << "\nRAM Command is: " << grid[data_int-1] << endl;
    cout << "\nCPU Command is: " << cpu_command << endl;

}