// LicViewDlg.cpp : implementation file
//

#include "pch.h"
#include "LicGen.h"
#include "afxdialogex.h"
#include "LicViewDlg.h"


// CLicViewDlg dialog

IMPLEMENT_DYNAMIC(CLicViewDlg, CDialogEx)

CLicViewDlg::CLicViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LICVIEW, pParent)
	, m_S1(_T(""))
	, m_T1(_T(""))
	, m_S2(_T(""))
	, m_E4(_T(""))
	, m_OZ(_T(""))
	, m_F1(_T(""))
{

}

CLicViewDlg::~CLicViewDlg()
{
}

void CLicViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_S1, m_S1);
	DDX_Text(pDX, IDC_EDIT_T1, m_T1);
	DDX_Text(pDX, IDC_EDIT_S2, m_S2);
	DDX_Text(pDX, IDC_EDIT_E4, m_E4);
	DDX_Text(pDX, IDC_EDIT_OZ, m_OZ);
	DDX_Text(pDX, IDC_EDIT_F1, m_F1);
}


BEGIN_MESSAGE_MAP(CLicViewDlg, CDialogEx)
END_MESSAGE_MAP()


// CLicViewDlg message handlers
