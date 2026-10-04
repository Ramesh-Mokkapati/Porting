#include "stdafx.h"
#include "FileUtils.h"
#include "FixAPI.h"
#include <algorithm>
#include <cctype>
#include <cwctype>
#include <mutex>
#include <stdio.h>

namespace FileUtils
{
    namespace
    {
        bool EndsWithIgnoreCase(const std::wstring& value, const std::wstring& ending)
        {
            if (value.length() < ending.length())
            {
                return false;
            }

            std::wstring valueEnding = value.substr(value.length() - ending.length());
            std::transform(valueEnding.begin(), valueEnding.end(), valueEnding.begin(),
                [](wchar_t ch) { return static_cast<wchar_t>(towlower(ch)); });

            std::wstring endingLower = ending;
            std::transform(endingLower.begin(), endingLower.end(), endingLower.begin(),
                [](wchar_t ch) { return static_cast<wchar_t>(towlower(ch)); });
            return valueEnding == endingLower;
        }

        bool IsIdentifierStart(char ch)
        {
            const unsigned char uch = static_cast<unsigned char>(ch);
            return std::isalpha(uch) != 0 || ch == '_';
        }

        bool IsIdentifierChar(char ch) {
            const unsigned char uch = static_cast<unsigned char>(ch);
            return std::isalnum(uch) != 0 || ch == '_';
        }

        bool ShouldReplaceIdentifier(const std::string& text, size_t startPos)
        {
            if (text.compare(startPos, 4, "char") != 0)
            {
                return true;
            }

            if (startPos >= 9 && text.compare(startPos - 9, 9, "unsigned ") == 0)
            {
                return false;
            }

            if (startPos >= 7 && text.compare(startPos - 7, 7, "signed ") == 0) {
                return false;
            }

            return true;
        }

        std::string ApplyApiReplacements(const std::string& source, bool& modified, TransformMode transformMode)
        {
            enum ParseMode
            {
                ParseNormal,
                ParseLineComment,
                ParseBlockComment,
                ParseStringLiteral,
                ParseCharLiteral
            };

            ParseMode parseMode = ParseNormal;
            std::string output;
            output.reserve(source.size());

            for (size_t i = 0; i < source.size();)
            {
                if (parseMode == ParseNormal)
                {
                    if (i + 1 < source.size() && source[i] == '/' && source[i + 1] == '/')
                    {
                        parseMode = ParseLineComment;
                        output.push_back(source[i++]);
                        output.push_back(source[i++]);
                        continue;
                    }

                    if (i + 1 < source.size() && source[i] == '/' && source[i + 1] == '*')
                    {
                        parseMode = ParseBlockComment;
                        output.push_back(source[i++]);
                        output.push_back(source[i++]);
                        continue;
                    }

                    if (source[i] == '"')
                    {
                        parseMode = ParseStringLiteral;
                        output.push_back(source[i++]);
                        continue;
                    }

                    if (source[i] == '\'')
                    {
                        parseMode = ParseCharLiteral;
                        output.push_back(source[i++]);
                        continue;
                    }

                    if (IsIdentifierStart(source[i]))
                    {
                        const size_t start = i;
                        ++i;
                        while (i < source.size() && IsIdentifierChar(source[i]))
                        {
                            ++i;
                        }

                        std::string token = source.substr(start, i - start);

                        int toCheck = 0;
                        const int apiMode = (transformMode == TransformModeX64) ? ApiTransformX64 : ApiTransformUnicode;
                        const char* replacement = fixBadApiForMode(token.c_str(), &toCheck, apiMode);

                        if (replacement != NULL && token != replacement && ShouldReplaceIdentifier(source, start))
                        {
                            output.append(replacement);
                            modified = true;
                        }
                        else
                        {
                            output.append(token);
                        }

                        continue;
                    }

                    output.push_back(source[i++]);
                    continue;
                }

                if (parseMode == ParseLineComment)
                {
                    output.push_back(source[i]);
                    if (source[i] == '\n')
                    {
                        parseMode = ParseNormal;
                    }

                    ++i;
                    continue;
                }

                if (parseMode == ParseBlockComment)
                {
                    output.push_back(source[i]);
                    if (i + 1 < source.size() && source[i] == '*' && source[i + 1] == '/')
                    {
                        output.push_back(source[i + 1]);
                        i += 2;
                        parseMode = ParseNormal;
                    }
                    else
                    {
                        ++i;
                    }

                    continue;
                }

                if (parseMode == ParseStringLiteral || parseMode == ParseCharLiteral)
                {
                    output.push_back(source[i]);
                    if (source[i] == '\\' && i + 1 < source.size())
                    {
                        output.push_back(source[i + 1]);
                        i += 2;
                        continue;
                    }

                    if ((parseMode == ParseStringLiteral && source[i] == '"') ||
                        (parseMode == ParseCharLiteral && source[i] == '\''))
                    {
                        parseMode = ParseNormal;
                    }

                    ++i;
                    continue;
                }
            }

            return output;
        }
    }

    void FindAllFiles(const std::wstring& directory, const std::wstring& searchPattern, std::vector<std::wstring>& foundFiles)
    {
        std::wstring searchPath = directory + L"\\" + searchPattern;
        WIN32_FIND_DATAW findData;

        HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            return;
        }

        do
        {
            std::wstring fileName = findData.cFileName;
            if (fileName == L"." || fileName == L"..")
            {
                continue;
            }

            std::wstring fullPath = directory + L"\\" + fileName;

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                FindAllFiles(fullPath, searchPattern, foundFiles);
            }
            else
            {
                foundFiles.push_back(fullPath);
            }
        } while (FindNextFileW(hFind, &findData) != 0);

        FindClose(hFind);
    }

    bool SanitizeFile(const std::wstring& filePath, bool createBackup, TransformMode mode, bool& modified)
    {
        std::string fileContents;
        modified = false;

        static std::once_flag onceFlag;
        std::call_once(onceFlag, []() { initApiTables(); });

        // 1. Read file into string buffer
        {
            FILE* inputFile = _wfopen(filePath.c_str(), L"rb");
            if (inputFile == NULL)
            {
                return false;
            }

            char buffer[4096];
            while (true)
            {
                const size_t bytesRead = fread(buffer, 1, sizeof(buffer), inputFile);
                if (bytesRead > 0)
                {
                    fileContents.append(buffer, bytesRead);
                }

                if (bytesRead < sizeof(buffer))
                {
                    break;
                }
            }

            fclose(inputFile);
        }

        // 2. Perform string replacements (replacing previous Flex logic & SanitizeOutputFile)
        bool localModified = false;

        auto replaceAll = [&](const std::string& from, const std::string& to)
        {
            size_t pos = 0;
            while ((pos = fileContents.find(from, pos)) != std::string::npos)
            {
                fileContents.replace(pos, from.length(), to);
                pos += to.length();
                localModified = true;
            }
        };

        // Replaces logic formerly handled by MiniC.l and SanitizeOutputFile
        if (mode == TransformModeUnicode)
        {
            replaceAll("static TCHAR THIS_FILE[] = __FILE__;", "static char THIS_FILE[] = __FILE__;");
        }

        fileContents = ApplyApiReplacements(fileContents, localModified, mode);

        // 3. Write back only if modified
        if (localModified)
        {
            if (createBackup)
            {
                const std::wstring backupPath = filePath + L".bak";
                if (!CopyFileW(filePath.c_str(), backupPath.c_str(), FALSE))
                {
                    return false;
                }
            }

            FILE* outputFile = _wfopen(filePath.c_str(), L"wb");
            if (outputFile == NULL)
            {
                return false;
            }

            const size_t bytesWritten = fwrite(fileContents.data(), 1, fileContents.size(), outputFile);
            fclose(outputFile);
            if (bytesWritten != fileContents.size())
            {
                return false;
            }
        }

        modified = localModified;
        return true;
    }

    void ProcessDirectory(
        const std::wstring& directory,
        const std::vector<std::wstring>& extensions,
        bool createBackup,
        TransformMode mode,
        size_t& modifiedCount,
        size_t& failedCount)
    {

        modifiedCount = 0;
        failedCount = 0;
        std::vector<std::wstring> allFiles;
        FindAllFiles(directory, L"*.*", allFiles);

        for (const auto& file : allFiles)
        {
            for (const auto& ext : extensions)
            {
                if (EndsWithIgnoreCase(file, ext))
                {
                    bool fileModified = false;
                    if (!SanitizeFile(file, createBackup, mode, fileModified))
                    {
                        ++failedCount;
                    }
                    else if (fileModified)
                    {
                        ++modifiedCount;
                    }

                    break;
                }
            }
        }
    }
}
