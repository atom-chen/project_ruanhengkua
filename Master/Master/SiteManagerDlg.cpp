// SiteManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "SiteManagerDlg.h"
#include "afxdialogex.h"
#include "DB/SiteDBManager.h"


// CSiteDBManagerDlg 对话框

IMPLEMENT_DYNAMIC(CSiteDBManagerDlg, CXTPDialog)

CSiteDBManagerDlg::CSiteDBManagerDlg()
	: CXTPDialog(CSiteDBManagerDlg::IDD)
    , m_strSiteName(_T(""))
{
    m_dwSiteId = -1;
    m_bStateAdd = TRUE;
}

CSiteDBManagerDlg::~CSiteDBManagerDlg()
{
}

void CSiteDBManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CXTPDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_SITENAME, m_edtSiteName);
    DDX_Text(pDX, IDC_EDIT_SITENAME, m_strSiteName);
}


BEGIN_MESSAGE_MAP(CSiteDBManagerDlg, CXTPDialog)
END_MESSAGE_MAP()


// CSiteDBManagerDlg 消息处理程序


BOOL CSiteDBManagerDlg::OnInitDialog()
{
    CXTPDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    if (m_bStateAdd)//添加
    {
        SetWindowText(_T("添加站点"));
    }
    else
    {
        m_strOldName = m_strSiteName;
        SetWindowText(_T("修改站点"));
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CSiteDBManagerDlg::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    UpdateData(TRUE);
    if (m_strSiteName.GetLength() > 128)
    {
        AfxMessageBox(_T("站点名称长度限制在5个字符以内"));
        return;
    }

    if (m_bStateAdd)
    {
        SoftCrossSite site;
        site.m_strName = m_strSiteName;
        CSiteDBManager dbSite;
        if (!dbSite.AddSite(site))
        {
            AfxMessageBox(_T("该站点名称已存在，不能添加"));
            return;
        }
    }
    else
    {
        if (m_strSiteName != m_strOldName)
        {
            SoftCrossSite site;
            site.m_strName = m_strSiteName;
            CSiteDBManager dbSite;
            if (!dbSite.UpdateSite(site, m_dwSiteId))
            {
                AfxMessageBox(_T("该站点名称已存在，不能添加"));
                return;
            }
        }
    }

    CXTPDialog::OnOK();
}


void CSiteDBManagerDlg::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类

    CXTPDialog::OnCancel();
}
