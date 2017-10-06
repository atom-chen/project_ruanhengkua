// PillarPartDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Master.h"
#include "PillarPartDlg.h"
#include "afxdialogex.h"
#include "DB/PillarDBManager.h"
#include "DataValidator.h"

// CPillarPartDlg �Ի���

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


// CPillarPartDlg ��Ϣ�������


void CPillarPartDlg::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    if (!CDataValidator::IsUnsignFloat(m_strLen)
        || !CDataValidator::IsUnsignFloat(m_strUp)
        || !CDataValidator::IsUnsignFloat(m_strBelow)
        || !CDataValidator::IsUnsignFloat(m_strWidthOfUp))
    {
        AfxMessageBox("���ݸ�ʽ��������������");
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
    if (-1 == m_dwPillarId //���ʱ��������ظ�
        || m_strName != m_strOldName)//�޸�ʱ��������Ƿ��б��޸Ĺ�
    {
        if (!partDb.CheckName(m_strName))
        {
            AfxMessageBox(_T("֧�����Ʋ����ظ�������������"));
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

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    if (-1 == m_dwPillarId)//���
    {
        SetWindowText(_T("���֧��"));
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

        SetWindowText(_T("�޸�֧��"));   
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
