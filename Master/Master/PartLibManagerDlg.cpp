// PartLibManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Master.h"
#include "PartLibManagerDlg.h"
#include "afxdialogex.h"


// CPartLibManagerDlg �Ի���

IMPLEMENT_DYNAMIC(CPartLibManagerDlg, CDialog)

CPartLibManagerDlg::CPartLibManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartLibManagerDlg::IDD, pParent)
    , m_strLibName(_T(""))
{

}

CPartLibManagerDlg::~CPartLibManagerDlg()
{
}

void CPartLibManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_NONSELTREE, m_treeNonSel);
    DDX_Control(pDX, IDC_SELTREE, m_treeSel);
    DDX_Control(pDX, IDC_EDITLIBNAME, m_edtLibName);
    DDX_Text(pDX, IDC_EDITLIBNAME, m_strLibName);
}


BEGIN_MESSAGE_MAP(CPartLibManagerDlg, CDialog)
    ON_BN_CLICKED(IDB_SINGLESEL, &CPartLibManagerDlg::OnBnClickedSinglesel)
    ON_BN_CLICKED(IDB_SINGLEUNSEL, &CPartLibManagerDlg::OnBnClickedSingleunsel)
    ON_BN_CLICKED(IDOK, &CPartLibManagerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPartLibManagerDlg ��Ϣ�������


void CPartLibManagerDlg::OnBnClickedSinglesel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CPartLibManagerDlg::OnBnClickedSingleunsel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CPartLibManagerDlg::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialog::OnOK();
}
