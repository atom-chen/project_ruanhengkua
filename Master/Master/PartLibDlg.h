#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPartLibDlg 对话框

class CPartLibDlg : public CXTPDialog
{
	DECLARE_DYNAMIC(CPartLibDlg)

public:
	CPartLibDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPartLibDlg();

// 对话框数据
	enum { IDD = IDD_PARTLIBDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CXTPTreeCtrl m_treePart;
    CXTPButton m_btnAdd;
    CXTPButton m_btnEdit;
    CXTPButton m_btnDel;
    afx_msg void OnBnClickedAddlib();
    afx_msg void OnBnClickedEditlib();
    afx_msg void OnBnClickedDellib();
    afx_msg void OnSelchangeListPartLib();
    afx_msg void OnDblclkListPartLib();
    CXTPCheckListBox m_listPartLib;
    afx_msg void OnTvnSelchangedParttree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkParttree(NMHDR *pNMHDR, LRESULT *pResult);
};
