#pragma once


// CPillarPartDlg �Ի���

class CPillarPartDlg : public CDialog
{
	DECLARE_DYNAMIC(CPillarPartDlg)

public:
	CPillarPartDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPillarPartDlg();

// �Ի�������
	enum { IDD = IDD_PILLARPARTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
