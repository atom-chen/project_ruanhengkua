// NodeModifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "MainFrm.h"
#include "NodeModifyDlg.h"
#include "afxdialogex.h"
#include "DB/PartDBManager.h"
#include "DB/PartConnectDBManager.h"
#include "DB/NodeDBManager.h"
#include "GlobalFun.h"
#include "CommonFunction/CommonFunction.h"


// CNodeModifyDlg 对话框

IMPLEMENT_DYNAMIC(CNodeModifyDlg, CDialog)

CNodeModifyDlg::CNodeModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNodeModifyDlg::IDD, pParent)
	, m_nodeId(-1)
	, m_nodeName(_T(""))
	, m_nodeWeight(_T(""))
	, m_curSelectPartListIndex(-1)
	, m_curSelectNodePartListIndex(-1)
{

}

CNodeModifyDlg::~CNodeModifyDlg()
{
}

void CNodeModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NODE_NAME_EDIT, m_nodeName);
	DDX_Text(pDX, IDC_NODE_WEIGHT_EDIT, m_nodeWeight);
	DDX_Control(pDX, IDC_PART_LIST, m_partList);
	DDX_Control(pDX, IDC_NODE_PART_LIST, m_nodePartList);
}


BEGIN_MESSAGE_MAP(CNodeModifyDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_PART_LIST, &CNodeModifyDlg::OnClickPartList)
	ON_NOTIFY(NM_CLICK, IDC_NODE_PART_LIST, &CNodeModifyDlg::OnClickNodePartList)
	ON_BN_CLICKED(IDC_ADD_BTN, &CNodeModifyDlg::OnClickedAddBtn)
	ON_BN_CLICKED(IDC_DEL_BTN, &CNodeModifyDlg::OnClickedDelBtn)
	ON_BN_CLICKED(IDOK, &CNodeModifyDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_NODE_WEIGHT_EDIT, &CNodeModifyDlg::OnChangeWeightEdit)
END_MESSAGE_MAP()


// CNodeModifyDlg 消息处理程序


BOOL CNodeModifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_partList.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_partImageList.Create(1, 18, ILC_COLOR, 0, 1);
	m_partList.SetImageList(&m_partImageList, LVSIL_SMALL);

	m_partList.InsertColumn(0, _T("零件名称"));
	m_partList.InsertColumn(1, _T("重量"));
	m_partList.InsertColumn(2, _T("尺寸"));

	HWND hWndPartHeader = m_partList.GetDlgItem(0)->GetSafeHwnd();
	m_partHeaderCtrl.SubclassWindow(hWndPartHeader);
	m_partHeaderCtrl.EnableAutoSize(TRUE);
	m_partHeaderCtrl.ResizeColumnsToFit();
	//
	m_nodePartList.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_nodePartImageList.Create(1, 18, ILC_COLOR, 0, 1);
	m_nodePartList.SetImageList(&m_nodePartImageList, LVSIL_SMALL);

	m_nodePartList.InsertColumn(0, _T("零件名称"));
	m_nodePartList.InsertColumn(1, _T("重量"));
	m_nodePartList.InsertColumn(2, _T("尺寸"));

	HWND hWndNodePartHeader = m_nodePartList.GetDlgItem(0)->GetSafeHwnd();
	m_nodePartHeaderCtrl.SubclassWindow(hWndNodePartHeader);
	m_nodePartHeaderCtrl.EnableAutoSize(TRUE);
	m_nodePartHeaderCtrl.ResizeColumnsToFit();
	//
	SetWindowText(-1 == m_nodeId ? _T("添加节点") : _T("修改节点"));

	initPartList();
	initNodePartList(m_nodeId);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CNodeModifyDlg::initPartList(void)
{
	CPartDBManager partDB;
	vector<PartParam> partVec;
	partDB.GetParts(partVec);
	//
	m_partList.DeleteAllItems();
	for (size_t index = 0; index < partVec.size(); ++index)
	{
		PartParam part = partVec[index];
		m_partList.InsertItem(index, part.m_strName.c_str());
		m_partList.SetItemData(index, part.m_dwId);
		m_partList.SetItemText(index, 1, part.m_strWeight.c_str());
		m_partList.SetItemText(index, 2, part.m_strLength.c_str());
	}
	m_partList.AutoSizeColumn();
	m_curSelectPartListIndex = -1;
}


void CNodeModifyDlg::initNodePartList(DWORD dwNodeId)
{
	if (-1 != dwNodeId)	// 修改节点
	{
		CNodeDBManager db;
		SoftCrossNode param;
		db.GetNodeById(param, dwNodeId);
		CGlobalFun::GetStringVectorByToken(param.m_strPartId.c_str(), m_nodePartIdVec, _T('|'));
		SetDlgItemTextA(IDC_NODE_NAME_EDIT, param.m_strName.c_str());
		
		GetDlgItem(IDC_NODE_NAME_EDIT)->EnableWindow(FALSE);
		// 没有零件，启用自输入重量控件，禁用材料控件
		if (0 == m_nodePartIdVec.size())
		{
			GetDlgItem(IDC_NODE_WEIGHT_EDIT)->EnableWindow(TRUE);
			if ("" != param.m_strWeight)
			{
				SetDlgItemTextA(IDC_NODE_WEIGHT_EDIT, param.m_strWeight.c_str());
				GetDlgItem(IDC_ADD_BTN)->EnableWindow(FALSE);
				GetDlgItem(IDC_DEL_BTN)->EnableWindow(FALSE);
			}
		}
		else	// 有零件，启用材料控件，禁用自输入重量控件
		{
			GetDlgItem(IDC_NODE_WEIGHT_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_ADD_BTN)->EnableWindow(TRUE);
			GetDlgItem(IDC_DEL_BTN)->EnableWindow(TRUE);
		}
	}
	resetNodePartList();
}


void CNodeModifyDlg::resetNodePartList(void)
{
	m_nodePartList.DeleteAllItems();

	CPartDBManager partDB;
	PartParam part;
	for (size_t index = 0; index < m_nodePartIdVec.size(); ++index)
	{
		DWORD dwPartId = atoi(m_nodePartIdVec[index]);
		partDB.GetPartInfoById(part, dwPartId);
		m_nodePartList.InsertItem(index, part.m_strName.c_str());
		m_nodePartList.SetItemData(index, part.m_dwId);
		m_nodePartList.SetItemText(index, 1, part.m_strWeight.c_str());
		m_nodePartList.SetItemText(index, 2, part.m_strLength.c_str());
	}
	m_nodePartList.AutoSizeColumn();
}


void CNodeModifyDlg::addNodePartList(DWORD partId)
{
	m_nodePartIdVec.push_back(toString((int)partId).c_str());
	resetNodePartList();
}


void CNodeModifyDlg::delNodePartList(size_t index)
{
	size_t idx = 0;
	for (vector<CString>::iterator iter = m_nodePartIdVec.begin(); m_nodePartIdVec.end() != iter; ++iter)
	{
		if (index == idx)
		{
			m_nodePartIdVec.erase(iter);
			break;
		}
		++idx;
	}
	resetNodePartList();
}


void CNodeModifyDlg::saveToDB(DWORD dwNodeId)
{
	double totalWeights = 0.0;
	CString strTemp;
	vector<CString> partIdVector;
	// 没有零件，则总重量取自输入重量值
	if (0 == m_nodePartIdVec.size())
	{
		totalWeights = atof(m_nodeWeight.GetBuffer());
	}
	// 有零件的话，则重量为各个零件重量的累加
	for (vector<CString>::iterator iter = m_nodePartIdVec.begin(); m_nodePartIdVec.end() != iter; ++iter)
	{
		DWORD partId = atoi(*iter);
		// 获取零件
		CPartDBManager dbPart;
		PartParam part;
		dbPart.GetPartInfoById(part, partId);
		// 计算
		totalWeights += atof(part.m_strWeight.c_str());
		strTemp.Format(_T("%d"), partId);
		partIdVector.push_back(strTemp);
	}
	// 格式化节点
	SoftCrossNode node;
	node.m_strName = m_nodeName;
	node.m_strWeight = CGlobalFun::DoubleToString(totalWeights);
	CGlobalFun::FormatStringByToken(strTemp, partIdVector, _T('|'));
	node.m_strPartId = strTemp;
	// 保存数据库
	CNodeDBManager dbNode;
	if (-1 == dwNodeId)		// 添加
	{
		dbNode.Add(node);
	}
	else					// 更新
	{
		dbNode.Update(node, m_nodeId);
	}
}


void CNodeModifyDlg::OnClickPartList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_curSelectPartListIndex = pNMItemActivate->iItem;
}


void CNodeModifyDlg::OnClickNodePartList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_curSelectNodePartListIndex = pNMItemActivate->iItem;
}


void CNodeModifyDlg::OnClickedAddBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	if (-1 == m_curSelectPartListIndex)
	{
		AfxMessageBox(_T("请在可选零件列表里选择要添加的零件"));
		return;
	}
	if (m_nodePartIdVec.size() >= 20)
	{
		AfxMessageBox(_T("最多只能添加20个零件"));
		return;
	}
	DWORD partId = m_partList.GetItemData(m_curSelectPartListIndex);
	addNodePartList(partId);
	m_curSelectPartListIndex = -1;
	// 禁用自输入重量控件
	GetDlgItem(IDC_NODE_WEIGHT_EDIT)->EnableWindow(FALSE);
}


void CNodeModifyDlg::OnClickedDelBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	if (-1 == m_curSelectNodePartListIndex)
	{
		AfxMessageBox(_T("请在已选零件列表里选择要删除的零件"));
		return;
	}
	delNodePartList(m_curSelectNodePartListIndex);
	m_curSelectNodePartListIndex = -1;
	// 启用自输入重量控件
	if (0 == m_nodePartIdVec.size())
	{
		GetDlgItem(IDC_NODE_WEIGHT_EDIT)->EnableWindow(TRUE);
	}
}


void CNodeModifyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	// 节点名字为空
	if (_T("") == m_nodeName)
	{
		AfxMessageBox(_T("请输入节点名称"));
		return;
	}
	// 没有零件并且自输入重量为""
	if (_T("") == m_nodeWeight && 0 == m_nodePartIdVec.size())
	{
		AfxMessageBox(_T("请添加零件或自输入节点重量"));
		return;
	}
	// 自输入重量非数字
	if (false == isDigit(m_nodeWeight.GetBuffer()))
	{
		AfxMessageBox(_T("自输入重量含有非数字符号"));
		return;
	}
	CString tip = -1 == m_nodeId ? _T("您确定要添加该节点么？") : _T("您确定要修改该节点么？");
	if (::MessageBox(NULL, tip, _T("提示"), MB_YESNO|MB_TASKMODAL) == IDYES)
	{
		saveToDB(m_nodeId);
	}

	CDialog::OnOK();
}


void CNodeModifyDlg::OnChangeWeightEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString text;
	GetDlgItemTextA(IDC_NODE_WEIGHT_EDIT, text);
	BOOL enable = _T("") == text ? TRUE : FALSE;
	// 根据自输入是否为空启用/禁用材料控件
	GetDlgItem(IDC_ADD_BTN)->EnableWindow(enable);
	GetDlgItem(IDC_DEL_BTN)->EnableWindow(enable);
}


void CNodeModifyDlg::SetNodeId(DWORD dwId)
{
	m_nodeId = dwId;
}


