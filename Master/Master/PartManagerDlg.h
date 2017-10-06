#pragma once
#include "afxwin.h"


// CPartManagerDlg 对话框

class CPartManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartManagerDlg)

public:
	CPartManagerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPartManagerDlg();

// 对话框数据
	enum { IDD = IDD_PARTMANAGERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CString m_strPartName;
    CComboBox m_comboType;

public:
    void SetTypeData(const CString& strTypeName, DWORD dwData, DWORD dwFlag);
    void SetPartId(DWORD dwId);
    DWORD   m_dwId;//当前ID
    
private:
    void SetUIState();

private:
    CString m_strTypeName;//所属零件类型名
    DWORD   m_dwTypeId;//类型ID
    DWORD   m_dwTypeFlag;//类型标志
public:
    virtual BOOL OnInitDialog();
    CString m_strWeight;
    CString m_strLen;
    afx_msg void OnBnClickedOk();
};
