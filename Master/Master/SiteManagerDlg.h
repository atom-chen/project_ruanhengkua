#pragma once

// CSiteDBManagerDlg �Ի���

class CSiteDBManagerDlg : public CXTPDialog
{
	DECLARE_DYNAMIC(CSiteDBManagerDlg)

public:
	CSiteDBManagerDlg();
	virtual ~CSiteDBManagerDlg();

// �Ի�������
	enum { IDD = IDD_SITE_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

public:
    BOOL m_bStateAdd;//��ǰ����ӻ����޸�
    DWORD m_dwSiteId;//Ҫ�޸ĵ�վ���ID
    CXTPEdit m_edtSiteName;
    CString m_strSiteName;
    CString m_strOldName;

public:
    virtual void OnOK();
    virtual void OnCancel();
};
