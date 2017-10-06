#pragma once


#include "CListCtrlEx/ListCtrlEx.h"
#include "DB/DxfTitleDBManager.h"

// CDxfTitleSetDlg 对话框

class CDxfTitleSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CDxfTitleSetDlg)

public:
	CDxfTitleSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDxfTitleSetDlg();

// 对话框数据
	enum { IDD = IDD_DXFTITLE_SET_DLG };

public:
	CListCtrlEx m_listDxfTitle;
	CXTPHeaderCtrl m_headCtrl;
	CImageList m_imageList;

	CEdit m_edtTitle;
	CComboBox m_comboProperty;
	static BOOL EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	int m_curSelectListIndex;
	DWORD m_dwId;
	std::vector<DxfTitleCell> m_cellVector;

	void InitCombo(void);
	void InitList(void);
	void InsertList(int index, DxfTitleCell cell);
	void DeleteList(int index);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListTitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddBtn();
	afx_msg void OnBnClickedDelBtn();
	afx_msg void OnBnClickedSaveBtn();

	void Init(DWORD dwId = -1);
};
