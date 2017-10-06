// PartConnectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "MainFrm.h"
#include "PartConnectDlg.h"
#include "afxdialogex.h"
#include "DB/PartConnectDBManager.h"
#include "PartConnectModifyDlg.h"


// CPartConnectDlg 对话框

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


// CPartConnectDlg 消息处理程序


BOOL CPartConnectDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listPart.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_imageList.Create(1, 18, ILC_COLOR, 0, 1);
	m_listPart.SetImageList(&m_imageList, LVSIL_SMALL);

	m_listPart.InsertColumn(0, _T("名称"));
	m_listPart.InsertColumn(1, _T("零件连接情况"));
	
	HWND hWndHeader = m_listPart.GetDlgItem(0)->GetSafeHwnd();
	m_headCtrl.SubclassWindow(hWndHeader);
	m_headCtrl.EnableAutoSize(TRUE);
	m_headCtrl.ResizeColumnsToFit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CPartConnectDlg::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* pParent = (CPropertySheet*)GetParent(); // 获得属性表的指针
	if (NULL != pParent)
	{
		pParent->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);	//上一步，下一步
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_curSelectListIndex = pNMItemActivate->iItem;
}

void CPartConnectDlg::OnDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (-1 == pNMItemActivate->iItem)
		return;

	OnBnClickedModifyBtn();
}


void CPartConnectDlg::OnBnClickedModifyBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (-1 == m_curSelectListIndex)
	{
		AfxMessageBox(_T("请选中一个条目进行修改!"));
		return;
	}
	DWORD dwId = m_listPart.GetItemData(m_curSelectListIndex);
	openModifyDlg(dwId);
}
