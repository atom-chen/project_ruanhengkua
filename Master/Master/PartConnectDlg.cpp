// PartConnectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Master.h"
#include "MainFrm.h"
#include "PartConnectDlg.h"
#include "afxdialogex.h"
#include "DB/PartConnectDBManager.h"
#include "PartConnectModifyDlg.h"


// CPartConnectDlg �Ի���

IMPLEMENT_DYNAMIC(CPartConnectDlg, CPropertyPage)

CPartConnectDlg::CPartConnectDlg()
	: CPropertyPage(CPartConnectDlg::IDD)
	, m_curSelectListIndex(-1)
{

}

CPartConnectDlg::~CPartConnectDlg()
{
}

void CPartConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTPART, m_listPart);
}


BEGIN_MESSAGE_MAP(CPartConnectDlg, CPropertyPage)
	ON_NOTIFY(NM_CLICK, IDC_LISTPART, &CPartConnectDlg::OnClickListpart)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTPART, &CPartConnectDlg::OnDblclkListpart)
	ON_BN_CLICKED(IDC_MODIFY_BTN, &CPartConnectDlg::OnBnClickedModifyBtn)
END_MESSAGE_MAP()


// CPartConnectDlg ��Ϣ�������


BOOL CPartConnectDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listPart.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_imageList.Create(1, 18, ILC_COLOR, 0, 1);
	m_listPart.SetImageList(&m_imageList, LVSIL_SMALL);

	m_listPart.InsertColumn(0, _T("����"));
	m_listPart.InsertColumn(1, _T("����������"));
	
	HWND hWndHeader = m_listPart.GetDlgItem(0)->GetSafeHwnd();
	m_headCtrl.SubclassWindow(hWndHeader);
	m_headCtrl.EnableAutoSize(TRUE);
	m_headCtrl.ResizeColumnsToFit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CPartConnectDlg::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	CPropertySheet* pParent = (CPropertySheet*)GetParent(); // ������Ա��ָ��
	if (NULL != pParent)
	{
		pParent->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);	//��һ������һ��
	}

	updateList();

	return CPropertyPage::OnSetActive();
}


void CPartConnectDlg::updateList(void)
{
	m_listPart.DeleteAllItems();

	CPartConnectDBManager db;
	vector<PartConnectParam> vec = db.GetAllPartConnect(CMainFrame::CurSegId);
	for (size_t index = 0; index < vec.size(); ++index)
	{
		PartConnectParam param = vec[index];
		m_listPart.InsertItem(index, param.m_name.c_str());
		m_listPart.SetItemData(index, param.m_dwId);
		m_listPart.SetItemText(index, 1, param.m_partNames.c_str());
	}
	m_listPart.AutoSizeColumn();
}


void CPartConnectDlg::openModifyDlg(int dwId)
{
	CPartConnectModifyDlg dlg;
	dlg.SetPartConnectId(dwId);
	if (dlg.DoModal() == IDOK)
	{
		updateList();
	}
	m_curSelectListIndex = -1;
}


void CPartConnectDlg::OnClickListpart(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	m_curSelectListIndex = pNMItemActivate->iItem;
}

void CPartConnectDlg::OnDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if (-1 == pNMItemActivate->iItem)
		return;

	OnBnClickedModifyBtn();
}


void CPartConnectDlg::OnBnClickedModifyBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (-1 == m_curSelectListIndex)
	{
		AfxMessageBox(_T("��ѡ��һ����Ŀ�����޸�!"));
		return;
	}
	DWORD dwId = m_listPart.GetItemData(m_curSelectListIndex);
	openModifyDlg(dwId);
}
