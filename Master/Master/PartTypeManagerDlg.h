#pragma once
#include "afxcmn.h"

#include "PartTypeDlg.h"

// CPartTypeManagerDlg �Ի���

class CPartTypeManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartTypeManagerDlg)

public:
	CPartTypeManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPartTypeManagerDlg();

// �Ի�������
	enum { IDD = IDD_PARTTYPEMANAGERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CXTPListCtrl m_listNode;
    CXTPHeaderCtrl m_header;
    CImageList m_ImageList;

    void UpdateList();
    afx_msg void OnBnClickedAdd();
    afx_msg void OnBnClickedEdit();
    afx_msg void OnBnClickedDel();
    virtual BOOL OnInitDialog();
    afx_msg void OnNMDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult);
};
