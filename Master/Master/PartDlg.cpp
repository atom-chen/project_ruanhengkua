// PartDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Master.h"
#include "PartDlg.h"
#include "afxdialogex.h"
#include "DB/PartTypeManager.h"
#include "PartManagerDlg.h"
#include "DB/PartDBManager.h"
#include "PillarPartDlg.h"
#include "DB/PillarDBManager.h"
#include "DB/PartTypeManager.h"

// CPartDlg �Ի���

//���������Щ����

IMPLEMENT_DYNAMIC(CPartDlg, CDialog)

CPartDlg::CPartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartDlg::IDD, pParent)
    , m_nTypeSel(0)
{
}

CPartDlg::~CPartDlg()
{
}

void CPartDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LISTPART, m_listPart);
    DDX_Control(pDX, IDC_COMBOTYPE, m_comboTypeList);
    DDX_CBIndex(pDX, IDC_COMBOTYPE, m_nTypeSel);
}


BEGIN_MESSAGE_MAP(CPartDlg, CDialog)
    ON_BN_CLICKED(IDB_ADD, &CPartDlg::OnBnClickedAdd)
    ON_CBN_SELCHANGE(IDC_COMBOTYPE, &CPartDlg::OnCbnSelchangeCombotype)
    ON_BN_CLICKED(IDB_EDIT, &CPartDlg::OnBnClickedEdit)
    ON_BN_CLICKED(IDB_DEL, &CPartDlg::OnBnClickedDel)
    ON_NOTIFY(NM_DBLCLK, IDC_LISTPART, &CPartDlg::OnNMDblclkListpart)
    ON_BN_CLICKED(IDB_ADD2, &CPartDlg::OnBnClickedAdd2)
END_MESSAGE_MAP()


// CPartDlg ��Ϣ�������
void CPartDlg::ResetTypeList(DWORD dwTypeId)
{
    m_mapIdType.clear();
    m_comboTypeList.ResetContent();

    vector<PartType>& vecType = CPartTypeManager::m_vecPartType;
    vector<PartType>::iterator itType;
    int nIndex = 0;
    for (itType = vecType.begin(); itType != vecType.end(); itType++)
    {
        m_comboTypeList.AddString(itType->m_strName);
        m_comboTypeList.SetItemData(nIndex, itType->m_dwId);
        if (itType->m_dwId == dwTypeId)
        {
            m_comboTypeList.SetCurSel(nIndex);
        }
        nIndex++;

        m_mapIdType.insert(map<DWORD, DWORD>::value_type(itType->m_dwId, itType->m_dwFlag));
    }
}

BOOL CPartDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    ResetTypeList();
    m_nTypeSel = 0;

    m_listPart.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    m_ImageList.Create(1, 18, ILC_COLOR, 0, 1);
    m_listPart.SetImageList(&m_ImageList, LVSIL_SMALL);

    //listctrlͷ������
    HWND hWndHeader = m_listPart.GetDlgItem(0)->GetSafeHwnd();
    m_header.SubclassWindow(hWndHeader);
    // enable auto sizing.
    m_header.EnableAutoSize(TRUE);

    UpdateData(FALSE);
    OnCbnSelchangeCombotype();

    //listctrl��ʽ
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CPartDlg::OnBnClickedAdd()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    CString strType;
    m_comboTypeList.GetLBText(m_nTypeSel, strType);
    DWORD dwTypeId = m_comboTypeList.GetItemData(m_nTypeSel);//����ID
    DWORD dwRet = 0;
    if (1010 == dwTypeId)
    {
        //֧��
        CPillarPartDlg dlg;
        dwRet = dlg.DoModal();        
    }
    else
    {
        DWORD dwTypeFlag = GetTypeById(dwTypeId);//��־
        CPartManagerDlg dlg;
        dlg.SetTypeData(strType, dwTypeId, dwTypeFlag); 
        dwRet = dlg.DoModal();
    }
    if (IDOK == dwRet)
    {
        OnCbnSelchangeCombotype();
    }   
}

void CPartDlg::OnCbnSelchangeCombotype()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    m_listPart.DeleteAllItems();
    DWORD dwId = m_comboTypeList.GetItemData(m_nTypeSel);
    DWORD dwType = GetTypeById(dwId);
    if (-1 == dwType)
    {
        return;
    }
    int nCol = m_listPart.GetColumnCount();
    for (int i = 0; i < nCol; i++)
    {
        m_listPart.DeleteColumn(0);//ɾ����
    }

    int nIndex = 0;

    m_listPart.InsertColumn(0, _T("����"));
    if (dwType & PART_PILLAR)//֧��
    {
        CPillarDBManager partDb;
        vector<PillarParam> vecParts;
        vector<PillarParam>::iterator itPart;
        partDb.GetAllPillars(vecParts);

        m_listPart.InsertColumn(1, _T("֧������"));
        m_listPart.InsertColumn(2, _T("֧���ܳ�"));
        m_listPart.InsertColumn(3, _T("֧���ϵ�"));
        m_listPart.InsertColumn(4, _T("֧���µ�"));
        m_listPart.InsertColumn(5, _T("�������"));
        for (itPart = vecParts.begin(); itPart != vecParts.end(); itPart++)
        {
            m_listPart.InsertItem(nIndex, itPart->m_strName.c_str());
            if (itPart->m_dwType == 0)
            {
                m_listPart.SetItemText(nIndex, 1, _T("��������"));
            }
            else
            {
                m_listPart.SetItemText(nIndex, 1, _T("����"));
            }
            m_listPart.SetItemText(nIndex, 2, itPart->m_strLength.c_str());
            m_listPart.SetItemText(nIndex, 3, itPart->m_strUp.c_str());
            m_listPart.SetItemText(nIndex, 4, itPart->m_strBelow.c_str());
            m_listPart.SetItemText(nIndex, 5, itPart->m_strWidthOfUp.c_str());

            m_listPart.SetItemData(nIndex, itPart->m_dwId);
            nIndex++;
        }
    }
    else
    {
        CPartDBManager partDb;
        vector<PartParam> vecParts;
        vector<PartParam>::iterator itPart;
        partDb.GetPartsByTypeId(vecParts, dwId);

        int nLenCol = 1;//�ߴ��ڵڼ���
        if (dwType & PART_WEIGHT)
        {
            m_listPart.InsertColumn(1, _T("����"));
            nLenCol = 2;
        }
        if (dwType & PART_LENGTH)
        {
            m_listPart.InsertColumn(nLenCol, _T("�ߴ�"));
        }
        for (itPart = vecParts.begin(); itPart != vecParts.end(); itPart++)
        {
            m_listPart.InsertItem(nIndex, itPart->m_strName.c_str());
            if (dwType & PART_WEIGHT)
            {
                m_listPart.SetItemText(nIndex, 1, itPart->m_strWeight.c_str());
            }
            if (dwType & PART_LENGTH)
            {
                m_listPart.SetItemText(nIndex, nLenCol, itPart->m_strLength.c_str());
            }

            m_listPart.SetItemData(nIndex, itPart->m_dwId);
            nIndex++;
        }
    }
    m_header.ResizeColumnsToFit();
}

DWORD CPartDlg::GetTypeById(DWORD dwId)
{
    map<DWORD, DWORD>::iterator it = m_mapIdType.find(dwId);
    if (it != m_mapIdType.end())
    {
        return it->second;
    }
    return -1;
}

void CPartDlg::OnBnClickedEdit()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    int nSel = m_listPart.GetSelectionMark();
    if (nSel < 0 || nSel >= m_listPart.GetItemCount())
    {
        AfxMessageBox(_T("��ѡ��һ����������޸�!"));
        return;
    }
    DWORD dwId = m_listPart.GetItemData(nSel);//���ID

    CString strType;
    m_comboTypeList.GetLBText(m_nTypeSel, strType);
    DWORD dwTypeId = m_comboTypeList.GetItemData(m_nTypeSel);//����ID


    DWORD dwRet = 0;
    if (1010 == dwTypeId)//֧������
    {
        //֧��
        CPillarPartDlg dlg;
        dlg.m_dwPillarId = dwId;
        dwRet = dlg.DoModal();        
    }
    else
    {
        DWORD dwTypeFlag = GetTypeById(dwTypeId);//��־
        CPartManagerDlg dlg;
        dlg.m_dwId = dwId;
        dlg.SetTypeData(strType, dwTypeId, dwTypeFlag); 
        dwRet = dlg.DoModal();
    }
    if (IDOK == dwRet)
    {
        OnCbnSelchangeCombotype();
    }
}


void CPartDlg::OnBnClickedDel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    int nSel = m_listPart.GetSelectionMark();
    if (nSel < 0 || nSel >= m_listPart.GetItemCount())
    {
        AfxMessageBox(_T("��ѡ��һ���������ɾ��!"));
        return;
    }

    CString strType;
    m_comboTypeList.GetLBText(m_nTypeSel, strType);
    DWORD dwTypeId = m_comboTypeList.GetItemData(m_nTypeSel);//����ID

    DWORD dwId = m_listPart.GetItemData(nSel);//���ID
    if (::MessageBox(NULL, _T("��ȷ��Ҫɾ���������"), _T("��ʾ"), MB_YESNO) == IDYES)
    {
        if (1010 == dwTypeId)
        {
            CPillarDBManager db;
            db.Del(dwId);
        }
        else
        {
            CPartDBManager db;
            db.Del(dwId);
        }
        OnCbnSelchangeCombotype();
    }
}


void CPartDlg::OnNMDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    OnBnClickedEdit();
    *pResult = 0;
}


void CPartDlg::OnBnClickedAdd2()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CPartTypeManagerDlg dlg;
    dlg.DoModal();

    UpdateData(TRUE);
    DWORD dwTypeId = m_comboTypeList.GetItemData(m_nTypeSel);
    CPartTypeManager::InitPartType();
    ResetTypeList(dwTypeId);
    OnCbnSelchangeCombotype();
}
