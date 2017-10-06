#pragma once
#include "afxwin.h"

// CSegDBManagerDlg 对话框

class CSegDBManagerDlg : public CXTPDialog
{
	DECLARE_DYNAMIC(CSegDBManagerDlg)

public:
	CSegDBManagerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSegDBManagerDlg();

// 对话框数据
	enum { IDD = IDD_SEG_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CXTPComboBox m_comboSite;
    CXTPEdit m_edtSegName;
    CString m_strSegName;//EDIT控件

    CString m_strOldSegName;//旧的名称
    CString m_strSiteName;
    DWORD   m_dwSiteId;
    DWORD   m_dwSegId;
    BOOL    m_bStateAdd;//当前是添加还是修改
    virtual BOOL OnInitDialog();
    virtual void OnOK();
};
