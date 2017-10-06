// NodeManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Master.h"
#include "PartTypeManagerDlg.h"
#include "afxdialogex.h"
#include "DB/PartTypeDBManager.h"


// CPartTypeManagerDlg �Ի���

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


// CPartTypeManagerDlg ��Ϣ�������


void CPartTypeManagerDlg::OnBnClickedAdd()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CPartTypeDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        UpdateList();
    }
}


void CPartTypeManagerDlg::OnBnClickedEdit()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int nSel = m_listNode.GetSelectionMark();
    if (nSel < 0 || nSel >= m_listNode.GetItemCount())
    {
        AfxMessageBox(_T("��ѡ��һ��������ͽ����޸�!"));
        return;
    }
    DWORD dwId = m_listNode.GetItemData(nSel);//�ڵ�ID
    CPartTypeDlg dlg;
    dlg.m_dwId = dwId;//���ýڵ�ID
    if (dlg.DoModal() == IDOK)
    {
        UpdateList();
    }
}


void CPartTypeManagerDlg::OnBnClickedDel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int nSel = m_listNode.GetSelectionMark();
    if (nSel < 0 || nSel >= m_listNode.GetItemCount())
    {
        AfxMessageBox(_T("��ѡ��һ��������ͽ���ɾ��!"));
        return;
    }
    DWORD dwId = m_listNode.GetItemData(nSel);//ID
    if (::MessageBox(NULL, _T("��ȷ��Ҫɾ�������������"), _T("��ʾ"), MB_YESNO) == IDYES)
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

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_listNode.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_ImageList.Create(1, 18, ILC_COLOR, 0, 1);
    m_listNode.SetImageList(&m_ImageList, LVSIL_SMALL);

    m_listNode.InsertColumn(0, _T("�����������"));

    //listctrlͷ������
    HWND hWndHeader = m_listNode.GetDlgItem(0)->GetSafeHwnd();
    m_header.SubclassWindow(hWndHeader);    
    m_header.EnableAutoSize(TRUE);// enable auto sizing.
    m_header.ResizeColumnsToFit();

    UpdateList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CPartTypeManagerDlg::OnNMDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (pNMItemActivate->iItem == -1)
    {
        return;
    }
    OnBnClickedEdit();
    *pResult = 0;
}
