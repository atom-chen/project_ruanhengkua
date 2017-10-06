// SegManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "SegManagerDlg.h"
#include "afxdialogex.h"
#include "DB/SiteDBManager.h"
#include "DB/SegDBManager.h"
#include "DB/SegParamDBManager.h"
#include <vector>
using namespace std;


// CSegDBManagerDlg 对话框

IMPLEMENT_DYNAMIC(CSegDBManagerDlg, CXTPDialog)

CSegDBManagerDlg::CSegDBManagerDlg(CWnd* pParent /*=NULL*/)
	: CXTPDialog(CSegDBManagerDlg::IDD)
    , m_strSegName(_T(""))
{
    m_bStateAdd = TRUE;
    m_dwSegId = 0;
    m_dwSiteId = 0;
}

CSegDBManagerDlg::~CSegDBManagerDlg()
{
}

void CSegDBManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CXTPDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBOSITE, m_comboSite);
    DDX_Control(pDX, IDC_EDITSEGNAME, m_edtSegName);
    DDX_Text(pDX, IDC_EDITSEGNAME, m_strSegName);
}


BEGIN_MESSAGE_MAP(CSegDBManagerDlg, CXTPDialog)
END_MESSAGE_MAP()


// CSegDBManagerDlg 消息处理程序



BOOL CSegDBManagerDlg::OnInitDialog()
{
    CXTPDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_comboSite.ResetContent();
    m_comboSite.AddString(m_strSiteName);
    m_comboSite.SelectString(0, m_strSiteName);
    m_comboSite.EnableWindow(FALSE);

    if (!m_bStateAdd)
    {
        m_strOldSegName = m_strSegName;
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CSegDBManagerDlg::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    UpdateData(TRUE);
    if (m_strSiteName.GetLength() > 128)
    {
        AfxMessageBox(_T("分段名称长度限制在5个字符以内"));
        return;
    }

    if (m_bStateAdd)
    {
        SoftCrossSeg seg;
        seg.m_dwSiteId = m_dwSiteId;
        seg.m_strName = m_strSegName;
        CSegDBManager dbSeg;
        if (!dbSeg.Add(seg))
        {
            AfxMessageBox(_T("该分段名称已存在，不能添加"));
            return;
        }
        //添加分段参数信息
        DWORD dwSegId = dbSeg.GetInsertId();
        m_dwSegId = dwSegId;
    }
    else
    {
        if (m_strSegName != m_strOldSegName)
        {
            SoftCrossSeg seg;
            seg.m_strName = m_strSegName;
            CSegDBManager dbSeg;
            if (!dbSeg.Update(seg, m_dwSegId))
            {
                AfxMessageBox(_T("该分段名称已存在，不能修改"));
                return;
            }
        }
    }

    CXTPDialog::OnOK();
}
