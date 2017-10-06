#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPartLibManagerDlg �Ի���

class CPartLibManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartLibManagerDlg)

public:
	CPartLibManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPartLibManagerDlg();

// �Ի�������
	enum { IDD = IDD_PARTLIBMANAGERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CTreeCtrl m_treeNonSel;
    CTreeCtrl m_treeSel;
    CEdit m_edtLibName;
    CString m_strLibName;
    afx_msg void OnBnClickedSinglesel();
    afx_msg void OnBnClickedSingleunsel();
    afx_msg void OnBnClickedOk();
};
