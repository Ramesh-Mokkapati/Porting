#include "afxwin.h"
#include "afxcmn.h"

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CPortAssistDlg dialog

class CPortAssistDlg : public CDialog
{
public:
    CPortAssistDlg(CWnd* pParent = NULL);	// standard constructor
    enum { IDD = IDD_PORTASSIST_DIALOG };
    afx_msg void OnBnClickedSelectFolder();
    afx_msg void OnBnClickedOk();
    afx_msg void OnTcnSelchangePortingTab(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedOverwrite();
    afx_msg void OnBnClickedSaveBackup();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

private:
    HICON       m_hIcon;
    bool        m_bOverWriteFiles;
    bool        m_bUseX64Porting;
    CString     m_sFolderName;
    CString     m_sPatterns;
    CComboBox   m_cPatterns;
    CTabCtrl    m_cPortingTab;
};
