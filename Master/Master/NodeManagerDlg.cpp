// NodeManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Master.h"
#include "NodeManagerDlg.h"
#include "afxdialogex.h"
#include "NodeModifyDlg.h"
#include "DB/NodeDBManager.h"
#include "DB/PartDBManager.h"
#include "GlobalFun.h"


// CNodeManagerDlg �Ի���

IMPLEMENT_DYNAMIC(CNodeManagerDlg, CDialog)

CNodeManagerDlg::CNodeManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNodeManagerDlg::IDD, pParent)
{

}

CNodeManagerDlg::~CNodeManagerDlg()
{
}

void CNodeManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LISTPART, m_listNode);
}


BEGIN_MESSAGE_MAP(CNodeManagerDlg, CDialog)
    ON_BN_CLICKED(IDB_ADD, &CNodeManagerDlg::OnBnClickedAdd)
    ON_BN_CLICKED(IDB_EDIT, &CNodeManagerDlg::OnBnClickedEdit)
    ON_BN_CLICKED(IDB_DEL, &CNodeManagerDlg::OnBnClickedDel)
    ON_NOTIFY(NM_DBLCLK, IDC_LISTPART, &CNodeManagerDlg::OnNMDblclkListpart)
END_MESSAGE_MAP()


// CNodeManagerDlg ��Ϣ�������


void CNodeManagerDlg::OnBnClickedAdd()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
	CNodeModifyDlg dlg;
	dlg.SetNodeId(-1);
    if (dlg.DoModal() == IDOK)
    {
        UpdateNodeList();
    }
}


void CNodeManagerDlg::OnBnClickedEdit()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int nSel = m_listNode.GetSelectionMark();
    if (nSel < 0 || nSel >= m_listNode.GetItemCount())
    {
        AfxMessageBox(_T("��ѡ��һ���ڵ�����޸�!"));
        return;
    }
    DWORD dwId = m_listNode.GetItemData(nSel);//�ڵ�ID
    CNodeModifyDlg dlg;
    dlg.SetNodeId(dwId);	//���ýڵ�ID
    if (dlg.DoModal() == IDOK)
    {
        UpdateNodeList();
    }
}


void CNodeManagerDlg::OnBnClickedDel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int nSel = m_listNode.GetSelectionMark();
    if (nSel < 0 || nSel >= m_listNode.GetItemCount())
    {
        AfxMessageBox(_T("��ѡ��һ���ڵ����ɾ��!"));
        return;
    }
    DWORD dwId = m_listNode.GetItemData(nSel);//ID
    if (::MessageBox(NULL, _T("��ȷ��Ҫɾ���ýڵ���"), _T("��ʾ"), MB_YESNO) == IDYES)
    {
        CNodeDBManager db;
        db.Del(dwId);
        UpdateNodeList();
    }
}

void CNodeManagerDlg::UpdateNodeList()
{
    m_listNode.DeleteAllItems();

    CPartDBManager partDb;
    PartParam thePart;

    vector<SoftCrossNode> vecNodes;
    CNodeDBManager db;
    db.GetAllNodes(vecNodes);
    vector<SoftCrossNode>::iterator itNode;
    int nIndex = 0;
    for (itNode = vecNodes.begin(); itNode != vecNodes.end(); itNode++)
    {
        m_listNode.InsertItem(nIndex, itNode->m_strName.c_str()); 
        m_listNode.SetItemText(nIndex, 1, itNode->m_strWeight.c_str());

        CString strPartList = itNode->m_strPartId.c_str();
        vector<CString> vecParts;
        vector<CString>::iterator itParts;
        CGlobalFun::GetStringVectorByToken(strPartList, vecParts, _T('|'));
        for (itParts = vecParts.begin(); itParts != vecParts.end(); itParts++)
        {
            DWORD dwPartId = atoi(*itParts);
            partDb.GetPartInfoById(thePart, dwPartId);
            *itParts = thePart.m_strName.c_str();
        }
        CGlobalFun::FormatStringByToken(strPartList, vecParts, _T('|'));
        m_listNode.SetItemText(nIndex, 2, strPartList);

        m_listNode.SetItemData(nIndex, itNode->m_dwId);

        nIndex++;
    }
    m_listNode.AutoSizeColumn();
}

BOOL CNodeManagerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_listNode.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ImageList.Create(1, 18, ILC_COLOR, 0, 1);
	m_listNode.SetImageList(&m_ImageList, LVSIL_SMALL);

    m_listNode.InsertColumn(0, _T("�ڵ�����"));
    m_listNode.InsertColumn(1, _T("�ڵ�����"));
    m_listNode.InsertColumn(2, _T("����б�"));

    //listctrlͷ������
    HWND hWndHeader = m_listNode.GetDlgItem(0)->GetSafeHwnd();
    m_header.SubclassWindow(hWndHeader);    
    m_header.EnableAutoSize(TRUE);// enable auto sizing.
    m_header.ResizeColumnsToFit();


    UpdateNodeList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CNodeManagerDlg::OnNMDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult)
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
