#pragma once
#include "afxwin.h"

// CSegDBManagerDlg �Ի���

class CSegDBManagerDlg : public CXTPDialog
{
	DECLARE_DYNAMIC(CSegDBManagerDlg)

public:
	CSegDBManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSegDBManagerDlg();

// �Ի�������
	enum { IDD = IDD_SEG_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CXTPComboBox m_comboSite;
    CXTPEdit m_edtSegName;
    CString m_strSegName;//EDIT�ؼ�

    CString m_strOldSegName;//�ɵ�����
    CString m_strSiteName;
    DWORD   m_dwSiteId;
    DWORD   m_dwSegId;
    BOOL    m_bStateAdd;//��ǰ����ӻ����޸�
    virtual BOOL OnInitDialog();
    virtual void OnOK();
};
