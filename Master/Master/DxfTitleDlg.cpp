// DxfTitleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Master.h"
#include "DxfTitleDlg.h"
#include "afxdialogex.h"
#include "DB/DxfTitleDBManager.h"
#include "DxfTitleSetDlg.h"
#include "dxf/DxfParser.h"


// CDxfTitleDlg �Ի���

IMPLEMENT_DYNAMIC(CDxfTitleDlg, CDialog)

CDxfTitleDlg::CDxfTitleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDxfTitleDlg::IDD, pParent)
	, m_curSelectListIndex(-1)
{

}

CDxfTitleDlg::~CDxfTitleDlg()
{
}

void CDxfTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DXFTITLE_LIST, m_listDxfTitle);
}


BEGIN_MESSAGE_MAP(CDxfTitleDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_DXFTITLE_LIST, &CDxfTitleDlg::OnClickListDxfTitle)
	ON_NOTIFY(NM_DBLCLK, IDC_DXFTITLE_LIST, &CDxfTitleDlg::OnDblclkListDxfTitle)
	ON_BN_CLICKED(ID_ADD_BTN, &CDxfTitleDlg::OnBnClickedAdd)
	ON_BN_CLICKED(ID_MODIFY_BTN, &CDxfTitleDlg::OnBnClickedModify)
	ON_BN_CLICKED(ID_DEL_BTN, &CDxfTitleDlg::OnBnClickedDel)
	ON_BN_CLICKED(ID_PARSE_BTN, &CDxfTitleDlg::OnBnClickedParse)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


void CDxfTitleDlg::ResetDxfTitleList()
{
	m_listDxfTitle.DeleteAllItems();

	DxfTitleDBManager db;
	std::vector<DxfTitle> vec = db.GetAllTitles();
	for (size_t index = 0; index < vec.size(); ++index)
	{
		DxfTitle param = vec[index];
		std::vector<DxfTitleCell> cellVec = db.ChangeToTitleCell(param.titleList);
		std::string str = "";
		for (size_t i = 0; i<cellVec.size(); ++i)
		{
			std::string temp = cellVec[i].toShowString();
			str += 0 == i ? temp : "|" + temp;
		}
		m_listDxfTitle.InsertItem(index, str.c_str());
		m_listDxfTitle.SetItemData(index, param.dwId);
	}
	m_listDxfTitle.AutoSizeColumn();
}

// CDxfTitleDlg ��Ϣ�������

BOOL CDxfTitleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listDxfTitle.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_imageList.Create(1, 18, ILC_COLOR, 0, 1);
	m_listDxfTitle.SetImageList(&m_imageList, LVSIL_SMALL);

	m_listDxfTitle.InsertColumn(0, _T("��ϸ�����ͷ"));

	HWND hWndHeader = m_listDxfTitle.GetDlgItem(0)->GetSafeHwnd();
	m_headCtrl.SubclassWindow(hWndHeader);
	m_headCtrl.EnableAutoSize(TRUE);
	m_headCtrl.ResizeColumnsToFit();

	ResetDxfTitleList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDxfTitleDlg::OnClickListDxfTitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	m_curSelectListIndex = pNMItemActivate->iItem;
}

void CDxfTitleDlg::OnDblclkListDxfTitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if (-1 == pNMItemActivate->iItem)
		return;

	OnBnClickedModify();
}

void CDxfTitleDlg::OnBnClickedAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDxfTitleSetDlg dlg;
	dlg.Init(-1);
	dlg.DoModal();
	ResetDxfTitleList();
}

void CDxfTitleDlg::OnBnClickedModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (-1 == m_curSelectListIndex)
	{
		AfxMessageBox(_T("��ѡ��һ����Ŀ�����޸�!"));
		return;
	}
	DWORD dwId = m_listDxfTitle.GetItemData(m_curSelectListIndex);
	CDxfTitleSetDlg dlg;
	dlg.Init(dwId);
	dlg.DoModal();
	ResetDxfTitleList();
}

void CDxfTitleDlg::OnBnClickedDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (-1 == m_curSelectListIndex)
	{
		AfxMessageBox(_T("��ѡ��һ����Ŀ����ɾ��!"));
		return;
	}
	if (::MessageBox(NULL, _T("��ȷ��Ҫɾ��������¼ô��"), _T("��ʾ"), MB_YESNO) == IDNO)
		return;

	DWORD dwId = m_listDxfTitle.GetItemData(m_curSelectListIndex);
	DxfTitleDBManager db;
	db.Del(dwId);
	ResetDxfTitleList();
}

void CDxfTitleDlg::OnBnClickedParse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (-1 == m_curSelectListIndex)
	{
		AfxMessageBox(_T("��ѡ��һ����Ŀ��Ϊ�����ĸ�ʽ!"));
		return;
	}
	DWORD dwId = m_listDxfTitle.GetItemData(m_curSelectListIndex);
	DxfTitleDBManager db;
	DxfTitle title = db.GetTitleById(dwId);
	std::vector<DxfTitleCell> cellVector = db.ChangeToTitleCell(title.titleList);
	std::vector<std::string> titleList;
	std::string key;
	std::string unit;
	std::string value;
	for (size_t i = 0; i<cellVector.size(); ++i)
	{
		DxfTitleCell cell = cellVector[i];
		titleList.push_back(cell.title);
		switch (cell.property)
		{
		case DxfTitleCellProperty::DTCP_KEY:
			key = cell.title;
			break;
		case DxfTitleCellProperty::DTCP_UNIT:
			unit = cell.title;
			break;
		case DxfTitleCellProperty::DTCP_VALUE:
			value = cell.title;
			break;
		}
	}
	GetDlgItem(IDC_TEXT_LABEL_BG)->ShowWindow(TRUE);
	GetDlgItem(IDC_TEXT_LABEL)->ShowWindow(TRUE);
	SetDlgItemTextA(IDC_TEXT_LABEL, "���ڽ���.dxf�ļ�����ϸ�����Ե� ... ");
	std::vector<DxfComputeRow> rows = getInfoFromDxf(m_filenames, titleList, key, value, unit);
	GetDlgItem(IDC_TEXT_LABEL_BG)->ShowWindow(FALSE);
	GetDlgItem(IDC_TEXT_LABEL)->ShowWindow(FALSE);
	CFileDialog fileDlg(FALSE, "xls", "*.xls");
	if (IDOK != fileDlg.DoModal())
		return;

	CString filename = fileDlg.GetPathName();
	writeInfoToXls(filename.GetBuffer(), rows, key, value, unit);
}

void CDxfTitleDlg::SetFilenames(std::vector<std::string> filenames)
{
	m_filenames = filenames;
}

