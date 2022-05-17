int main()
{
    int data = 75;      // Test data variable
    cout << "Inserted data is: " << data << endl;       // Print out the integer being converted
    stringstream stream;    // Create hex-conversion object

    stream << hex << data;    // Tell it to convert from int to hex-chars
    string result( stream.str() );      // Do the conversion
    cout << "Converted to Hex-chars is: " << result << endl;     // Print the result
    

    stringstream ss;        // Probably because I'm an idiot, but need a new version of the same object... if we don't, we don't get the right result back.
    int x;      // Create a variable for the converted integer
    ss << hex << result;    // Take the received hex-chars and covert to an integer
    //ss >> x;        // Write the coverted integer to the integer variable
    stream >> x;        // Write the coverted integer to the integer variable
    cout << "Converted back is: " << x << endl;
    
   
   return 0;
}