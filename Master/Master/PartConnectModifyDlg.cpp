// PartConnectModifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "MainFrm.h"
#include "PartConnectModifyDlg.h"
#include "afxdialogex.h"
#include "DB/PartDBManager.h"
#include "DB/PartConnectDBManager.h"
#include "DB/PartTypeManager.h"
#include "DB/PartTypeDBManager.h"
#include "GlobalFun.h"
#include "CommonFunction/CommonFunction.h"


// CPartConnectModifyDlg 对话框

IMPLEMENT_DYNAMIC(CPartConnectModifyDlg, CDialog)

CPartConnectModifyDlg::CPartConnectModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartConnectModifyDlg::IDD, pParent)
	, m_partConnectId(-1)
	, m_curSelectPartListIndex(-1)
{

}

CPartConnectModifyDlg::~CPartConnectModifyDlg()
{
}

void CPartConnectModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PART_LIST, m_partList);
	DDX_Control(pDX, IDC_PART_TYPE_COMBO, m_partTypeBox);
	DDX_Control(pDX, IDC_PART_COMBO, m_partBox);
}


BEGIN_MESSAGE_MAP(CPartConnectModifyDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_PART_TYPE_COMBO, &CPartConnectModifyDlg::OnSelchangePartTypeCombo)
	ON_BN_CLICKED(IDC_ADD_BTN, &CPartConnectModifyDlg::OnBnClickedAddBtn)
	ON_BN_CLICKED(IDC_DEL_BTN, &CPartConnectModifyDlg::OnBnClickedDelBtn)
	ON_BN_CLICKED(IDC_INSERT_BTN, &CPartConnectModifyDlg::OnBnClickedInsertBtn)
	ON_BN_CLICKED(IDOK, &CPartConnectModifyDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_PART_LIST, &CPartConnectModifyDlg::OnClickPartList)
END_MESSAGE_MAP()


// CPartConnectModifyDlg 消息处理程序


BOOL CPartConnectModifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_partList.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_imageList.Create(1, 18, ILC_COLOR, 0, 1);
	m_partList.SetImageList(&m_imageList, LVSIL_SMALL);

	m_partList.InsertColumn(0, _T("序号"));
	m_partList.InsertColumn(1, _T("零件名称"));
	m_partList.InsertColumn(2, _T("重量"));
	m_partList.InsertColumn(3, _T("尺寸"));

	HWND hWndHeader = m_partList.GetDlgItem(0)->GetSafeHwnd();
	m_headerCtrl.SubclassWindow(hWndHeader);
	m_headerCtrl.EnableAutoSize(TRUE);
	m_headerCtrl.ResizeColumnsToFit();

	initPartList(m_partConnectId);
	initPartTypeComboBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPartConnectModifyDlg::initPartList(DWORD dwId)
{
	if (-1 != dwId)
	{
		CPartConnectDBManager db;
		PartConnectParam param = db.GetPartConnectById(CMainFrame::CurSegId, dwId);
		CGlobalFun::GetStringVectorByToken(param.m_partIds.c_str(), m_partIdVec, _T('|'));
		SetDlgItemTextA(IDC_NAME_EDIT, param.m_name.c_str());
	}
	resetPartList();
}


void CPartConnectModifyDlg::resetPartList(void)
{
	m_partList.DeleteAllItems();

	CPartDBManager partDB;
	PartParam part;
	for (size_t index = 0; index < m_partIdVec.size(); ++index)
	{
		DWORD dwPartId = atoi(m_partIdVec[index]);
		partDB.GetPartInfoById(part, dwPartId);
		m_partList.InsertItem(index, toString(index).c_str());
		m_partList.SetItemData(index, part.m_dwId);
		m_partList.SetItemText(index, 1, part.m_strName.c_str());
		m_partList.SetItemText(index, 2, part.m_strWeight.c_str());
		m_partList.SetItemText(index, 3, part.m_strLength.c_str());
	}
	m_partList.AutoSizeColumn();
	m_curSelectPartListIndex = -1;
}


void CPartConnectModifyDlg::addPartList(DWORD partId)
{
	m_partIdVec.push_back(toString((int)partId).c_str());
	resetPartList();
}


void CPartConnectModifyDlg::delPartList(size_t index)
{
	size_t idx = 0;
	for (vector<CString>::iterator iter = m_partIdVec.begin(); m_partIdVec.end() != iter; ++iter)
	{
		if (index == idx)
		{
			m_partIdVec.erase(iter);
			break;
		}
		++idx;
	}
	resetPartList();
}


void CPartConnectModifyDlg::insertPartList(size_t index, DWORD partId)
{
	m_partIdVec.insert(m_partIdVec.begin() + index, toString((int)partId).c_str());
	resetPartList();
}


void CPartConnectModifyDlg::updatePartList(size_t index, DWORD partId)
{
	m_partIdVec[index] = toString((int)partId).c_str();
	resetPartList();
}


void CPartConnectModifyDlg::saveToDB(void)
{
	if (-1 == m_partConnectId)
		return;
	
	CPartDBManager partDB;
	PartParam part;
	vector<CString> partNameVec;
	for (size_t index = 0; index < m_partIdVec.size(); ++index)
	{
		DWORD dwPartId = atoi(m_partIdVec[index]);
		partDB.GetPartInfoById(part, dwPartId);
		partNameVec.push_back(part.m_strName.c_str());
	}
	
	CPartConnectDBManager db;
	PartConnectParam param = db.GetPartConnectById(CMainFrame::CurSegId, m_partConnectId);
	CString partIdStr;
	CGlobalFun::FormatStringByToken(partIdStr, m_partIdVec, _T('|'));
	CString partNameStr;
	CGlobalFun::FormatStringByToken(partNameStr, partNameVec, _T('|'));
	param.m_partIds = partIdStr;
	param.m_partNames = partNameStr;
	db.Update(param);
}


void CPartConnectModifyDlg::initPartTypeComboBox(void)
{
	m_partTypeBox.ResetContent();

	vector<PartType> partTypeVec = CPartTypeManager::m_vecPartType;
	size_t index = 0;
	for (vector<PartType>::iterator iter = partTypeVec.begin(); partTypeVec.end() !=  iter; ++iter)
	{
		if (1010 == iter->m_dwId)		// 支柱
			continue;

		m_partTypeBox.AddString(iter->m_strName);
		m_partTypeBox.SetItemData(index, iter->m_dwId);
		++index;
	}
	m_partTypeBox.SetCurSel(-1);
}


void CPartConnectModifyDlg::setSelectPartTypeComboBox(DWORD partTypeId)
{
	DWORD curSelectPartTypeId = m_partTypeBox.GetItemData(m_partTypeBox.GetCurSel());
	if (partTypeId == curSelectPartTypeId)
		return;

	for (int index = 0; index < m_partTypeBox.GetCount(); ++index)
	{
		if (partTypeId == m_partTypeBox.GetItemData(index))
		{
			m_partTypeBox.SetCurSel(index);
		}
	}
}


void CPartConnectModifyDlg::resetPartComboBox(DWORD partTypeId, DWORD partId)
{
	m_partBox.ResetContent();

	CPartDBManager partDB;
	vector<PartParam> partVec;
	partDB.GetPartsByTypeId(partVec, partTypeId);
	for (size_t index = 0; index < partVec.size(); ++index)
	{
		PartParam part = partVec[index];
		m_partBox.AddString(part.m_strName.c_str());
		m_partBox.SetItemData(index, part.m_dwId);
		if (partId == part.m_dwId)
		{
			m_partBox.SetCurSel(index);
		}
	}
}



void CPartConnectModifyDlg::OnSelchangePartTypeCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	int curSelectTypeIndex = m_partTypeBox.GetCurSel();
	DWORD partTypeId = m_partTypeBox.GetItemData(curSelectTypeIndex);
	resetPartComboBox(partTypeId, -1);
}


void CPartConnectModifyDlg::OnClickPartList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_curSelectPartListIndex = pNMItemActivate->iItem;
	if (-1 == m_curSelectPartListIndex)
		return;
	
	DWORD partId = m_partList.GetItemData(m_curSelectPartListIndex);
	CPartDBManager partDB;
	PartParam part;
	partDB.GetPartInfoById(part, partId);
	setSelectPartTypeComboBox(part.m_dwTypeId);
	resetPartComboBox(part.m_dwTypeId, partId);
}


void CPartConnectModifyDlg::OnBnClickedAddBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	int curSelectPartIndex = m_partBox.GetCurSel();
	DWORD partId = m_partBox.GetItemData(curSelectPartIndex);
	if (-1 == partId)
	{
		AfxMessageBox(_T("请选中零件进行添加"));
		return;
	}
	addPartList(partId);
}


void CPartConnectModifyDlg::OnBnClickedDelBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (-1 == m_curSelectPartListIndex)
	{
		AfxMessageBox(_T("请从列表选中一个零件进行删除"));
		return;
	}
	if (::MessageBox(NULL, _T("您确定要从零件连接情况中移除选中的零件吗"), _T("提示"), MB_YESNO|MB_TASKMODAL) == IDYES)
	{
		delPartList(m_curSelectPartListIndex);
	}
}


void CPartConnectModifyDlg::OnBnClickedInsertBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	int curSelectPartIndex = m_partBox.GetCurSel();
	DWORD partId = m_partBox.GetItemData(curSelectPartIndex);
	if (-1 == partId)
	{
		AfxMessageBox(_T("请选中零件进行插入"));
		return;
	}
	if (-1 == m_curSelectPartListIndex)
	{
		AfxMessageBox(_T("请从列表选中要在之前插入的零件"));
		return;
	}
	insertPartList(m_curSelectPartListIndex, partId);
}


void CPartConnectModifyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (::MessageBox(NULL, _T("您确定要保存当前修改么？"), _T("提示"), MB_YESNO|MB_TASKMODAL) == IDYES)
	{
		saveToDB();
	}
	CDialog::OnOK();
}


void CPartConnectModifyDlg::SetPartConnectId(DWORD dwId)
{
	m_partConnectId = dwId;
}