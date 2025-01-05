#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

int main() {
    try {
        // Get the Current Working Directory
        std::filesystem::path current_working_directory = std::filesystem::current_path();

        // Iterate over the current directory
        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(current_working_directory)) {
            if(entry.is_regular_file()) {
                // Regular File (No directories or shortcut links)
                std::cout << entry.path().string() << std::endl;

                // Open the file in read mode
                std::ifstream file(entry.path(), std::ios::binary);
                if(file) {
                    std::string file_content(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
                }
            }
        }
    }
     catch (const std::filesystem::filesystem_error &e) {
        std::cout << e.what();
     }
}