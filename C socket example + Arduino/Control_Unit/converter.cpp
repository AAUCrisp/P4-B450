#include <string>
// #include <stdio>

// Test Variables
int data = 250;


string grid = [];       // Array to contain the grid in the RAM
int x_axis = 50;      // Length of the x-axis of the grid
int y_axis = 50;      // Length of the y-axis of the grid
// int x_axis = 5000;      // Length of the x-axis of the grid
// int y_axis = 5000;      // Length of the y-axis of the grid


/* Option to store the grid in memory and save processing power */
void generate_grid(int x_axis, int y_axis) {
    printf("\nCreating GRID!\n");

    int x_count = 1;
    int y_count = 1;
    for (int i = 0; i < (x_axis * y_axis); i++) {
        grid[i] = string(x_axis) . ":" . string(y_axis);

        if (x_count == x_axis){
            x_count = 1;
            y_count++;
        }
    }
}

/* Option for processing the coordinates instead of using memory */
string convert_to_grid(int number) {
    int x = number % x_axis;
    int y = (number + 1) / x_axis; 

    string coordinate = string(x) . ":" . string(y);

    return coordinate;
}


void main(){
    printf("\nSensor Data is: %d", data);
    generate_grid(x_axis, y_axis);
    string cpu_command = convert_to_grid(data);
    printf("\nCPU Command is: ");
    //printf(cpu_command);
    printf("\nRAM Command is: ");
    // printf(grid[data]);

}