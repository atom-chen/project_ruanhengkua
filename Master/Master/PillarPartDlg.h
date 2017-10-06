#pragma once


// CPillarPartDlg 对话框

class CPillarPartDlg : public CDialog
{
	DECLARE_DYNAMIC(CPillarPartDlg)

public:
	CPillarPartDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPillarPartDlg();

// 对话框数据
	enum { IDD = IDD_PILLARPARTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CString m_strLen;
    CString m_strUp;
    CString m_strBelow;
    CString m_strWidthOfUp;
    CString m_strName;
    DWORD   m_dwPillarId;
    CString m_strOldName;
    afx_msg void OnBnClickedOk();
    virtual BOOL OnInitDialog();
    int m_dwType;
};
