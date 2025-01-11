#include <iostream>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <openssl/md5.h>
#include <string>

namespace fs = std::filesystem;

std::string compute_md5(std::string& file_path) {
    std::cout << file_path << std::endl;
    return "";
}

int main() {
    // Hash {md5_string -> file}
    std::unordered_map<std::string, std::string> hash_map;

    // Iterate over files in the current working directory
    for (const auto& entry : fs::directory_iterator(".")) {

        // Check if its a regular file (regular_file enum = 1)
        // Skips Directories
        if (entry.is_regular_file()) {
            std::string file_path = entry.path().string();

            // Checking the 2nd Index, as 
            if(file_path[2] == '.') {
                std::cout << file_path << " is a hidden file ! Skipping it";
            }

            std::string md5_hash = compute_md5(file_path);
        }
    }

    return 0;
}
