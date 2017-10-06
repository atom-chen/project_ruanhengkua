// SiteManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Master.h"
#include "SiteManagerDlg.h"
#include "afxdialogex.h"
#include "DB/SiteDBManager.h"


// CSiteDBManagerDlg �Ի���

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


// CSiteDBManagerDlg ��Ϣ�������


BOOL CSiteDBManagerDlg::OnInitDialog()
{
    CXTPDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    if (m_bStateAdd)//���
    {
        SetWindowText(_T("���վ��"));
    }
    else
    {
        m_strOldName = m_strSiteName;
        SetWindowText(_T("�޸�վ��"));
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CSiteDBManagerDlg::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    UpdateData(TRUE);
    if (m_strSiteName.GetLength() > 128)
    {
        AfxMessageBox(_T("վ�����Ƴ���������5���ַ�����"));
        return;
    }

    if (m_bStateAdd)
    {
        SoftCrossSite site;
        site.m_strName = m_strSiteName;
        CSiteDBManager dbSite;
        if (!dbSite.AddSite(site))
        {
            AfxMessageBox(_T("��վ�������Ѵ��ڣ��������"));
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
                AfxMessageBox(_T("��վ�������Ѵ��ڣ��������"));
                return;
            }
        }
    }

    CXTPDialog::OnOK();
}


void CSiteDBManagerDlg::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���

    CXTPDialog::OnCancel();
}
