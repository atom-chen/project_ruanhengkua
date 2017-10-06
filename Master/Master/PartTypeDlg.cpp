// NodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "PartTypeDlg.h"
#include "afxdialogex.h"
#include "DB/PartTypeDBManager.h"

// CPartTypeDlg 对话框
IMPLEMENT_DYNAMIC(CPartTypeDlg, CDialog)

CPartTypeDlg::CPartTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartTypeDlg::IDD, pParent)
    , m_strName(_T(""))
    , m_bWeight(TRUE)
    , m_bLength(TRUE)
{
    m_dwId = -1;
}

CPartTypeDlg::~CPartTypeDlg()
{
}

void CPartTypeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDITNAME, m_strName);
    DDX_Check(pDX, IDC_CHKWEIGHT, m_bWeight);
    DDX_Check(pDX, IDC_CHKHEIGHT, m_bLength);
}


BEGIN_MESSAGE_MAP(CPartTypeDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CPartTypeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPartTypeDlg 消息处理程序

BOOL CPartTypeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    // TODO:  在此添加额外的初始化
    if (-1 == m_dwId)//添加
    {
        SetWindowText(_T("添加零件类型"));
        GetDlgItem(IDC_EDITNAME)->EnableWindow(TRUE);
    }
    else
    {
        SetWindowText(_T("修改零件类型"));
        GetDlgItem(IDC_EDITNAME)->EnableWindow(FALSE);

        CPartTypeDBManager db;
        PartType type;

        if (db.GetTypeById(type, m_dwId))
        {
            m_bWeight = type.m_dwFlag & PART_WEIGHT;
            m_bLength = type.m_dwFlag & PART_LENGTH;

            m_strName = type.m_strName;

            UpdateData(FALSE);
        }
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CPartTypeDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    PartType type;

    type.m_dwFlag = 0;
    if (m_bLength)
    {
        type.m_dwFlag |= PART_LENGTH;
    }
    if (m_bWeight)
    {
        type.m_dwFlag |= PART_WEIGHT;
    }

    CPartTypeDBManager db;
    if (-1 == m_dwId)//添加
    {
        if (db.CheckName(m_strName))
        {
            type.m_strName = m_strName;
            db.Add(type);
        }
        else
        {
            AfxMessageBox(_T("零件类型名称不能重复"));
            return;
        }
    }
    else
    {
        db.Update(type, m_dwId);
    }
    CDialog::OnOK();
}