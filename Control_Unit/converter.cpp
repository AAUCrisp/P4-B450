#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include<iomanip>
#include <cstdio>
#include<iostream>
using namespace std;


#ifndef CONVERT_VARIABLES
#define CONVERT_VARIABLES
/* -- Test Data Variable-- */
// int data_int = 1500000;     // Static Test Variable
// int x_axis = 5000;      // Length of the x-axis of the grid
// int y_axis = 5000;      // Length of the y-axis of the grid
// int coordinates = x_axis*y_axis;
// string* grid = new string[coordinates]; // Don't forget to delete [] a; when you're done!
#endif


/* --- Conversion from Integer to Hex-chars --- */
string int_to_hex(int data, int back = 0) {
    // cout << "INT TO HEX" << endl;
    stringstream stream;    // Create hex-conversion object

    stream << hex << data;    // Tell it to convert from int to hex-chars
    string result( stream.str() );      // Do the conversion
    // cout << "Converted to Hex-chars is: " << result << endl;     // Print the result


    /* -- Coversion back in same object, which already has the value -- */
    if(back == 1) {
        int x;
        stream >> x;        // Write the coverted integer to the integer variable
        cout << "Converted back is: " << x << endl;
    }
    // cout << "INT TO HEX END" << endl;
    return result;
}


/* --- Option to store the grid in memory and save processing power --- */
string* generate_grid(int x_axis, int y_axis, int use_hex = 1) {
    // int coor = x_axis * y_axis;
    // string* grid = new string[coor]; // Don't forget to delete [] a; when you're done!
    string* grid = new string[coordinates]; // Don't forget to delete [] a; when you're done!

    int x_count = 1;
    int y_count = 1;
    
    cout << "\n\n  --- BUILDING GRID FOR MEMORY ---  " << endl;
    printf("\n0 Rows Done.");

    for (int i = 0; i < (x_axis * y_axis); i++) {
        string x;
        string y;
        if(use_hex == 1) {
            x = int_to_hex(x_count);
            y = int_to_hex(y_count);
        }
        else {
            x = to_string(x_count);
            y = to_string(y_count);
        }

        grid[i] = x + ":" + y;      // Write coordinate to grid array
        

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

    return grid;
}


/* --- Print Grid Function --- */
void print_grid(string* grid, int coordinates) {
    /* --  Loop for printing out the grid coordinate... TROUBLESHOOTING  --  */
    for (int i = 0; i < coordinates; i++) {
            cout << "Sensor Data: " << i+1 << "   Becomes GRID Position   " << grid[i] << endl;
    }
}
  

/* Option for processing the coordinates instead of using memory */
// string convert_to_coordinate(int number, int x_axis, int y_axis, int hex=1) {
string convert_to_coordinate(int number, int use_hex=1) {
    // int x = (number % (x_axis+1));
    // cout << "\n\nIS THIS HERE? \n\n" << endl;
    int x = ((number-1) % (x_axis))+1;
    int y = ((number-1) / x_axis)+1; 

    string str_x;
    string str_y;
    if(use_hex == 1) {
        str_x = int_to_hex(x);
        // cout << "Between HEX'es" << endl;
        str_y = int_to_hex(y);
    }
    else{
        str_x = to_string(x);
        str_y = to_string(y);
    }

    string coordinate = str_x + ":" + str_y;
    // cout << "\n\nTHIS IS THE END!! \n\n" << endl;

    return coordinate;
}
