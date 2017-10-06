// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "WorkArea.h"
//#include "TrueColorToolbar/TrueColorToolBar.h"

typedef struct _BMPINFO
{
    TCHAR*  fileName;               //文件名
    TCHAR*  descript;               //工具栏图片的说明文字
}BmpInfo;

class CSplitterWndEx : public CXTPSplitterWnd
{
public:
	CSplitterWndEx()
	{
		m_cyBorder = m_cxBorder = 0;
		m_cxSplitter = m_cySplitter = 4;
		m_cySplitterGap = m_cxSplitterGap = 4;
	}

};

class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	//CSplitterWndEx m_wndSplitter;
	CSplitterWndEx m_wndSplitterCaption;
	//CSplitterWndEx m_wndSplitterWorkspace;
	//CSplitterWndEx m_wndSplitterPreview;
	CXTPCaption       m_wndCaption;
	CFont            m_fontCaption;

	CXTPStatusBar  m_wndStatusBar;
	CXTPDockingPaneManager m_paneManager;
    //CXTPImageManager m_ImageManager;
	CXTPOfficeBorder<CStatic> m_wndOptions;
	CXTPOfficeBorder<CEdit> m_wndProperties;

    //CImageList  m_ImgToolbar;
	CImageList m_ilTreeIcons;
	CXTPTreeCtrl m_wndTreeCtrl;

	CWorkArea  * m_pWorkArea;
	static DWORD CurSegId;
    //CTrueColorToolBar m_TrueColorToolBar;
    
private:
	BOOL m_isPreSelectSegment;
	
// Operations
public:
	BOOL InitializeCaptionBar();


// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
    afx_msg void OnRBClickTree(NMHDR*   pNMHDR,   LRESULT*   pResult);
	DECLARE_MESSAGE_MAP()

private:
    HTREEITEM GetTreeItemBySite(DWORD dwSiteId);
    HTREEITEM GetTreeItemBySeg(HTREEITEM hSiteItem, DWORD dwSegId);
    DWORD GetSelTreeData();
    void RefreshSiteTree();
	void initPartConnectDb(DWORD segId);
    HTREEITEM m_hSiteRootItem;

public:
	static CArray<CXTPDockingPaneManager*, CXTPDockingPaneManager*> m_arrManagers;
    afx_msg void OnSetSegParam();
    afx_msg void OnSectionManager();
    afx_msg void OnPartLibManager();
    afx_msg void OnPartManager();
    afx_msg void OnNodeManager();
    afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
#ifndef GRIDCONTROL_NO_CLIPBOARD
    afx_msg void OnEditPaste();
    afx_msg void OnEditCopy();
    afx_msg void OnEditCut();
    afx_msg void OnUpdateEditCopyOrCut(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
#endif
    afx_msg void OnAdd();
    afx_msg void OnEdit();
    afx_msg void OnDel();
	afx_msg void OnDxfParse();
    afx_msg void OnUpdateAdd(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEdit(CCmdUI *pCmdUI);
    afx_msg void OnUpdateDel(CCmdUI *pCmdUI);
};
