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
int x_axis = 5000;      // Length of the x-axis of the grid
int y_axis = 5000;      // Length of the y-axis of the grid
int coordinates = x_axis*y_axis;
string* grid = new string[coordinates]; // Don't forget to delete [] a; when you're done!


/* --- Conversion from Integer to Hex-chars --- */
string int_to_hex(int data, int back = 0) {
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
    return result;
}


/* --- Option to store the grid in memory and save processing power --- */
void generate_grid(int x_axis, int y_axis, int hex = 1) {
    int x_count = 1;
    int y_count = 1;
    
    cout << "  --- BUILDING GRID ---  " << endl;
    printf("\n0 Rows Done.");

    for (int i = 0; i < (x_axis * y_axis); i++) {
        string x;
        string y;
        if(hex == 1) {
            x = int_to_hex(x_count);
            y = int_to_hex(y_count);
        }
        else {
            x = to_string(x_count);
            y = to_string(y_count);
        }

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
string convert_to_coordinate(int number, int hex=1) {
    // int x = (number % (x_axis+1));
    int x = ((number-1) % (x_axis))+1;
    int y = ((number-1) / x_axis)+1; 

    string str_x;
    string str_y;
    if(hex == 1) {
        str_x = int_to_hex(x);
        str_y = int_to_hex(y);
    }
    else{
        str_x = to_string(x);
        str_y = to_string(y);
    }

    string coordinate = str_x + ":" + str_y;

    return coordinate;
}


int main(int argc, char *argv[]){
    // Test Variables
    int data_int = 9;       // Static Test variable
    string data_str = to_string(data_int);     // For "Firm" data
    int hex = 1;

    if(argc > 1) {
        cout << "\nArgument accepted." << endl;
        data_str = argv[1];
        data_int = atoi(argv[1]);
        cout << "Third Argument?: " << argv[3] << endl;
    }
    if(argc == 3) {     // For enabling or disabling Hex-char output.
        cout << "Hex argument received: " << argv[2] << endl;
        hex = atoi(argv[2]);
    }

    else {
        cout << "\nNo arguments inserted, running staticly." << endl;
    }
    
    generate_grid(x_axis, y_axis, hex);
    
    /* --  Loop for printing out the grid coordinate... TROUBLESHOOTING  --  */
    // for (int i = 0; i < coordinates; i++) {
    //         cout << "Sensor Data: " << i+1 << "   Becomes GRID Position   " << grid[i] << endl;
    // }

    string cpu_command = convert_to_coordinate(data_int, hex);

    // -- Print out results -- 
    cout << "\nSensor Data is: " << data_str << endl;
    cout << "\nRAM Command is: " << grid[data_int-1] << endl;
    cout << "\nCPU Command is: " << cpu_command << endl;

}