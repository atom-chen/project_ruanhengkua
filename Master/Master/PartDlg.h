#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "PartTypeManagerDlg.h"
#include <map>
using namespace std;


// CPartDlg 对话框

class CPartDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartDlg)

public:
	CPartDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPartDlg();

// 对话框数据
	enum { IDD = IDD_PARTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CXTPHeaderCtrl m_header;
    CXTPListCtrl m_listPart;
    CImageList m_ImageList;
    CComboBox m_comboTypeList;
    int m_nTypeSel;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedAdd();
    afx_msg void OnCbnSelchangeCombotype();
    void ResetTypeList(DWORD dwTypeId = -1);

private:
    DWORD GetTypeById(DWORD dwId);
    map<DWORD, DWORD>   m_mapIdType;//Id和类型的对应关系
public:
    afx_msg void OnBnClickedEdit();
    afx_msg void OnBnClickedDel();
    afx_msg void OnNMDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedAdd2();
};
