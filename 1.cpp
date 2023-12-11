#include <iostream>
#include <cstdio>

int main() {
    // Your C++ code to process data

    // Example data
    const char* data = "Info1 123\nInfo2 456\nInfo3 789\n";

    // Write the data to a temporary file (or you can use pipes)
    FILE* file = fopen("tempfile.txt", "w");
    fprintf(file, "%s", data);
    fclose(file);

    // Execute the awk command to extract the return byte number
    FILE* pipe = popen("awk '{print $2}' tempfile.txt", "r");
    if (!pipe) {
        std::cerr << "Error executing command" << std::endl;
        return 1;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        // Process each line of the output (return byte number)
        int returnByteNumber;
        sscanf(buffer, "%d", &returnByteNumber);

        // Your code to use returnByteNumber as needed
        std::cout << "Return Byte Number: " << returnByteNumber << std::endl;
    }

    // Close the pipe
    pclose(pipe);

    // Remove the temporary file
    remove("tempfile.txt");

    return 0;
}
