#pragma once

// CSiteDBManagerDlg 对话框

class CSiteDBManagerDlg : public CXTPDialog
{
	DECLARE_DYNAMIC(CSiteDBManagerDlg)

public:
	CSiteDBManagerDlg();
	virtual ~CSiteDBManagerDlg();

// 对话框数据
	enum { IDD = IDD_SITE_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

public:
    BOOL m_bStateAdd;//当前是添加还是修改
    DWORD m_dwSiteId;//要修改的站点的ID
    CXTPEdit m_edtSiteName;
    CString m_strSiteName;
    CString m_strOldName;

public:
    virtual void OnOK();
    virtual void OnCancel();
};
