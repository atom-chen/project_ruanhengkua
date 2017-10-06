// PillarPartDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "PillarPartDlg.h"
#include "afxdialogex.h"
#include "DB/PillarDBManager.h"
#include "DataValidator.h"

// CPillarPartDlg 对话框

IMPLEMENT_DYNAMIC(CPillarPartDlg, CDialog)

CPillarPartDlg::CPillarPartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPillarPartDlg::IDD, pParent)
    , m_strLen(_T(""))
    , m_strUp(_T(""))
    , m_strBelow(_T(""))
    , m_strWidthOfUp(_T(""))
    , m_strName(_T(""))
    , m_dwType(0)
{
    m_dwPillarId = -1;
    m_strOldName = _T("");
}

CPillarPartDlg::~CPillarPartDlg()
{
}

void CPillarPartDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDTLEN, m_strLen);
    DDX_Text(pDX, IDC_EDTUP, m_strUp);
    DDX_Text(pDX, IDC_EDTBELOW, m_strBelow);
    DDX_Text(pDX, IDC_EDTWIDTHUP, m_strWidthOfUp);
    DDX_Text(pDX, IDC_EDTNAME, m_strName);
    DDX_CBIndex(pDX, IDC_COMBOTYPE, m_dwType);
}


BEGIN_MESSAGE_MAP(CPillarPartDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CPillarPartDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPillarPartDlg 消息处理程序


void CPillarPartDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);

    if (!CDataValidator::IsUnsignFloat(m_strLen)
        || !CDataValidator::IsUnsignFloat(m_strUp)
        || !CDataValidator::IsUnsignFloat(m_strBelow)
        || !CDataValidator::IsUnsignFloat(m_strWidthOfUp))
    {
        AfxMessageBox("数据格式错误，请重新输入");
        return;
    }

    PillarParam part;
    part.m_strLength = m_strLen;
    part.m_strUp = m_strUp;
    part.m_strBelow = m_strBelow;
    part.m_strWidthOfUp = m_strWidthOfUp;
    part.m_strName = m_strName;
    part.m_dwType = m_dwType;

    CPillarDBManager partDb;
    if (-1 == m_dwPillarId //添加时检查名称重复
        || m_strName != m_strOldName)//修改时检查名称是否有被修改过
    {
        if (!partDb.CheckName(m_strName))
        {
            AfxMessageBox(_T("支柱名称不能重复，请重新输入"));
            return;
        }
    }
    if (-1 == m_dwPillarId)
    {
        partDb.Add(part);
    }
    else
    {
        partDb.Update(part, m_dwPillarId); 
    }

    CDialog::OnOK();
}


BOOL CPillarPartDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    if (-1 == m_dwPillarId)//添加
    {
        SetWindowText(_T("添加支柱"));
    }
    else
    {
        PillarParam part;
        CPillarDBManager partDb;
        partDb.GetPillarInfoById(part, m_dwPillarId);
        m_strLen = part.m_strLength.c_str();
        m_strUp = part.m_strUp.c_str();
        m_strBelow = part.m_strBelow.c_str();
        m_strWidthOfUp = part.m_strWidthOfUp.c_str();
        m_strName = part.m_strName.c_str();
        m_strOldName = m_strName;
        m_dwType = part.m_dwType;
        UpdateData(FALSE);

        SetWindowText(_T("修改支柱"));   
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
