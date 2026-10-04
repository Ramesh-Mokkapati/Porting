#pragma once
#include <string>
#include <vector>

namespace FileUtils
{
    enum TransformMode
    {
        TransformModeUnicode = 0,
        TransformModeX64 = 1
    };

    // Find all matching files recursively
    void FindAllFiles(const std::wstring& directory, const std::wstring& searchPattern, std::vector<std::wstring>& foundFiles);

    // Perform text replacements directly on a file in-place
    bool SanitizeFile(const std::wstring& filePath, bool createBackup, TransformMode mode, bool& modified);

    // Process all files in a folder matching extensions
    void ProcessDirectory(
        const std::wstring& directory,
        const std::vector<std::wstring>& extensions,
        bool createBackup,
        TransformMode mode,
        size_t& modifiedCount,
        size_t& failedCount);
}
