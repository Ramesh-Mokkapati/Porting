#include "stdafx.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

/**
 * @namespace FileUtils
 * @brief Provides decoupled, standalone file and directory operations.
 */
namespace FileUtils
{
    /**
     * @brief Recursively searches a directory for files matching a specific pattern.
     *
     * @param directory The absolute or relative path to the target directory.
     * @param searchPattern The file pattern to match (e.g., L"*.*").
     * @param foundFiles A vector populated with the absolute paths of all discovered files.
     */
    void FindAllFiles(const std::wstring& directory, const std::wstring& searchPattern, std::vector<std::wstring>& foundFiles)
    {
        std::wstring searchPath = directory + L"\\" + searchPattern;
        WIN32_FIND_DATAW findData;

        // Initiate the directory search using the wide-character Windows API
        HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

        if (hFind == INVALID_HANDLE_VALUE)
        {
            return; // Directory inaccessible or empty
        }

        do
        {
            std::wstring fileName = findData.cFileName;

            // Ignore current and parent directory navigational markers
            if (fileName == L"." || fileName == L"..")
            {
                continue;
            }

            std::wstring fullPath = directory + L"\\" + fileName;

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // Recursively traverse subdirectories
                FindAllFiles(fullPath, searchPattern, foundFiles);
            }
            else
            {
                foundFiles.push_back(fullPath);
            }
        } while (FindNextFileW(hFind, &findData) != 0);

        // CRITICAL FIX: Ensure the search handle is explicitly closed to prevent resource leaks
        FindClose(hFind);
    }

    /**
     * @brief Reads a file, performs necessary sanitization (e.g., keyword replacements),
     *        and overwrites the file safely.
     *
     * @param filePath The full path to the target file.
     * @return true if the sanitization was successful, false otherwise.
     */
    bool SanitizeFile(const std::wstring& filePath)
    {
        std::string fileContents;

        // Step 1: Safely read the entire file into memory
        {
            std::ifstream inputFile(filePath, std::ios::in | std::ios::binary);
            if (!inputFile.is_open())
            {
                std::wcerr << L"Error: Could not open file for reading: " << filePath << L"\n";
                return false; // Safely abort without throwing unhandled exceptions
            }

            std::ostringstream contentsStream;
            contentsStream << inputFile.rdbuf();
            fileContents = contentsStream.str();
        } // inputFile goes out of scope and is automatically closed here

        // Step 2: Perform string replacements (Redundant replacements removed)
        // Example: Replace deprecated API calls or fix formatting
        const std::string searchStr = "OLD_API_CALL";
        const std::string replaceStr = "NEW_API_CALL";
        size_t pos = 0;

        while ((pos = fileContents.find(searchStr, pos)) != std::string::npos)
        {
            fileContents.replace(pos, searchStr.length(), replaceStr);
            pos += replaceStr.length();
        }

        // Step 3: Write the sanitized content back to the file
        {
            // std::ios::trunc ensures the file is cleared before writing
            std::ofstream outputFile(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
            if (!outputFile.is_open())
            {
                std::wcerr << L"Error: Could not open file for writing (may be locked): " << filePath << L"\n";
                return false;
            }

            outputFile << fileContents;
        } // outputFile goes out of scope and is safely closed here

        return true;
    }

} // namespace FileUtils