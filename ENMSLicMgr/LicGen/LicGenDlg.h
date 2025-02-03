
// LicGenDlg.h : header file
//

#pragma once
enum cypherType;

// CLicGenDlg dialog
class CLicGenDlg : public CDialogEx
{
// Construction
public:
	CLicGenDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LICGEN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DWORD GetModuleLongFileName(HMODULE hModule, LPTSTR pszFilename, DWORD dwSize);
	CString GetProcessDirectory();
	CString GetWorkingDirectory();
	BOOL WriteSignatureToFile(CString szfile, CSoftKeyHelper& akey);
	CString GetUserSelectedFolder();
	CString GetEncryptedString(const CString& incput, BOOL bEncrypt = TRUE);
	CString DecryptString(const CString& incput, cypherType nType);

	void OpenFileLocation(const CString& szFilePath);

	CSoftKeyHelper m_akey;
	CSoftLicenseMgr m_softLicMgr;


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonCreateSignature();
	afx_msg void OnBnClickedButtonEncrypt();
	afx_msg void OnBnClickedButtonDecrypt();
	CString m_serial;
	int m_users;
	int m_meters;
	int m_connections;
	CString m_signature;
	CString	m_machine;
	CString	m_domain;
	CString	m_m_encodedDomain;
	CString m_cpu;

	afx_msg void OnBnClickedButtonEncrypt2();
	afx_msg void OnBnClickedButtonCheck();
	afx_msg void OnBnClickedButtonImportSignature();
	afx_msg void OnBnClickedButtonImportSignature2();
};
