// NodeManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "PartTypeManagerDlg.h"
#include "afxdialogex.h"
#include "DB/PartTypeDBManager.h"


// CPartTypeManagerDlg 对话框

IMPLEMENT_DYNAMIC(CPartTypeManagerDlg, CDialog)

CPartTypeManagerDlg::CPartTypeManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartTypeManagerDlg::IDD, pParent)
{

}

CPartTypeManagerDlg::~CPartTypeManagerDlg()
{
}

void CPartTypeManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LISTPART, m_listNode);
}


BEGIN_MESSAGE_MAP(CPartTypeManagerDlg, CDialog)
    ON_BN_CLICKED(IDB_ADD, &CPartTypeManagerDlg::OnBnClickedAdd)
    ON_BN_CLICKED(IDB_EDIT, &CPartTypeManagerDlg::OnBnClickedEdit)
    ON_BN_CLICKED(IDB_DEL, &CPartTypeManagerDlg::OnBnClickedDel)
    ON_NOTIFY(NM_DBLCLK, IDC_LISTPART, &CPartTypeManagerDlg::OnNMDblclkListpart)
END_MESSAGE_MAP()


// CPartTypeManagerDlg 消息处理程序


void CPartTypeManagerDlg::OnBnClickedAdd()
{
    // TODO: 在此添加控件通知处理程序代码
    CPartTypeDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        UpdateList();
    }
}


void CPartTypeManagerDlg::OnBnClickedEdit()
{
    // TODO: 在此添加控件通知处理程序代码
    int nSel = m_listNode.GetSelectionMark();
    if (nSel < 0 || nSel >= m_listNode.GetItemCount())
    {
        AfxMessageBox(_T("请选中一个零件类型进行修改!"));
        return;
    }
    DWORD dwId = m_listNode.GetItemData(nSel);//节点ID
    CPartTypeDlg dlg;
    dlg.m_dwId = dwId;//设置节点ID
    if (dlg.DoModal() == IDOK)
    {
        UpdateList();
    }
}


void CPartTypeManagerDlg::OnBnClickedDel()
{
    // TODO: 在此添加控件通知处理程序代码
    int nSel = m_listNode.GetSelectionMark();
    if (nSel < 0 || nSel >= m_listNode.GetItemCount())
    {
        AfxMessageBox(_T("请选中一个零件类型进行删除!"));
        return;
    }
    DWORD dwId = m_listNode.GetItemData(nSel);//ID
    if (::MessageBox(NULL, _T("您确定要删除该零件类型吗"), _T("提示"), MB_YESNO) == IDYES)
    {
        CPartTypeDBManager db;
        db.Del(dwId);
        UpdateList();
    }
}

void CPartTypeManagerDlg::UpdateList()
{
    m_listNode.DeleteAllItems();

    vector<PartType> vecTypes;
    CPartTypeDBManager db;
    db.GetAllType(vecTypes);

    vector<PartType>::iterator it;

    int nIndex = 0;
    for (it = vecTypes.begin(); it != vecTypes.end(); it++)
    {
        m_listNode.InsertItem(nIndex, it->m_strName);
        m_listNode.SetItemData(nIndex, it->m_dwId);

        nIndex++;
    }
}

BOOL CPartTypeManagerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_listNode.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_ImageList.Create(1, 18, ILC_COLOR, 0, 1);
    m_listNode.SetImageList(&m_ImageList, LVSIL_SMALL);

    m_listNode.InsertColumn(0, _T("零件类型名称"));

    //listctrl头部标题
    HWND hWndHeader = m_listNode.GetDlgItem(0)->GetSafeHwnd();
    m_header.SubclassWindow(hWndHeader);    
    m_header.EnableAutoSize(TRUE);// enable auto sizing.
    m_header.ResizeColumnsToFit();

    UpdateList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CPartTypeManagerDlg::OnNMDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    if (pNMItemActivate->iItem == -1)
    {
        return;
    }
    OnBnClickedEdit();
    *pResult = 0;
}
