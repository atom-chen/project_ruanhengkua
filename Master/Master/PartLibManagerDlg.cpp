// PartLibManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "PartLibManagerDlg.h"
#include "afxdialogex.h"


// CPartLibManagerDlg 对话框

IMPLEMENT_DYNAMIC(CPartLibManagerDlg, CDialog)

CPartLibManagerDlg::CPartLibManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartLibManagerDlg::IDD, pParent)
    , m_strLibName(_T(""))
{

}

CPartLibManagerDlg::~CPartLibManagerDlg()
{
}

void CPartLibManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_NONSELTREE, m_treeNonSel);
    DDX_Control(pDX, IDC_SELTREE, m_treeSel);
    DDX_Control(pDX, IDC_EDITLIBNAME, m_edtLibName);
    DDX_Text(pDX, IDC_EDITLIBNAME, m_strLibName);
}


BEGIN_MESSAGE_MAP(CPartLibManagerDlg, CDialog)
    ON_BN_CLICKED(IDB_SINGLESEL, &CPartLibManagerDlg::OnBnClickedSinglesel)
    ON_BN_CLICKED(IDB_SINGLEUNSEL, &CPartLibManagerDlg::OnBnClickedSingleunsel)
    ON_BN_CLICKED(IDOK, &CPartLibManagerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPartLibManagerDlg 消息处理程序


void CPartLibManagerDlg::OnBnClickedSinglesel()
{
    // TODO: 在此添加控件通知处理程序代码
}


void CPartLibManagerDlg::OnBnClickedSingleunsel()
{
    // TODO: 在此添加控件通知处理程序代码
}

void CPartLibManagerDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    CDialog::OnOK();
}
