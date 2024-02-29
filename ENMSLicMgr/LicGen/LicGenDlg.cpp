
// LicGenDlg.cpp : implementation file
//

#include "pch.h"
#include "inifile.h"
#include "SoftKeyHelper.h"
#include "LicGen.h"
#include "LicGenDlg.h"
#include "afxdialogex.h"

#include "EncryptionMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <windows.h>

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <atlstr.h> 


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLicGenDlg dialog



CLicGenDlg::CLicGenDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LICGEN_DIALOG, pParent)
    , m_serial(_T("TEST01"))
    , m_users(100)
    , m_meters(150)
    , m_connections(2)
    , m_signature(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLicGenDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SERIAL, m_serial);
    DDX_Text(pDX, IDC_EDIT_USERS, m_users);
    DDX_Text(pDX, IDC_EDIT_METERS, m_meters);
    DDX_Text(pDX, IDC_EDIT_CONNEXIONS, m_connections);
    DDX_Text(pDX, IDC_EDIT_SIGNATURE, m_signature);
}

BEGIN_MESSAGE_MAP(CLicGenDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CLicGenDlg::OnBnClickedButtonCreate)
    ON_BN_CLICKED(IDC_BUTTON_CREATE_SIGNATURE, &CLicGenDlg::OnBnClickedButtonCreateSignature)
    ON_BN_CLICKED(IDC_BUTTON_ENCRYPT, &CLicGenDlg::OnBnClickedButtonEncrypt)
    ON_BN_CLICKED(IDC_BUTTON_DECRYPT, &CLicGenDlg::OnBnClickedButtonDecrypt)
    ON_BN_CLICKED(IDC_BUTTON_ENCRYPT2, &CLicGenDlg::OnBnClickedButtonEncrypt2)
END_MESSAGE_MAP()


// CLicGenDlg message handlers

BOOL CLicGenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    m_akey.RetrieveSystemInformation();

    CString szSig;
    szSig.Format(_T("fig1;1;%s"), m_akey.m_sMacAdvanced);
  
    m_signature = GetEncryptedString(szSig);

    UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLicGenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLicGenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLicGenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD CLicGenDlg::GetModuleLongFileName(HMODULE hModule, LPTSTR pszFilename, DWORD dwSize)
{
    DWORD cchBuffer = ::GetModuleFileName(hModule, pszFilename, dwSize);

    if (cchBuffer == 0)
        return 0;

    TCHAR szModuleLongFilename[_MAX_PATH];

    return GetLongPathName(pszFilename, szModuleLongFilename, _countof(szModuleLongFilename));
}

CString CLicGenDlg::GetProcessDirectory()
{
    TCHAR szAppPathName[2000];

    GetModuleLongFileName(NULL, szAppPathName, _countof(szAppPathName));

    CString strAppDir = szAppPathName;

    int n = strAppDir.ReverseFind('\\');

    return strAppDir.Left(n + 1);
}

CString CLicGenDlg::GetWorkingDirectory()
{
    CString szReturn;

    TCHAR my_documents[MAX_PATH];

    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_DEFAULT, my_documents);

    if (SUCCEEDED(result))
    {
        CString szPath = CString(my_documents);

        if (szPath.GetLength())
        {
            szReturn = szPath;
        }
    }

    if (szReturn.IsEmpty())
    {
        szReturn = GetProcessDirectory();
    }

    return szReturn;
}

BOOL CLicGenDlg::WriteSignatureToFile(CString szfile, CSoftKeyHelper& akey)
{
    try
    {
        CStdioFile afile(szfile, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyWrite);
        afile.WriteString(_T("[Signatures]\r\n"));
        afile.WriteString(_T("1=") + akey.GetHardSignature() + _T("\r\n"));
        afile.WriteString(_T("2=") + akey.GetSoftSignature() + _T("\r\n"));
        afile.WriteString(_T("3=") + akey.GetCRC() + _T("\r\n"));
        afile.WriteString(_T("4=") + akey.GetTargetName() + _T("\r\n"));
        afile.WriteString(_T("5=") + akey.GetDomain() + _T("\r\n"));
        afile.WriteString(_T("6=") + akey.GetHardSignatureAdvanced() + _T("\r\n"));

        afile.WriteString(_T("[GeneralInfo]\r\n"));
        afile.WriteString(_T("1=\r\n"));
        afile.WriteString(_T("2=\r\n"));
        afile.WriteString(_T("3=\r\n"));
        afile.WriteString(_T("4=\r\n"));
        afile.WriteString(_T("5=\r\n"));
        afile.WriteString(_T("6=\r\n"));

        afile.Flush();
        afile.Close();
        return TRUE;
    }
    catch (...)
    {
        return FALSE;
    }

}

CString CLicGenDlg::GetUserSelectedFolder()
{
    //This is needed for virtually everything in BrowseFolder.
    TCHAR path[MAX_PATH];
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Select destination folder for the signature file");
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    if (pidl != 0)
    {
        // get the name of the folder and put it in path
        SHGetPathFromIDList(pidl, path);

        //Set the current directory to path
        SetCurrentDirectory(path);

        // free memory used
        IMalloc* imalloc = 0;
        if (SUCCEEDED(SHGetMalloc(&imalloc)))
        {
            imalloc->Free(pidl);
            imalloc->Release();
        }
    }

    return path;
}


void CLicGenDlg::OnBnClickedButtonCreate()
{
    UpdateData(TRUE);

    CString szfile = GetWorkingDirectory() + _T("\\") + _T("license-'") + m_serial+ _T("'.dat");
  
    CStdioFile afile(szfile, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyWrite);
    afile.WriteString(_T("[General]\r\n"));
    afile.WriteString(_T("licenseNumber=") + m_serial + _T("\r\n"));
    afile.WriteString(_T("version=1\r\n"));

    CString szline;
    szline.Format(_T("meters=%d\r\n"), m_meters);
    afile.WriteString(szline);

    szline.Format(_T("users=%d\r\n"), m_users);
    afile.WriteString(szline);

    szline.Format(_T("connections=%d\r\n"), m_connections);
    afile.WriteString(szline);

    afile.WriteString(_T("product=1\r\n"));
    afile.WriteString(_T("update=1\r\n"));
    afile.WriteString(_T("key=\r\n"));
    
    CString szBlank;

    afile.WriteString(_T("[Features]\r\n"));

    // Generating a new GUID
    GUID guid;
    
    CString strGuid;

    // Converting GUID to string
    CoCreateGuid(&guid);

    // Converting GUID to CString
    strGuid.Format(_T("{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);


    szBlank = GetEncryptedString(strGuid);
    afile.WriteString(_T("S1=") + szBlank + _T("\r\n"));
  
    afile.WriteString(_T("T1=") + m_signature+ _T("\r\n"));
    
   
    szBlank = GetEncryptedString(m_akey.m_sMachine);
    afile.WriteString(_T("S2=") + szBlank + _T("\r\n"));
  
    szBlank.Format(_T("lic1;%s;%d;%d;%d;1;1"), m_serial, m_meters, m_users, m_connections);

    afile.WriteString(_T("F1=") + GetEncryptedString(szBlank) + _T("\r\n"));
    afile.WriteString(_T("key=\r\n"));
    
    afile.Flush();
    afile.Close();

    int n = szfile.ReverseFind(_T('\\'));
    CString	szFilePath = szfile.Left(n);

    int i = (int)::ShellExecute(m_hWnd, _T("explore"), szFilePath, NULL, NULL, SW_SHOWNORMAL);
}


void CLicGenDlg::OnBnClickedButtonCreateSignature()
{
    CSoftKeyHelper akey;
    akey.RetrieveSystemInformation();

    CString computername = akey.GetTargetName();
    computername = computername.MakeLower();

    int lentgh = computername.GetLength();
    for (int i = 0; i < lentgh; i++)
    {
        if (computername[i] > 127)
        {
            ::AfxMessageBox(_T("Software license only supports ascii characters in computer name [A..Z, a..z,-(hyphen),0..9]"), MB_OK | MB_ICONSTOP);
            return;
        }
    }

    CString szfile;

    if (computername.GetLength() == 0)
        szfile = GetWorkingDirectory() + _T("\\") + _T("Signature.dat");
    else
    {

        szfile = GetWorkingDirectory() + _T("\\") + _T("Signature'") + computername + _T("'.dat");
    }

    BOOL bOK = FALSE;

    bOK = WriteSignatureToFile(szfile, akey);

    if (bOK)
    {
        AfxMessageBox(_T("The following file must be sent to software supplier\r\n") + szfile);

        int n = szfile.ReverseFind(_T('\\'));
        CString	szFilePath = szfile.Left(n);

        int i = (int)::ShellExecute(m_hWnd, _T("explore"), szFilePath, NULL, NULL, SW_SHOWNORMAL);

        if (i <= 32)
            ::AfxMessageBox(_T("Unable to open Windows Explorer"));

        return;
    }

    ::AfxMessageBox(_T("Please select the destination folder to save the signature file"));

    CString szFolder = GetUserSelectedFolder();

    if (szFolder.GetLength())
    {
        bOK = FALSE;

        szfile = szFolder + _T("\\") + _T("Signature'") + computername + _T("'.dat");

        bOK = WriteSignatureToFile(szfile, akey);

        if (bOK)
        {
            AfxMessageBox(_T("The following file must be sent to software supplier\r\n") + szfile);

            int n = szfile.ReverseFind(_T('\\'));
            CString	szFilePath = szfile.Left(n);

            int i = (int)::ShellExecute(m_hWnd, _T("explore"), szFilePath, NULL, NULL, SW_SHOWNORMAL);

            if (i <= 32)
                ::AfxMessageBox(_T("Unable to open Windows Explorer"));

            return;
        }
    }
}


std::string CStringToString(const CString& cstr) 
{
    CT2CA pszConvertedAnsi(cstr);
    return std::string(pszConvertedAnsi);
}

CString StringToCString(const std::string& str)
{
    return CString(str.c_str());
}


void CLicGenDlg::OnBnClickedButtonEncrypt()
{
    UpdateData(TRUE);

    m_signature = GetEncryptedString(m_signature);

    UpdateData(FALSE);
}



CString CLicGenDlg::GetEncryptedString(const CString& incput, BOOL bEncrypt )
{
    string sz = CStringToString(incput);

    int key = 3;
    string salt = "Olaf";
    int randomLength = 5; // Length of random characters to append

    // Seed for random number generation
    srand(time(0));

    CustomCypher cipher(key, salt, randomLength);
    string szReturn;
    if(bEncrypt)
        szReturn = cipher.encrypt(sz);
    else
        szReturn = cipher.decrypt(sz);

    return StringToCString(szReturn);
}


void CLicGenDlg::OnBnClickedButtonDecrypt()
{
    UpdateData(TRUE);

    m_signature = GetEncryptedString(m_signature, FALSE);

    UpdateData(FALSE);
}


void CLicGenDlg::OnBnClickedButtonEncrypt2()
{
    string message = "@toto cuello HOUGANA 45!";
    int key = 3; // Key for Caesar Cipher rotation
    string salt = "mysalt"; // Salt
    int randomLength = 5; // Length of random characters to append

    // Seed for random number generation
    srand(time(0));

    CustomCypher cipher(key, salt, randomLength);

    // Encryption
    string encrypted = cipher.encrypt(message);
    cout << "Encrypted: " << encrypted << endl;

    // Decryption
    string decrypted = cipher.decrypt(encrypted);
    cout << "Decrypted: " << decrypted << endl;


}

