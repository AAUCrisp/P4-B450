#include<iostream> 
using namespace std; 

// -- Setup Area --
int x_axis = 5000;      // Length of the x-axis of the grid
int y_axis = 5000;      // Length of the y-axis of the grid
int coordinates = x_axis*y_axis;
string* grid = new string[coordinates]; // Don't forget to delete [] a; when you're done!


/* Option to store the grid in memory and save processing power */
void generate_grid(int x_axis, int y_axis) {
    int x_count = 1;
    int y_count = 1;
    
    for (int i = 0; i < (x_axis * y_axis); i++) {
        //cout << "Er her!";
        //cout << "X:" << x_count << " Y:"<< y_count << endl;
        //grid[i] = x_count + ":" + y_count;
        string x = to_string(x_count);
        string y = to_string(y_count);
        grid[i] = x + ":" + y;
        //cout << grid[i] << endl;
        
        if(x_count == (x_axis)){
            x_count = 1;
            y_count++;
        }
        else {
            x_count++;
        }
    }
    
    return;
}
  
int main() { 

    generate_grid(x_axis, y_axis);
    for (int i = 0; i < coordinates; i++) {
        cout << "Sensor Data: " << i+1 << "   Becomes GRID Position   " << grid[i] << endl;      

    }
}