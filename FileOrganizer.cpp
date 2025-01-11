#include <iostream>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <openssl/evp.h>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

std::string compute_md5(const std::string &file_path)
{
    // Open the file in binary mode
    std::ifstream file(file_path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + file_path);
    }

    // Read the file into a buffer
    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});

    // Initialize the EVP context for MD5
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx)
    {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    // Initialize MD5 digest
    if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1)
    {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize MD5 digest");
    }

    // Update the digest with file data
    if (EVP_DigestUpdate(ctx, buffer.data(), buffer.size()) != 1)
    {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to update MD5 digest");
    }

    // Finalize the digest and obtain the hash
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;
    if (EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1)
    {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize MD5 digest");
    }

    // Clean up
    EVP_MD_CTX_free(ctx);

    // Convert the hash to a hexadecimal string
    std::ostringstream ss;
    for (unsigned int i = 0; i < hash_len; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
}

int main()
{
    // Hash {md5_string -> {path, file}}
    std::unordered_map<std::string, std::pair<std::string, std::string>> hash_map;

    // Iterate over files in the current working directory
    for (const auto &entry : fs::directory_iterator("."))
    {
        // Check if it's a regular file
        if (entry.is_regular_file())
        {
            std::string file_path = entry.path().string();

            // Skip hidden files
            if (file_path[2] == '.')
            {
                std::cout << file_path << " is a hidden file! Skipping it.\n\n";
                continue;
            }

            try
            {
                std::string md5_hash = compute_md5(file_path);
                std::cout << "File: " << file_path << "\nMD5: " << md5_hash << "\n\n";

                // Open the file in binary mode
                std::ifstream file(file_path, std::ios::binary);
                if (!file)
                {
                    throw std::runtime_error("Failed to open file: " + file_path);
                }

                // Read the file into a buffer
                std::string buffer(std::istreambuf_iterator<char>(file), {});
                hash_map[md5_hash] = {file_path, buffer};
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error processing file " << file_path << ": " << e.what() << "\n";
            }
        }
    }

    for (auto &[key, value] : hash_map)
    {
        std::string file_path = value.first;

        // Extract the file extension
        int extension_dot_index = file_path.find_last_of('.');
        std::string file_extension = file_path.substr(extension_dot_index + 1);

        // Directory to store files with the same extension
        std::string dir_path = file_extension + " files";
        if (!fs::exists(dir_path))
        {
            std::cout << "Creating " << file_extension + " files folder" << std::endl;

            if (!fs::create_directories(dir_path))
            {
                throw std::runtime_error("Failed to create directory: " + dir_path);
            }
        }

        // Construct the full file path (directory + original file name)
        // Extract original file name
        std::string file_name = fs::path(file_path).filename().string();
        std::string new_file_path = dir_path + "/" + file_name;

        // Write the file content to the new path
        std::ofstream out(new_file_path, std::ios::binary);
        if (!out)
        {
            throw std::runtime_error("Failed to open file for writing: " + new_file_path);
        }
        out.write(value.second.c_str(), value.second.size());

        if (!out.good())
        {
            throw std::runtime_error("Failed to write to file: " + new_file_path);
        }

        std::cout << "File Written Successfully: " << new_file_path << std::endl;
    }

    return 0;
}
