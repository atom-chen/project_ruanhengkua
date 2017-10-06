#pragma once


#include <vector>
// CDxfTitleDlg 对话框

class CDxfTitleDlg : public CDialog
{
	DECLARE_DYNAMIC(CDxfTitleDlg)

public:
	CDxfTitleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDxfTitleDlg();

// 对话框数据
	enum { IDD = IDD_DXFTITLE_DLG };

public:
	CXTPListCtrl m_listDxfTitle;
	CXTPHeaderCtrl m_headCtrl;
	CImageList m_imageList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	void ResetDxfTitleList();

private:
	int m_curSelectListIndex;
	std::vector<std::string> m_filenames;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListDxfTitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListDxfTitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedParse();

	void SetFilenames(std::vector<std::string> filenames);
};
