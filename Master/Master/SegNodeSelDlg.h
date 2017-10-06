#pragma once

#include "DB/SegParamDBManager.h"
#include "SegParamFormatter.h"
#include "CListCtrlEx/ListCtrlEx.h"
#include "afxwin.h"
#include <map>
using namespace std;
// CSegNodeSelDlg 对话框

class CSegNodeSelDlg : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(CSegNodeSelDlg)

public:
	CSegNodeSelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSegNodeSelDlg();

// 对话框数据
	enum { IDD = IDD_SEGNODESELDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    SoftCrossSegParam* m_pSegParam;
    CListCtrlEx m_listQ;
    CXTPHeaderCtrl m_header;
    virtual BOOL OnInitDialog();
    void UpdateQ(int nIndex);
    virtual BOOL OnSetActive();
    virtual LRESULT OnWizardBack();
    virtual BOOL OnWizardFinish();

private:
    CImageList m_ImageList;
    CSegParamFormatter m_formatter;
    map<CString, DWORD> m_mapNodeNameId;
    map<CString, DWORD> m_mapCatenaryNameId;
    map<CString, DWORD> m_mapTouchLineNameId;

    void InitNodeListAndMap();
    void InitCatenaryListAndMap();
    void InitTouchLineListAndMap();
    DWORD GetIdByName(map<CString, DWORD>& mapNameId, const CString& strName);
    BOOL DoSave();
public:
    static BOOL InitEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
    static BOOL EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
    CComboBox m_comboNode;
    CComboBox m_comboHang;
    CComboBox m_comboCatenary;
    CComboBox m_comboTouchLine;
    CEdit m_edtDistance;
    afx_msg void OnBnClickedNodeset();
};
