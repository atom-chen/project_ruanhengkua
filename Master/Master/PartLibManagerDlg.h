#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPartLibManagerDlg 对话框

class CPartLibManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartLibManagerDlg)

public:
	CPartLibManagerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPartLibManagerDlg();

// 对话框数据
	enum { IDD = IDD_PARTLIBMANAGERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
