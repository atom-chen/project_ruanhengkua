// PartLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "PartLibDlg.h"
#include "afxdialogex.h"


// CPartLibDlg 对话框

IMPLEMENT_DYNAMIC(CPartLibDlg, CXTPDialog)

CPartLibDlg::CPartLibDlg(CWnd* pParent /*=NULL*/)
	: CXTPDialog(CPartLibDlg::IDD, pParent)
{

}

CPartLibDlg::~CPartLibDlg()
{
}

void CPartLibDlg::DoDataExchange(CDataExchange* pDX)
{
    CXTPDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PARTTREE, m_treePart);
    DDX_Control(pDX, IDB_ADDLIB, m_btnAdd);
    DDX_Control(pDX, IDB_EDITLIB, m_btnEdit);
    DDX_Control(pDX, IDB_DELLIB, m_btnDel);
    DDX_Control(pDX, IDC_LISTPARTLIB, m_listPartLib);
}


BEGIN_MESSAGE_MAP(CPartLibDlg, CXTPDialog)
    ON_BN_CLICKED(IDB_ADDLIB, &CPartLibDlg::OnBnClickedAddlib)
    ON_BN_CLICKED(IDB_EDITLIB, &CPartLibDlg::OnBnClickedEditlib)
    ON_BN_CLICKED(IDB_DELLIB, &CPartLibDlg::OnBnClickedDellib)
    ON_LBN_SELCHANGE(IDC_LISTPARTLIB, &CPartLibDlg::OnSelchangeListPartLib)
    ON_LBN_DBLCLK(IDC_LISTPARTLIB, &CPartLibDlg::OnDblclkListPartLib)
    ON_NOTIFY(TVN_SELCHANGED, IDC_PARTTREE, &CPartLibDlg::OnTvnSelchangedParttree)
    ON_NOTIFY(NM_DBLCLK, IDC_PARTTREE, &CPartLibDlg::OnNMDblclkParttree)
END_MESSAGE_MAP()


// CPartLibDlg 消息处理程序




BOOL CPartLibDlg::OnInitDialog()
{
    CXTPDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_listPartLib.SetCheckStyle(BS_AUTOCHECKBOX);
    m_listPartLib.AddString("abc");
    m_listPartLib.AddString("def");
    m_listPartLib.AddString("hij");
    m_listPartLib.AddString("lmn");
    m_listPartLib.SetItemData(0, 11);
    m_listPartLib.SetItemData(1, 22);
    m_listPartLib.SetItemData(2, 31);
    m_listPartLib.SetItemData(3, 41);
    //m_listPartLib.SetCurSel(2);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

#include "PartLibManagerDlg.h"
void CPartLibDlg::OnBnClickedAddlib()
{
    // TODO: 在此添加控件通知处理程序代码
    CPartLibManagerDlg dlg;
    dlg.DoModal();
}

void CPartLibDlg::OnBnClickedEditlib()
{
    // TODO: 在此添加控件通知处理程序代码

    int nSel = m_listPartLib.GetCurSel();
    if (-1 != nSel)
    {
        int nData = m_listPartLib.GetItemData(nSel);
    }
    else
    {
        AfxMessageBox("选择一项零件库再修改");
    }
}


void CPartLibDlg::OnBnClickedDellib()
{
    // TODO: 在此添加控件通知处理程序代码
}


void CPartLibDlg::OnSelchangeListPartLib()
{
    // TODO: 在此添加控件通知处理程序代码
}


void CPartLibDlg::OnDblclkListPartLib()
{
    // TODO: 在此添加控件通知处理程序代码
}

//第一位重量，第二位尺寸，第三位支柱

void CPartLibDlg::OnTvnSelchangedParttree(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}


void CPartLibDlg::OnNMDblclkParttree(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}
