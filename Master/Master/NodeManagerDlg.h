#pragma once
#include "afxcmn.h"


// CNodeManagerDlg �Ի���

class CNodeManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CNodeManagerDlg)

public:
	CNodeManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNodeManagerDlg();

// �Ի�������
	enum { IDD = IDD_NODEMANAGERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CXTPListCtrl m_listNode;
    CXTPHeaderCtrl m_header;
    CImageList m_ImageList;

    void UpdateNodeList();
    afx_msg void OnBnClickedAdd();
    afx_msg void OnBnClickedEdit();
    afx_msg void OnBnClickedDel();
    virtual BOOL OnInitDialog();
    afx_msg void OnNMDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult);
};
