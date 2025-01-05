#include <iostream>
#include <filesystem>
#include <string>

int main() {
    try {
        std::filesystem::path current_working_directory = std::filesystem::current_path();
        std::cout << current_working_directory;
    }
     catch (const std::filesystem::filesystem_error &e) {
        std::cout << e.what();
     }
}