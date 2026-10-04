#pragma once
#include <string>

using namespace std;

class FlexParser
{
public:
    FlexParser();
    FlexParser(CString sRootDirectory, bool bOverWriteFiles);
    ~FlexParser();
    void SetRootDirectory(CString sRootDirectory);
    CString GetRootDirectory();
    void SetOverWriteFiles(bool bOverWriteFiles);
    bool GetOverWriteFiles();
    void ProcessFilesInDirectory(CString sCurrentFolder);
    void SetFilePatterns(CStringArray& oFilePatterns);
    CStringArray& GetFilePatterns();
    void FindAllFiles(std::string folderName);

private:
    __int64 GetCurrentProcessedTargetFileSize(LPCTSTR name);
    void ProcessCurrentFile();
    bool IsSourceAndTargetFilesSame();
    CString GetFileExtension(CString sFileName);
    bool IsCurrentFilePartOfPatterns(CString sFileExtention);
    void SanitizeOutputFile();
private:
    CString			m_sRootDirectory;
    bool			m_bOverWriteFiles;
    CFileFind		m_oFileFinder;
    CString			m_sCurrentProcessedSourceFileName;
    __int64			m_nCurrentProcessedSourceFileSize;
    CString			m_sCurrentProcessedTargetFileName;
    __int64			m_nCurrentProcessedTargetFileSize;
    CStringArray	m_oFilePatterns;
};

