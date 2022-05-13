#include <string>
#include<iostream>
using namespace std;


int main() {
}


// Test Variables
int data = 250;


std::string grid = [];       // Array to contain the grid in the RAM
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
        grid[i] = std::string(x_axis) . ":" . std::string(y_axis);

        if (x_count == x_axis){
            x_count = 1;
            y_count++;
        }
    }
}

/* Option for processing the coordinates instead of using memory */
std::string convert_to_grid(int number) {
    int x = number % x_axis;
    int y = (number + 1) / x_axis; 

    std::string coordinate = string(x) . ":" . string(y);

    return coordinate;
}


int main(){
   
   string animals[4] = {"Elephant", "Lion", "Deer", "Tiger"}; //The
   string type array
   for (int i = 0; i < 4; i++)
      cout << animals[i] << endl;



    printf("\nSensor Data is: %d", data);
    generate_grid(x_axis, y_axis);
    std::string cpu_command = convert_to_grid(data);
    printf("\nCPU Command is: ");
    //printf(cpu_command);
    printf("\nRAM Command is: ");
    // printf(grid[data]);

}