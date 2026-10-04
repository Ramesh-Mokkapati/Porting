#include "stdafx.h"
#include "FlexParser.h"
#include "MiniC.h"

FlexParser::FlexParser()
{

}

FlexParser::FlexParser(CString sRootDirectory, bool bOverWriteFiles)
    : m_sRootDirectory(sRootDirectory)
    , m_bOverWriteFiles(bOverWriteFiles)
{

}

FlexParser::~FlexParser()
{
    m_oFileFinder.Close();
}

void FlexParser::SetRootDirectory(CString sRootDirectory)
{
    m_sRootDirectory = sRootDirectory;
}

CString FlexParser::GetRootDirectory()
{
    return m_sRootDirectory;
}

void FlexParser::SetOverWriteFiles(bool bOverWriteFiles)
{
    m_bOverWriteFiles = bOverWriteFiles;
}

bool FlexParser::GetOverWriteFiles()
{
    return m_bOverWriteFiles;
}

void FlexParser::ProcessFilesInDirectory(CString sCurrentFolder)
{
    FindAllFiles(sCurrentFolder.GetBuffer(0));
}

void FlexParser::SanitizeOutputFile()
{
    CStdioFile oFileReader, oFileWriter;
    oFileReader.Open(m_sCurrentProcessedTargetFileName, CFile::modeRead);
    CStringArray oFileReaderContent;
    CString str_line;
    bool bIsFileModified = false;
    while (oFileReader.ReadString(str_line))
    {
        if (str_line == _T("static TCHAR THIS_FILE[] = __FILE__;"))
        {
            str_line = _T("static char THIS_FILE[] = __FILE__;");
            bIsFileModified = true;
        }

        oFileReaderContent.Add(str_line);
    }

    oFileReader.Close();

    if (bIsFileModified)
    {
        // modeCreate truncates the existing file; no Remove beforehand so the
        // target is never left in a deleted state if Open fails.
        if (!oFileWriter.Open(m_sCurrentProcessedTargetFileName, CFile::modeCreate | CFile::modeWrite))
        {
            TRACE(_T("SanitizeOutputFile: could not open %s for writing\n"), m_sCurrentProcessedTargetFileName);
            return;
        }

        for (int i = 0; i < oFileReaderContent.GetCount(); i++)
        {
            str_line = oFileReaderContent.GetAt(i);
            oFileWriter.WriteString(str_line + _T('\n'));
        }

        oFileWriter.Close();
    }
}
void FlexParser::ProcessCurrentFile()
{
    if (IsCurrentFilePartOfPatterns(GetFileExtension(m_sCurrentProcessedSourceFileName)))
    {
        m_sCurrentProcessedTargetFileName = m_sCurrentProcessedSourceFileName + _T(".u");

        flexmain(m_sCurrentProcessedSourceFileName.GetBuffer(0), m_sCurrentProcessedTargetFileName.GetBuffer(0));
        SanitizeOutputFile();
        m_nCurrentProcessedTargetFileSize = GetCurrentProcessedTargetFileSize(m_sCurrentProcessedTargetFileName);

        if (m_nCurrentProcessedTargetFileSize < 0)
        {
            // Target file missing or unreadable; do not touch the source file.
            TRACE(_T("ProcessCurrentFile: target file unreadable, skipping %s\n"), m_sCurrentProcessedSourceFileName);
            return;
        }

        if (IsSourceAndTargetFilesSame())
        {
            try
            {
                CFile::Remove(m_sCurrentProcessedTargetFileName);
            }
            catch (CFileException* pEx)
            {
                TRACE(_T("File %s cannot be removed\n"), m_sCurrentProcessedTargetFileName);
                pEx->Delete();
            }
        }
        else
        {
            if (m_bOverWriteFiles)
            {
                try
                {
                    CFile::Remove(m_sCurrentProcessedSourceFileName);
                    CFile::Rename(m_sCurrentProcessedTargetFileName, m_sCurrentProcessedSourceFileName);
                }
                catch (CFileException* pEx)
                {
                    TRACE(_T("File %s not found, cause = %d\n"), m_sCurrentProcessedSourceFileName, pEx->m_cause);
                    pEx->Delete();
                }
            }
        }
    }
}

CString FlexParser::GetFileExtension(CString sFileName)
{
    CString tmpStr = PathFindExtensionA(sFileName);
    return tmpStr;
}

bool FlexParser::IsCurrentFilePartOfPatterns(CString sFileExtention)
{
    for (int i = 0; i < m_oFilePatterns.GetCount(); i++)
    {
        CString tmpStr = m_oFilePatterns.ElementAt(i);
        if (sFileExtention.Compare(m_oFilePatterns.ElementAt(i)) == 0)
        {
            return true;
        }
    }

    return false;
}

bool FlexParser::IsSourceAndTargetFilesSame()
{
    bool bSouceAndTargetFilesSame = false;
    if (m_nCurrentProcessedSourceFileSize == m_nCurrentProcessedTargetFileSize)
    {
        bSouceAndTargetFilesSame = true;
    }

    return bSouceAndTargetFilesSame;
}

__int64 FlexParser::GetCurrentProcessedTargetFileSize(LPCTSTR name)
{
    HANDLE hFile = CreateFile(name, GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return -1; // error condition, could call GetLastError to find out more
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(hFile, &size))
    {
        CloseHandle(hFile);
        return -1; // error condition, could call GetLastError to find out more
    }

    CloseHandle(hFile);
    return size.QuadPart;
}

void FlexParser::SetFilePatterns(CStringArray& oFilePatterns)
{
    m_oFilePatterns.Copy(oFilePatterns);
}

CStringArray& FlexParser::GetFilePatterns()
{
    return m_oFilePatterns;
}

void FlexParser::FindAllFiles(std::string sCurrentFolder)
{
    if (sCurrentFolder.length() == 0)
    {
        sCurrentFolder = m_sRootDirectory.GetBuffer(0);
    }

    WIN32_FIND_DATA FileData;
    std::string folderNameWithSt = sCurrentFolder + _T("*");
    HANDLE FirstFile = FindFirstFile(folderNameWithSt.c_str(), &FileData);

    if (FirstFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (_tcscmp(FileData.cFileName, _T(".")) != 0 && _tcscmp(FileData.cFileName, _T("..")) != 0)
            {
                if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    std::string NewPath = sCurrentFolder + FileData.cFileName;
                    NewPath = NewPath + _T("\\");

                    FindAllFiles(NewPath);
                }
                else
                {
                    m_sCurrentProcessedSourceFileName = CString(sCurrentFolder.c_str()) + FileData.cFileName;
                    LARGE_INTEGER sz;
                    __int64 FileSize = 0;
                    sz.LowPart = FileData.nFileSizeLow;
                    sz.HighPart = FileData.nFileSizeHigh;
                    FileSize = sz.QuadPart;
                    m_nCurrentProcessedSourceFileSize = FileSize;
                    ProcessCurrentFile();
                }
            }
        } while (FindNextFile(FirstFile, &FileData));

        FindClose(FirstFile);
    }
}
