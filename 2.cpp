#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>

int main() {
    // Define an unordered_map to store filenames and the number of visits
    std::unordered_map<std::string, int> fileVisits;

    // Open the log file
    std::ifstream logFile("your_log_file.txt");

    if (logFile.is_open()) {
        std::string line;

        // Read each line from the log file
        while (std::getline(logFile, line)) {
            // Use stringstream to parse the line
            std::istringstream iss(line);
            
            // Extract the filename from the line
            std::string requestType, filename;
            iss >> requestType >> filename;

            // Increment the visit count for the filename in the unordered_map
            fileVisits[filename]++;
        }

        // Close the log file
        logFile.close();

        // Print the results
        std::cout << "Filename\tNumber of Visits\n";
        for (const auto& entry : fileVisits) {
            std::cout << entry.first << "\t\t" << entry.second << "\n";
        }
    } else {
        std::cerr << "Error opening the log file.\n";
    }

    return 0;
}
