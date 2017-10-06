#pragma once

#include <vector>
using namespace std;

// CPartConnectModifyDlg �Ի���

class CPartConnectModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartConnectModifyDlg)

public:
	CPartConnectModifyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPartConnectModifyDlg();

// �Ի�������
	enum { IDD = IDD_PART_CONNECT_MODIFY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	void initPartList(DWORD dwId);
	void resetPartList(void);
	void addPartList(DWORD partId);
	void delPartList(size_t index);
	void insertPartList(size_t index, DWORD partId);
	void updatePartList(size_t index, DWORD partId);
	void saveToDB(void);

	void initPartTypeComboBox(void);
	void setSelectPartTypeComboBox(DWORD partTypeId);
	void resetPartComboBox(DWORD partTypeId, DWORD partId);

private:
	CXTPListCtrl m_partList;
	CXTPHeaderCtrl m_headerCtrl;
	CImageList m_imageList;
	CComboBox m_partTypeBox;
	CComboBox m_partBox;

	int m_curSelectPartListIndex;
	DWORD m_partConnectId;
	vector<CString> m_partIdVec;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickPartList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangePartTypeCombo();
	afx_msg void OnBnClickedAddBtn();
	afx_msg void OnBnClickedDelBtn();
	afx_msg void OnBnClickedInsertBtn();
	afx_msg void OnBnClickedOk();
	
	void SetPartConnectId(DWORD dwId);
};
