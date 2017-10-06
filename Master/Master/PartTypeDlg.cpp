// NodeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Master.h"
#include "PartTypeDlg.h"
#include "afxdialogex.h"
#include "DB/PartTypeDBManager.h"

// CPartTypeDlg �Ի���
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


// CPartTypeDlg ��Ϣ�������

BOOL CPartTypeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    if (-1 == m_dwId)//���
    {
        SetWindowText(_T("����������"));
        GetDlgItem(IDC_EDITNAME)->EnableWindow(TRUE);
    }
    else
    {
        SetWindowText(_T("�޸��������"));
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
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CPartTypeDlg::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
    if (-1 == m_dwId)//���
    {
        if (db.CheckName(m_strName))
        {
            type.m_strName = m_strName;
            db.Add(type);
        }
        else
        {
            AfxMessageBox(_T("����������Ʋ����ظ�"));
            return;
        }
    }
    else
    {
        db.Update(type, m_dwId);
    }
    CDialog::OnOK();
}