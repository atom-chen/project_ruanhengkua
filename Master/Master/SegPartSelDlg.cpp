// SegPartSelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Master.h"
#include "SegPartSelDlg.h"
#include "afxdialogex.h"
#include "DB/PartDBManager.h"
#include "DB/PillarDBManager.h"
#include "PartDlg.h"


// CSegPartSelDlg �Ի���

IMPLEMENT_DYNAMIC(CSegPartSelDlg, CXTPPropertyPage)

CSegPartSelDlg::CSegPartSelDlg(CWnd* pParent /*=NULL*/)
	: CXTPPropertyPage(CSegPartSelDlg::IDD)
    , m_dwUnitRoad(0)
    , m_dwNodeNum(0)
    , m_bDouble(TRUE)
{
    m_pSegParam = NULL;
}

CSegPartSelDlg::~CSegPartSelDlg()
{
}

void CSegPartSelDlg::DoDataExchange(CDataExchange* pDX)
{
    CXTPPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBOCROSSBEAR, m_comboCrossBear);
    DDX_Control(pDX, IDC_COMBOFIXROPEUP, m_comboFixRopeUp);
    DDX_Control(pDX, IDC_COMBOFIXROPEBELOW, m_comboFixRopeBelow);
    DDX_Text(pDX, IDC_EDITUNITROAD, m_dwUnitRoad);
    DDX_Text(pDX, IDC_EDITNODENUM, m_dwNodeNum);
    DDX_Control(pDX, IDC_COMBOPILLARLEFT, m_comboPillarLeft);
    DDX_Control(pDX, IDC_COMBOPILLARRIGHT, m_comboPillarRight);
    DDX_Check(pDX, IDC_CHECK1, m_bDouble);
}


BEGIN_MESSAGE_MAP(CSegPartSelDlg, CXTPPropertyPage)
    ON_BN_CLICKED(IDCANCEL, &CSegPartSelDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDB_PARTMANAGER, &CSegPartSelDlg::OnBnClickedPartmanager)
END_MESSAGE_MAP()


// CSegPartSelDlg ��Ϣ�������


BOOL CSegPartSelDlg::OnInitDialog()
{
    CXTPPropertyPage::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
//     <item name="������" id="1011" type="1" />
//     <item name="�Ӵ���" id="1012" type="1" />
//     <item name="���������" id="1013" type="1" />
//     <item name="�ϲ��̶���" id="1013" type="1" />
//     <item name="�²��̶���" id="1013" type="1" />
    //��ʼ���б�
    m_dwUnitRoad = m_pSegParam->m_dwUnitRoad;
    m_dwNodeNum = m_pSegParam->m_dwNodeNum;
    m_bDouble = m_pSegParam->m_bDouble;
    UpdateData(FALSE);

    ResetCombo();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CSegPartSelDlg::ResetCombo()
{
    m_comboCrossBear.ResetContent();
    m_comboFixRopeUp.ResetContent();
    m_comboFixRopeBelow.ResetContent();
    m_comboPillarLeft.ResetContent();
    m_comboPillarRight.ResetContent();

    CPartDBManager partDb;
    vector<PartParam>::iterator itPart;

    vector<PartParam> vecParts;
    int nIndex = 0;
    partDb.GetPartsByTypeId(vecParts, 1013);//��С��̶���ʹ��ͬһ���͵����
    for (itPart = vecParts.begin(); itPart != vecParts.end(); itPart++)
    {
        m_comboCrossBear.AddString(itPart->m_strName.c_str());
        m_comboCrossBear.SetItemData(nIndex, itPart->m_dwId);
        if (itPart->m_dwId == m_pSegParam->m_dwCrossBear)
        {
            m_comboCrossBear.SetCurSel(nIndex);
        }

        m_comboFixRopeUp.AddString(itPart->m_strName.c_str());
        m_comboFixRopeUp.SetItemData(nIndex, itPart->m_dwId);
        if (itPart->m_dwId == m_pSegParam->m_dwFixRopeUp)
        {
            m_comboFixRopeUp.SetCurSel(nIndex);
        }

        m_comboFixRopeBelow.AddString(itPart->m_strName.c_str());
        m_comboFixRopeBelow.SetItemData(nIndex, itPart->m_dwId);
        if (itPart->m_dwId == m_pSegParam->m_dwFixRopeBelow)
        {
            m_comboFixRopeBelow.SetCurSel(nIndex);
        }

        nIndex++;
    }

    CPillarDBManager pillarDb;
    vector<PillarParam> vecPillars;
    pillarDb.GetAllPillars(vecPillars);

    vector<PillarParam>::iterator itPillar;
    nIndex = 0;
    for (itPillar = vecPillars.begin(); itPillar != vecPillars.end(); itPillar++)
    {
        m_comboPillarLeft.AddString(itPillar->m_strName.c_str());
        m_comboPillarLeft.SetItemData(nIndex, itPillar->m_dwId);
        m_comboPillarRight.AddString(itPillar->m_strName.c_str());
        m_comboPillarRight.SetItemData(nIndex, itPillar->m_dwId);
        if (itPillar->m_dwId == m_pSegParam->m_dwPillarLeft)
        {
            m_comboPillarLeft.SetCurSel(nIndex);
        }
        if (itPillar->m_dwId == m_pSegParam->m_dwPillarRight)
        {
            m_comboPillarRight.SetCurSel(nIndex);
        }

        nIndex++;
    }
}

DWORD CSegPartSelDlg::GetPartIdFromCombo(const CComboBox& comboBox)
{
    int nSel = comboBox.GetCurSel();
    if (-1 != nSel)
    {
        return comboBox.GetItemData(nSel);
    }
    return -1;
}


void CSegPartSelDlg::OnBnClickedCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    if (::MessageBox(NULL, _T("����δ��ɣ���ȷ��Ҫȡ����"), _T("��ʾ"), MB_YESNO) == IDNO)
    {
        return;
    }
    CXTPPropertyPage::OnCancel();
}


BOOL CSegPartSelDlg::OnSetActive()
{
    // TODO: �ڴ����ר�ô����/����û���

    CPropertySheet* pParent = (CPropertySheet*)GetParent(); // ������Ա��ָ��
    if (NULL != pParent)
    {
        pParent->SetWizardButtons(PSWIZB_NEXT); // �������Ա����ʾ��ťֻΪ��һ��
    }
    return CXTPPropertyPage::OnSetActive();
}


LRESULT CSegPartSelDlg::OnWizardNext()
{
    // TODO: �ڴ����ר�ô����/����û���
    UpdateData(TRUE);
    if (m_dwUnitRoad == 0 || m_dwUnitRoad > 20)//�ݶ��������20���ɵ���
    {
        AfxMessageBox(_T("�ɵ�������Ϊ0���Ҳ��ܴ���20������������!"));
        return -1;
    }
    if (m_dwNodeNum < m_dwUnitRoad)
    {
        AfxMessageBox(_T("�ڵ�������С�ڹɵ���������������!"));
        return -1;
    }

    DWORD dwCrossBear = GetPartIdFromCombo(m_comboCrossBear);
    DWORD dwFixRopeUp = GetPartIdFromCombo(m_comboFixRopeUp);
    DWORD dwFixRopeBelow = GetPartIdFromCombo(m_comboFixRopeBelow);
    DWORD dwPillarLeft = GetPartIdFromCombo(m_comboPillarLeft);
    DWORD dwPillarRight = GetPartIdFromCombo(m_comboPillarRight);
//     if (-1 == dwCrossBear || -1 == dwFixRopeUp || -1 == dwFixRopeBelow
//         || -1 == dwHeadStick || -1 == dwEarRingStick 
//         || -1 == dwSpring || -1 == dwEar 
//         || -1 == dwPillarLeft || -1 == dwPillarRight)
//     {
//         AfxMessageBox(_T("��ѡ���������Ͷ�Ӧ�����"));
//         return -1;
//     }
    m_pSegParam->m_dwUnitRoad = m_dwUnitRoad;
    m_pSegParam->m_dwNodeNum = m_dwNodeNum;
    m_pSegParam->m_dwCrossBear = dwCrossBear;
    m_pSegParam->m_dwFixRopeUp = dwFixRopeUp;
    m_pSegParam->m_dwFixRopeBelow = dwFixRopeBelow;
    m_pSegParam->m_dwPillarLeft = dwPillarLeft;
    m_pSegParam->m_dwPillarRight = dwPillarRight;
    m_pSegParam->m_bDouble = m_bDouble;

    CSegParamDBManager paramDB;
    paramDB.Update(*m_pSegParam, m_pSegParam->m_dwSegId, UPDATETYPE_PART);

    return CXTPPropertyPage::OnWizardNext();
}


void CSegPartSelDlg::OnBnClickedPartmanager()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CPartDlg dlg;
    dlg.DoModal();

    ResetCombo();
}
