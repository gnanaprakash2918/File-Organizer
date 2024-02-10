#include <iostream>
#include <string>
#include <filesystem>

/*
TODO
    * handle already existing files
    * handle errors for already existing directories and folder
    * handle permission error
*/

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    std::string CurrFileName = __FILE__;
    // Get Current Working Directory as string
    std::string Currentpath = fs::current_path().generic_string();

    // Iterate over the filenames in the path
    for (const auto &entry : fs::directory_iterator(Currentpath))
    {
        // Get File Path
        fs::path p(entry.path());

        // Check if current path is director or Current excutable source
        // if so skip it

        if (fs::is_directory(p))
            continue;

        // extract filename with its extension as string
        std::string filenameWithext(p.filename().generic_string());

        // Extract the File path
        std::string FilePath = Currentpath + "/" + p.filename().generic_string();

        // Extract the File Extension
        int findExtInd = filenameWithext.find_last_of('.');
        std::string ext = "";
        if (findExtInd != std::string::npos)
            ext = filenameWithext.substr(findExtInd + 1);

        // Name the folder based on the extension
        std::string folderName = ext;

        // If there is no extension (its not folder as we skipped it)
        // Creater Other files as its name
        if (folderName.empty())
            folderName = "Other";
        folderName.append(" Files");

        // Finalise the folderpath to be created
        std::string folderPath = Currentpath + "/" + folderName;

        // Check if it exists
        // if it doesnt exist create one
        if (!fs::exists(folderPath))
            fs::create_directory(folderName);

        // source file location
        fs::path sourceFile = FilePath;
        // source destination directory path
        fs::path destDir = folderPath + "/";

        try
        {
            // copy file
            fs::copy(sourceFile, destDir / sourceFile.filename());
            // delete the file
            fs::remove(sourceFile);
        }
        catch (const fs::filesystem_error &e)
        {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }
}