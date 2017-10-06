#pragma once

#include <vector>
using namespace std;

// CNodeModifyDlg 对话框

class CNodeModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CNodeModifyDlg)

public:
	CNodeModifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNodeModifyDlg();

// 对话框数据
	enum { IDD = IDD_NODE_MODIFY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void initPartList(void);
	void initNodePartList(DWORD dwNodeId);
	void resetNodePartList(void);
	void addNodePartList(DWORD partId);
	void delNodePartList(size_t index);
	void saveToDB(DWORD dwNodeId);

private:
	CXTPListCtrl m_partList;
	CXTPListCtrl m_nodePartList;
	CXTPHeaderCtrl m_partHeaderCtrl;
	CXTPHeaderCtrl m_nodePartHeaderCtrl;
	CImageList m_partImageList;
	CImageList m_nodePartImageList;

	int m_curSelectPartListIndex;
	int m_curSelectNodePartListIndex;
	DWORD m_nodeId;
	CString m_nodeName;
	CString m_nodeWeight;
	vector<CString> m_nodePartIdVec;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickPartList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickNodePartList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedAddBtn();
	afx_msg void OnClickedDelBtn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeWeightEdit();

	void SetNodeId(DWORD dwId);
};
