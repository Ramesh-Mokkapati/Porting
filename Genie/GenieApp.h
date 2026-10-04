// TestLex.h : main header file for the TESTLEX application
//

#if !defined(AFX_TESTLEX_H__AF830842_0E20_4328_BF2C_28808FBE5805__INCLUDED_)
#define AFX_TESTLEX_H__AF830842_0E20_4328_BF2C_28808FBE5805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestLexApp:
// See TestLex.cpp for the implementation of this class
//

class CGenieApp : public CWinApp
{
public:
	CGenieApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenieApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGenieApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTLEX_H__AF830842_0E20_4328_BF2C_28808FBE5805__INCLUDED_)
