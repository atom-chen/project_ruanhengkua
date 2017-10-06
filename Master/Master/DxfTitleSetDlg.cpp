// DxfTitleSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "DxfTitleSetDlg.h"
#include "afxdialogex.h"


// CDxfTitleSetDlg 对话框

IMPLEMENT_DYNAMIC(CDxfTitleSetDlg, CDialog)

CDxfTitleSetDlg::CDxfTitleSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDxfTitleSetDlg::IDD, pParent)
	, m_curSelectListIndex(-1)
	, m_dwId(-1)
{

}

CDxfTitleSetDlg::~CDxfTitleSetDlg()
{
}

void CDxfTitleSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TITLE_LIST, m_listDxfTitle);
	DDX_Control(pDX, IDC_TITLE_EDIT, m_edtTitle);
	DDX_Control(pDX, IDC_PROPERTY_COMBO, m_comboProperty);
}


BEGIN_MESSAGE_MAP(CDxfTitleSetDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_TITLE_LIST, &CDxfTitleSetDlg::OnClickListTitle)
	ON_BN_CLICKED(ID_ADD_BTN, &CDxfTitleSetDlg::OnBnClickedAddBtn)
	ON_BN_CLICKED(ID_DEL_BTN, &CDxfTitleSetDlg::OnBnClickedDelBtn)
	ON_BN_CLICKED(ID_SAVE_BTN, &CDxfTitleSetDlg::OnBnClickedSaveBtn)
END_MESSAGE_MAP()


// CDxfTitleSetDlg 消息处理程序


BOOL CDxfTitleSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(-1 == m_dwId ? _T("添加dxf文件明细表表头") : _T("修改dxf文件明细表表头"));

	m_listDxfTitle.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_imageList.Create(1, 18, ILC_COLOR, 0, 1);
	m_listDxfTitle.SetImageList(&m_imageList, LVSIL_SMALL);

	m_listDxfTitle.InsertColumn(0, _T("表头"));
	m_listDxfTitle.InsertColumn(1, _T("属性"));

	HWND hWndHeader = m_listDxfTitle.GetDlgItem(0)->GetSafeHwnd();
	m_headCtrl.SubclassWindow(hWndHeader);
	m_headCtrl.EnableAutoSize(TRUE);
	m_headCtrl.ResizeColumnsToFit();

	InitCombo();
	m_listDxfTitle.SetColumnEditor(0, NULL, NULL, &m_edtTitle);
	m_listDxfTitle.SetColumnEditor(1, NULL, &CDxfTitleSetDlg::EndEditor, &m_comboProperty);

	InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDxfTitleSetDlg::InitCombo(void)
{
	m_comboProperty.ResetContent();
	m_comboProperty.AddString(DTCPNONE);
	m_comboProperty.SetItemData(0, 0);
	m_comboProperty.AddString(DTCPKEY);
	m_comboProperty.SetItemData(1, 1);
	m_comboProperty.AddString(DTCPUNIT);
	m_comboProperty.SetItemData(2, 2);
	m_comboProperty.AddString(DTCPVALUE);
	m_comboProperty.SetItemData(3, 3);
}

BOOL CDxfTitleSetDlg::EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	ASSERT(pWnd);
	switch(nColumn)
	{
	case 0:
		break;
	case 1:
		{
			CComboBox *pCmb = dynamic_cast<CComboBox *>(*pWnd);
			int index = pCmb->GetCurSel();
			if(index >= 0) pCmb->GetLBText(index, strSubItemText);
		}
		break;
	}
	return TRUE;
}

void CDxfTitleSetDlg::InitList(void)
{
	m_listDxfTitle.DeleteAllItems();
	for (size_t i = 0; i<m_cellVector.size(); ++i)
	{
		InsertList(i, m_cellVector[i]);
	}
}

void CDxfTitleSetDlg::InsertList(int index, DxfTitleCell cell)
{
	if (index < 0)
		return;

	m_listDxfTitle.InsertItem(index, cell.title.c_str());
	TCHAR* arrProperty[4] = {DTCPNONE, DTCPKEY, DTCPUNIT, DTCPVALUE};
	m_listDxfTitle.SetItemText(index, 1, arrProperty[cell.property]);
}

void CDxfTitleSetDlg::DeleteList(int index)
{
	if (index < 0 || index >= m_listDxfTitle.GetItemCount())
		return;

	m_listDxfTitle.DeleteItem(index);
}

void CDxfTitleSetDlg::OnClickListTitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_curSelectListIndex = pNMItemActivate->iItem;
}

void CDxfTitleSetDlg::OnBnClickedAddBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	DxfTitleCell cell;
	m_cellVector.push_back(cell);
	InsertList(m_cellVector.size() - 1, cell);
}

void CDxfTitleSetDlg::OnBnClickedDelBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (-1 == m_curSelectListIndex)
	{
		AfxMessageBox(_T("请选中一个条目进行删除!"));
		return;
	}
	std::vector<DxfTitleCell>::iterator iter = m_cellVector.begin();
	m_cellVector.erase(iter + m_curSelectListIndex);
	DeleteList(m_curSelectListIndex);
	m_curSelectListIndex = -1;
}


void CDxfTitleSetDlg::OnBnClickedSaveBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (::MessageBox(NULL, _T("您确定要保存以上操作么？"), _T("提示"), MB_YESNO) == IDNO)
		return;

	for (size_t i = 0; i<m_cellVector.size(); ++i)
	{
		m_cellVector[i].title = m_listDxfTitle.GetItemText(i, 0);
		std::string property = m_listDxfTitle.GetItemText(i, 1);
		if (0 == property.compare(DTCPNONE))
		{
			m_cellVector[i].property = DxfTitleCellProperty::DTCP_NONE;
		}
		else if (0 == property.compare(DTCPKEY))
		{
			m_cellVector[i].property = DxfTitleCellProperty::DTCP_KEY;
		}
		else if (0 == property.compare(DTCPUNIT))
		{
			m_cellVector[i].property = DxfTitleCellProperty::DTCP_UNIT;
		}
		else if (0 == property.compare(DTCPVALUE))
		{
			m_cellVector[i].property = DxfTitleCellProperty::DTCP_VALUE;
		}
	}
	// 条件判断
	size_t keyCount = 0, unitCount = 0, valueCount = 0;
	DxfTitle title;
	for (size_t j = 0; j<m_cellVector.size(); ++j)
	{
		DxfTitleCell cell = m_cellVector[j];
		if (0 == cell.title.compare(""))
		{
			AfxMessageBox(_T("存在表头为空的行"));
			return;
		}
		std::string temp = cell.toDBString();
		title.titleList += 0 == j ? temp : "|" + temp;
		switch (cell.property)
		{
		case DxfTitleCellProperty::DTCP_KEY:
			++keyCount;
			break;
		case DxfTitleCellProperty::DTCP_UNIT:
			++unitCount;
			break;
		case DxfTitleCellProperty::DTCP_VALUE:
			++valueCount;
			break;
		}
		if (keyCount > 1)
		{
			AfxMessageBox(_T("只能选择一个表头作为关键字"));
			return;
		}
		if (unitCount > 1)
		{
			AfxMessageBox(_T("只能选择一个表头作为单位"));
			return;
		}
		if (valueCount > 1)
		{
			AfxMessageBox(_T("只能选择一个表头作为统计值"));
			return;
		}
	}
	if (0 == keyCount)
	{
		AfxMessageBox(_T("必须选择一个表头作为关键字"));
		return;
	}
	if (0 == valueCount)
	{
		AfxMessageBox(_T("必须选择一个表头作为统计值"));
		return;
	}
	// 保存数据库
	DxfTitleDBManager db;
	if (-1 == m_dwId)
	{
		db.Add(title);
	} 
	else
	{
		db.Update(title, m_dwId);
	}
}

void CDxfTitleSetDlg::Init(DWORD dwId /* = -1 */)
{
	m_dwId = dwId;
	if (-1 != dwId)
	{
		DxfTitleDBManager db;
		DxfTitle title = db.GetTitleById(dwId);
		m_cellVector = db.ChangeToTitleCell(title.titleList);
	}
}

