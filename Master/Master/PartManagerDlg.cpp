// PartManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "PartManagerDlg.h"
#include "afxdialogex.h"
#include "PartDlg.h"
#include "DB/PartDBManager.h"
#include "DataValidator.h"


// CPartManagerDlg 对话框

IMPLEMENT_DYNAMIC(CPartManagerDlg, CDialog)

CPartManagerDlg::CPartManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartManagerDlg::IDD, pParent)
    , m_strPartName(_T(""))
    , m_strWeight(_T(""))
    , m_strLen(_T(""))
{
    m_dwId = -1;
}

CPartManagerDlg::~CPartManagerDlg()
{
}

void CPartManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDTPARTNAME, m_strPartName);
    DDX_Control(pDX, IDC_COMBOTYPE, m_comboType);
    DDX_Text(pDX, IDC_EDTWEIGHT, m_strWeight);
    DDX_Text(pDX, IDC_EDTLENGTH, m_strLen);
}


BEGIN_MESSAGE_MAP(CPartManagerDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CPartManagerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPartManagerDlg 消息处理程序

void CPartManagerDlg::SetTypeData(const CString& strTypeName, DWORD dwData, DWORD dwFlag)
{
    m_strTypeName = strTypeName;
    m_dwTypeId = dwData;
    m_dwTypeFlag = dwFlag;
}

void CPartManagerDlg::SetPartId(DWORD dwId)
{
    m_dwId = dwId;
}

BOOL CPartManagerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_comboType.AddString(m_strTypeName);
    m_comboType.SetCurSel(0);
    m_comboType.SetItemData(0, m_dwTypeId);

    SetUIState();

    if (-1 == m_dwId)//添加
    {
        SetWindowText(_T("添加零件"));
    }
    else
    {
        PartParam part;
        CPartDBManager partDb;
        partDb.GetPartInfoById(part, m_dwId);
        m_strWeight = part.m_strWeight.c_str();
        m_strLen = part.m_strLength.c_str();
        m_strPartName = part.m_strName.c_str();
        UpdateData(FALSE);

        SetWindowText(_T("修改零件"));
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CPartManagerDlg::SetUIState()
{
    if (m_dwTypeFlag & PART_WEIGHT)
    {
        GetDlgItem(IDC_EDTWEIGHT)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_EDTWEIGHT)->EnableWindow(FALSE);
    }

    if (m_dwTypeFlag & PART_LENGTH)
    {
        GetDlgItem(IDC_EDTLENGTH)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_EDTLENGTH)->EnableWindow(FALSE);
    }
}

void CPartManagerDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);

    //验证数据合法性
    PartParam part;
    part.m_dwTypeId = m_dwTypeId;
    part.m_dwFlag = m_dwTypeFlag;
    part.m_strName = m_strPartName;

    if (m_dwTypeFlag & PART_WEIGHT)
    {
        if (!CDataValidator::IsUnsignFloat(m_strWeight))
        {
            AfxMessageBox("数据格式错误，请重新输入");
            return;
        }
        part.m_strWeight = m_strWeight;
    }

    if (m_dwTypeFlag & PART_LENGTH)
    {
        if (!CDataValidator::IsUnsignFloat(m_strLen))
        {
            AfxMessageBox("数据格式错误，请重新输入");
            return;
        }
        part.m_strLength = m_strLen;
    }

    CPartDBManager partDb;
    if (-1 == m_dwId)
    {
        partDb.Add(part);
    }
    else
    {
        part.m_dwId = m_dwId;
        partDb.Update(part, m_dwId);
    }

    CDialog::OnOK();
}
