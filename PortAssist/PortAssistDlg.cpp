#include "stdafx.h"
#include "PortAssistApp.h"
#include "PortAssistDlg.h"
#include "FileUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
        //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPortAssistDlg::CPortAssistDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CPortAssistDlg::IDD, pParent)
    , m_sFolderName(_T(""))
    , m_sPatterns(_T(""))
    , m_bOverWriteFiles(true)
    , m_bUseX64Porting(false)
{
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPortAssistDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_FOLDER_NAME, m_sFolderName);
    DDX_CBString(pDX, IDC_FILE_PATTERNS, m_sPatterns);
    DDX_Control(pDX, IDC_FILE_PATTERNS, m_cPatterns);
    DDX_Control(pDX, IDC_PORTING_TAB, m_cPortingTab);
}

BEGIN_MESSAGE_MAP(CPortAssistDlg, CDialog)
    //{{AFX_MSG_MAP(CPortAssistDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_SELECT_FOLDER, &CPortAssistDlg::OnBnClickedSelectFolder)
    ON_BN_CLICKED(IDC_OVERWRITE, &CPortAssistDlg::OnBnClickedOverwrite)
    ON_BN_CLICKED(IDC_SAVE_BACKUP, &CPortAssistDlg::OnBnClickedSaveBackup)
    ON_BN_CLICKED(IDOK, &CPortAssistDlg::OnBnClickedOk)
    ON_NOTIFY(TCN_SELCHANGE, IDC_PORTING_TAB, &CPortAssistDlg::OnTcnSelchangePortingTab)
END_MESSAGE_MAP()

BOOL CPortAssistDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    int nLast = m_cPatterns.GetCount() - 1;
    if (nLast >= 0)
    {
        m_cPatterns.SetCurSel(nLast);
    }

    m_cPortingTab.InsertItem(0, _T("Unicode Porting"));
    m_cPortingTab.InsertItem(1, _T("x64 Porting"));
    m_cPortingTab.SetCurSel(0);
    m_bUseX64Porting = false;

    CheckRadioButton(IDC_SAVE_BACKUP, IDC_OVERWRITE, IDC_OVERWRITE);

    return TRUE;
}

void CPortAssistDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

void CPortAssistDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPortAssistDlg::OnQueryDragIcon()
{
    return (HCURSOR)m_hIcon;
}

void CPortAssistDlg::OnBnClickedSelectFolder()
{
    CFolderPickerDialog m_dlg;
    m_dlg.m_ofn.lpstrTitle = _T("Select the Folder containing C++ Code");
    m_dlg.m_ofn.lpstrInitialDir = _T("C:\\");
    if (m_dlg.DoModal() == IDOK)
    {
        m_sFolderName = m_dlg.GetPathName();
        m_sFolderName += _T("\\");
        UpdateData(FALSE);
    }
}


void CPortAssistDlg::OnBnClickedOverwrite()
{
    m_bOverWriteFiles = true;
}


void CPortAssistDlg::OnBnClickedSaveBackup()
{
    m_bOverWriteFiles = false;
}


void CPortAssistDlg::OnBnClickedOk()
{
    UpdateData(TRUE);

    if (m_sFolderName.IsEmpty())
    {
        AfxMessageBox(_T("Please select a folder before running the transformation."));
        return;
    }

    std::vector<std::wstring> extensions = { L".cpp", L".h", L".hpp", L".c" };
    size_t modifiedCount = 0;
    size_t failedCount = 0;
    const FileUtils::TransformMode transformMode = m_bUseX64Porting
        ? FileUtils::TransformModeX64
        : FileUtils::TransformModeUnicode;

    FileUtils::ProcessDirectory(
        std::wstring(CT2W(m_sFolderName)),
        extensions,
        !m_bOverWriteFiles,
        transformMode,
        modifiedCount,
        failedCount);

    CString resultMessage;
    resultMessage.Format(
        _T("%s completed.\nModified files: %u\nFailed files: %u"),
        m_bUseX64Porting ? _T("x64 porting") : _T("Unicode transformation"),
        static_cast<unsigned int>(modifiedCount),
        static_cast<unsigned int>(failedCount));
    AfxMessageBox(resultMessage);
}

void CPortAssistDlg::OnTcnSelchangePortingTab(NMHDR* pNMHDR, LRESULT* pResult)
{
    m_bUseX64Porting = (m_cPortingTab.GetCurSel() == 1);
    *pResult = 0;
}
