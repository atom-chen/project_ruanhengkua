#pragma once


// CPartConnectDlg �Ի���

class CPartConnectDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CPartConnectDlg)

public:
	CPartConnectDlg();
	virtual ~CPartConnectDlg();

// �Ի�������
	enum { IDD = IDD_PART_CONNECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	void openModifyDlg(int dwId);
	void updateList(void);

private:
	CXTPListCtrl m_listPart;
	CXTPHeaderCtrl m_headCtrl;
	CImageList m_imageList;

	int m_curSelectListIndex;

public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnClickListpart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedModifyBtn();
};
