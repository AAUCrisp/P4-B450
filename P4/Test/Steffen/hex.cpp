#include <iostream>
#include <iomanip>
using namespace std;

/* --- Conversion from Integer to Hex-chars --- */
string int_to_hex(int data, int back = 0) {
    stringstream stream;    // Create hex-conversion object

    stream << hex << data;    // Tell it to convert from int to hex-chars
    string result( stream.str() );      // Do the conversion
    cout << "Converted to Hex-chars is: " << result << endl;     // Print the result


    /* -- Coversion back in same object, which already has the value -- */
    if(back == 1) {
        int x;
        stream >> x;        // Write the coverted integer to the integer variable
        cout << "Converted back is: " << x << endl;
    }
    return result;
}

/* --- Conversion from Hex-chars to Integer --- */
int hex_to_int(string data) {
    stringstream stream;    // Create hex-conversion object
    int result;      // Create a variable for the converted integer
    stream << hex << data;    // Take the received hex-chars and covert to an integer
    stream >> result;        // Write the coverted integer to the integer variable
    cout << "Converted back is: " << result << endl;
    return result;
}


int main()
{
    int data = 75;      // Test data variable
    cout << "Inserted data is: " << data << endl;       // Print out the integer being converted
    
    string data_hex = int_to_hex(data);
    int data_int = hex_to_int(data_hex);

   return 0;
}