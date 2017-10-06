#pragma once
#include "afxwin.h"


// CPartManagerDlg �Ի���

class CPartManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartManagerDlg)

public:
	CPartManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPartManagerDlg();

// �Ի�������
	enum { IDD = IDD_PARTMANAGERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CString m_strPartName;
    CComboBox m_comboType;

public:
    void SetTypeData(const CString& strTypeName, DWORD dwData, DWORD dwFlag);
    void SetPartId(DWORD dwId);
    DWORD   m_dwId;//��ǰID
    
private:
    void SetUIState();

private:
    CString m_strTypeName;//�������������
    DWORD   m_dwTypeId;//����ID
    DWORD   m_dwTypeFlag;//���ͱ�־
public:
    virtual BOOL OnInitDialog();
    CString m_strWeight;
    CString m_strLen;
    afx_msg void OnBnClickedOk();
};
