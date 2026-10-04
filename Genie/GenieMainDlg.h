// TestLexDlg.h : header file
//

#include "afxwin.h"
#if !defined(AFX_TESTLEXDLG_H__170493D0_9B1E_4075_B456_D21789FEC434__INCLUDED_)
#define AFX_TESTLEXDLG_H__170493D0_9B1E_4075_B456_D21789FEC434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestLexDlg dialog

class CGenieMainDlg : public CDialog
{
// Construction
public:
	CGenieMainDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGenieMainDlg)
	enum { IDD = IDD_TESTLEX_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenieMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGenieMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelectFolder();
	CString m_sFolderName;
	CString m_sPatterns;
	CComboBox m_cPatterns;
	afx_msg void OnBnClickedOverwrite();
	afx_msg void OnBnClickedSaveBackup();
private:
	bool m_bOverWriteFiles;
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTLEXDLG_H__170493D0_9B1E_4075_B456_D21789FEC434__INCLUDED_)
