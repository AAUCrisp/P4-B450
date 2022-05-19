

void logData1(int *arr) {
    File.open("log.txt", std::ofstream::out | std::ofstream::app);
    File << "\n\n movement on the x-axi:" << arr[0] << "\n movement on the y-axis:" << arr[1];
    File.close();
}