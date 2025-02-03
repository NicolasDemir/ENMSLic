#pragma once
#include "afxdialogex.h"


// CLicViewDlg dialog

class CLicViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLicViewDlg)

public:
	CLicViewDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLicViewDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LICVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_S1;
	CString m_T1;
	CString m_S2;
	CString m_E4;
	CString m_OZ;
	CString m_F1;

	/*CString m_MAC;
	CString m_T1;
	CString m_S2;
	CString m_E4;
	CString m_OZ;
	CString m_F1;
*/
};
