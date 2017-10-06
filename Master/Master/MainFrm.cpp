// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Master.h"
#include "MainFrm.h"
#include "WorkArea.h"
#include "DB/DBInit.h"
#include "DB/PartTypeManager.h"
#include "DB/SiteDBManager.h"
#include "DB/SegDBManager.h"
#include "DB/PartDBManager.h"
#include "DB/PillarDBManager.h"
#include "DB/NodeDBManager.h"
#include "DB/SegInputDBManager.h"
#include "DB/SegOutDBManager.h"
#include "DB/PartTypeDBManager.h"
#include "DB/PartConnectDBManager.h"
#include "DB/DxfTitleDBManager.h"
#include "SiteManagerDlg.h"
#include "SegManagerDlg.h"
#include "SegPartSelDlg.h"
#include "SegNodeSelDlg.h"
#include "PartLibDlg.h"
#include "PartDlg.h"
#include "NodeManagerDlg.h"
#include "PartConnectDlg.h"
#include "DxfTitleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_SITE_TREE 101

#define TREE_SITE_FLAG  0x80000000
#define TREE_SEG_FLAG   0x40000000

// CMainFrame
CArray<CXTPDockingPaneManager*, CXTPDockingPaneManager*> CMainFrame::m_arrManagers;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
    ON_NOTIFY(NM_RCLICK, ID_SITE_TREE, CMainFrame::OnRBClickTree)
    ON_NOTIFY(TVN_SELCHANGED, ID_SITE_TREE, &CMainFrame::OnTvnSelchangedTree)
    ON_COMMAND(IDM_ADD, &CMainFrame::OnAdd)
    ON_COMMAND(IDM_EDIT, &CMainFrame::OnEdit)
    ON_COMMAND(IDM_DEL, &CMainFrame::OnDel)
	ON_COMMAND(IDM_DXF_PARSE, &CMainFrame::OnDxfParse)
    ON_COMMAND(IDM_SET_SEGPARAM, &CMainFrame::OnSetSegParam)
    ON_COMMAND(IDM_SECTIONMANAGER, &CMainFrame::OnSectionManager)
    ON_COMMAND(IDM_PARTLIB, &CMainFrame::OnPartLibManager)
    ON_COMMAND(IDM_PART, &CMainFrame::OnPartManager)
    ON_COMMAND(IDM_NODEMANAGER, &CMainFrame::OnNodeManager)
#ifndef GRIDCONTROL_NO_CLIPBOARD
    ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
    ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
    ON_COMMAND(ID_EDIT_CUT, &CMainFrame::OnEditCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopyOrCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCopyOrCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
#endif
    ON_UPDATE_COMMAND_UI(IDM_ADD, &CMainFrame::OnUpdateAdd)
    ON_UPDATE_COMMAND_UI(IDM_EDIT, &CMainFrame::OnUpdateEdit)
    ON_UPDATE_COMMAND_UI(IDM_DEL, &CMainFrame::OnUpdateDel)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT uHideCmds[] =
{
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};

#define PANEID_NAVIGATION 10

// CMainFrame construction/destruction
DWORD CMainFrame::CurSegId = -1;

CMainFrame::CMainFrame()
:m_isPreSelectSegment(FALSE)
{
    // TODO: add member initialization code here
    CDBInit::InitDbPath();

    CPartDBManager::InitDb();//初始化数据库
    CPillarDBManager::InitDb();
    CSiteDBManager::InitDb();
    CSegDBManager::InitDb();
    CNodeDBManager::InitDb();
    CSegParamDBManager::InitDb();
    CSegInputDBManager::InitDb();
    CSegOutDBManager::InitDb();
    CPartTypeDBManager::InitDb();
    CPartTypeManager::InitPartType();
	CPartConnectDBManager::InitDb();
	DxfTitleDBManager::InitDb();
}

CMainFrame::~CMainFrame()
{
    CDBInit::Free();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    //CXTPPaintManager::SetTheme(xtpThemeVisualStudio2010);

	// Initialize the command bars

	if (!InitCommandBars())
		return -1;
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}
    
	// Add the menu bar
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
		_T("Menu Bar"), IDR_MAINMENU);//IDR_MAINFRAME
	if(pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

//     m_TrueColorToolBar.SetBorders(1, 1, 1, 1);
// 
//     if (!m_TrueColorToolBar.Create(this, WS_CHILD | 
//         WS_VISIBLE | CBRS_TOP  | CBRS_GRIPPER | 
//         CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
//         !m_TrueColorToolBar.LoadToolBar(IDR_MAINTOOLBAR))
//     {
//         TRACE0("Failed to create toolbar\n");
//         return -1; //Failed to create
//     }
// 
//     m_TrueColorToolBar.ModifyStyle(0, TBSTYLE_FLAT);//Fix for WinXP
//     m_TrueColorToolBar.LoadTrueColorToolBar(
//         32,
//         IDB_TOOLBARLIGHT,
//         IDB_TOOLBARLIGHT,
//         IDB_TOOLBARGRAY1);

	// Create ToolBar
	CXTPToolBar* pToolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("工具栏"), xtpBarTop);
	if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
    }

    //设置工具栏图标
    XTPImageManager()->SetIcons(IDR_MAINTOOLBAR, IDB_TOOLBARLIGHT, xtpImageDisabled);
    XTPImageManager()->SetIcons(IDR_MAINTOOLBAR, IDB_TOOLBARLIGHT, xtpImageNormal);
    XTPImageManager()->SetIcons(IDR_MAINTOOLBAR, IDB_TOOLBARLIGHT, xtpImageHot);
 
	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	// Load the previous state for toolbars and menus.
	//LoadCommandBars(_T("CommandBars"));
    
	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.

	m_paneManager.InstallDockingPanes(this);
	//m_paneManager.SetTheme(xtpPaneThemeOffice2007Word);

	m_arrManagers.Add(&m_paneManager);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		PANEID_NAVIGATION, CRect(0, 0,200, 120), xtpPaneDockLeft);

	pwndPane1->SetTitle(_T("项目列表"));

	const int  nIDs[] = {PANEID_NAVIGATION};
	m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDs, 1);

	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR8, 0, 1);

	CBitmap bmp;
	bmp.LoadBitmap(IDR_TREE_ICONS);

	m_ilTreeIcons.Add(&bmp, RGB(255, 0, 255));

    XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);//该函数的调用用于设置应用的哪些部分需要使用皮肤。
    XTPSkinManager()->LoadSkin(_T("Styles\\WinXP.Royale.cjstyles"));
    //CXTPPaintManager::SetTheme(xtpThemeVisualStudio2010);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnClose()
{

	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	// Save the current state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

	CFrameWnd::OnClose();
}


void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
// 	CXTPCommandBars* pCommandBars = GetCommandBars();
// 	if(pCommandBars != NULL)
// 	{
// 		// Instanciate the customize dialog object.
// 		CXTPCustomizeSheet dlg(pCommandBars);
// 
// 		// Add the keyboard page to the customize dialog.
// 		CXTPCustomizeKeyboardPage pageKeyboard(&dlg);
// 		dlg.AddPage(&pageKeyboard);
// 		pageKeyboard.AddCategories(IDR_MAINFRAME);
// 
// 		// Add the options page to the customize dialog.
// 		CXTPCustomizeOptionsPage pageOptions(&dlg);
// 		dlg.AddPage(&pageOptions);
// 
// 		// Add the commands page to the customize dialog.
// 		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
// 		pCommands->AddCategories(IDR_MAINFRAME);
// 
// 		// Use the command bar manager to initialize the
// 		// customize dialog.
// 		pCommands->InsertAllCommandsCategory();
// 		pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
// 		pCommands->InsertNewMenuCategory();
// 
// 		// Dispaly the dialog.
// 		dlg.DoModal();
// 	}
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		ASSERT(pPane->GetID() == PANEID_NAVIGATION);

		if (!pPane->IsValid())
		{
			// create and attach the edit control for this pane.
			if (!::IsWindow(m_wndTreeCtrl.m_hWnd))
			{
				// create the tree control.
				if (!m_wndTreeCtrl.Create (WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | TVS_HASLINES
                    | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_CHECKBOXES 
					, CRect(0,0,0,0), this, ID_SITE_TREE))
				{
					TRACE0( "Unable to create tree control.\n" );
					return -1;
                }
                m_wndTreeCtrl.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);
                //m_wndTreeCtrl.InsertItem(_T("工程项目列表"), 0, 0);
                m_hSiteRootItem = m_wndTreeCtrl.InsertItem(_T("车站列表"), 0, 0);
                m_wndTreeCtrl.SetItemData(m_hSiteRootItem, 0);

                RefreshSiteTree();
			}

			pPane->Attach(&m_wndTreeCtrl);
		}

		return TRUE; // handled
	}


/*
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
			case IDR_PANE_PROPERTIES:
				{
					if (m_wndProperties.GetSafeHwnd() == 0)
					{
						m_wndProperties.Create(WS_CHILD|
							ES_AUTOVSCROLL|ES_MULTILINE,
							CRect(0, 0, 0, 0), this, 0);
					}
					pPane->Attach(&m_wndProperties);
					break;
				}
			case IDR_PANE_OPTIONS:
				{
					if (m_wndOptions.GetSafeHwnd() == 0)
					{
						m_wndOptions.Create(_T("\n\nOptions"),
							WS_CHILD|WS_CLIPCHILDREN|
							WS_CLIPSIBLINGS|SS_CENTER,
							CRect(0, 0, 0, 0), this, 0);
					}
					pPane->Attach(&m_wndOptions);
					break;
				}
			}
		}
		return TRUE;
	}*/
	return FALSE;
}

void CMainFrame::RefreshSiteTree()
{
    HTREEITEM hItem = m_wndTreeCtrl.GetChildItem(m_hSiteRootItem);//先删除子树
    while(NULL != hItem)
    {
        HTREEITEM hItemTemp = hItem;
        hItem = m_wndTreeCtrl.GetNextSiblingItem(hItemTemp);
        m_wndTreeCtrl.DeleteItem(hItemTemp);//清空
    }

    vector<SoftCrossSite> vecSite;//重新查询数据库
    CSiteDBManager siteManager;
    CSegDBManager segManager;
    siteManager.GetAllSite(vecSite);
    int nSize = (int)vecSite.size();

    for (int i = 0; i < nSize; i++)
    {
        HTREEITEM hItemSite = m_wndTreeCtrl.InsertItem(vecSite[i].m_strName, i, i, m_hSiteRootItem);
        m_wndTreeCtrl.SetItemData(hItemSite, vecSite[i].m_dwSiteId | TREE_SITE_FLAG);//第一位为1代表站点，为0代表分段

        vector<SoftCrossSeg> vecSeg;
        segManager.GetAllSegmentBySid(vecSeg, vecSite[i].m_dwSiteId);
        int nSizeSeg = (int)vecSeg.size();
        for (int j = 0; j < nSizeSeg; j++)
        {
            HTREEITEM hItemSeg = m_wndTreeCtrl.InsertItem(vecSeg[j].m_strName, j, j, hItemSite);
            m_wndTreeCtrl.SetItemData(hItemSeg, vecSeg[j].m_dwSegId | TREE_SEG_FLAG);
        }
        m_wndTreeCtrl.Expand (hItemSite, TVE_EXPAND);
    }

    m_wndTreeCtrl.Expand (m_hSiteRootItem, TVE_EXPAND);
}
// CMainFrame message handlers

BOOL CMainFrame::InitializeCaptionBar()
{
	if (!m_wndCaption.Create(&m_wndSplitterCaption, _T("中铁七局接触网工程计算软件"), NULL,
		WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, CRect(0,0,0,0), m_wndSplitterCaption.IdFromRowCol(0, 0)))
	{
		TRACE0("Failed to caption window.\n");
		return FALSE;
	}

	//HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//图标

	// set the caption colors.
	m_wndCaption.SetCaptionColors (GetXtremeColor(COLOR_3DFACE),
		GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_WINDOW) );

	LOGFONT lf;
	XTPAuxData().fontBold.GetLogFont(&lf);
	lf.lfHeight = 18;
	m_fontCaption.CreateFontIndirect(&lf);

	// set the font and initial icon.
	m_wndCaption.ModifyCaptionStyle(0,
		&m_fontCaption, NULL);//, hIcon);

	// Sizing for splitter
	m_wndSplitterCaption.SetRowInfo (0, 22, 0);

	return TRUE;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* /*pContext*/)
{
	// TODO: Add your specialized code here and/or call the base class
	m_wndSplitterCaption.CreateStatic(this, 2, 1);
	m_wndSplitterCaption.ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	if (!InitializeCaptionBar())
		return FALSE;

	//m_pPreviewFrame = (CPreviewFrame*)CPreviewFrame::CreateObject();
	m_pWorkArea = (CWorkArea*)CWorkArea::CreateObject();
	m_pWorkArea->CreateEx(NULL, NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndSplitterCaption, m_wndSplitterCaption.IdFromRowCol(1, 0));

	m_wndSplitterCaption.SetSplitterStyle(XTP_SPLIT_NOFULLDRAG | XTP_SPLIT_NOSIZE | XTP_SPLIT_NOBORDER);

	return TRUE;
}

void CMainFrame::OnRBClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    CPoint point;  
    GetCursorPos(&point);  
    CPoint pointInTree = point;
    m_wndTreeCtrl.ScreenToClient(&pointInTree); 
    HTREEITEM hItem;  
    UINT uFlag;  
    hItem = m_wndTreeCtrl.HitTest(pointInTree, &uFlag);//判断鼠标在treectrl的text上
    if(hItem != NULL && uFlag & TVHT_ONITEMLABEL)  
    {  
        m_wndTreeCtrl.SelectItem(hItem);
        DWORD dwData = m_wndTreeCtrl.GetItemData(hItem);
        DWORD dwSiteId = dwData & ~TREE_SITE_FLAG;

        CMenu menu;
        if (0 == dwData)//根节点
        {
            menu.LoadMenu(IDR_ROOTSITEMENU);
        }
        else if(dwData & TREE_SITE_FLAG)//TRUE表示站点
        {
            menu.LoadMenu(IDR_SITEPOPMENU);
        }
        else if(dwData & TREE_SEG_FLAG)//分段
        {
            menu.LoadMenu(IDR_SEGPOPMENU);
        }
        menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x,  point.y, this, NULL);
    }
}

void CMainFrame::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    HTREEITEM hSel = pNMTreeView->itemNew.hItem;	// 鼠标所选择的树形控件节点
    if (NULL == hSel)
        return;
    
    DWORD dwData = m_wndTreeCtrl.GetItemData(hSel);
	DWORD dwSegId = dwData & ~TREE_SEG_FLAG;
	CurSegId = dwSegId;

	// 设置软跨页
	GridCtrl *grid = m_pWorkArea->getSoftCrossGrid();
	if (dwData & TREE_SEG_FLAG)
	{
		initPartConnectDb(dwSegId);
		m_isPreSelectSegment = TRUE;
		m_pWorkArea->getSoftCrossPage()->init(grid, dwSegId, 0 == m_pWorkArea->getSelTabIndex());
		m_pWorkArea->drawGraph(dwSegId);
	}
	else
	{
		if (m_isPreSelectSegment)
		{
			m_isPreSelectSegment = FALSE;
			m_pWorkArea->getSoftCrossPage()->init(grid, -1, 0 == m_pWorkArea->getSelTabIndex());
			m_pWorkArea->drawGraph(-1);
		}
	}

    *pResult = 0;
}

//设置分段的属性
void CMainFrame::OnSetSegParam()
{
    // TODO: 在此添加命令处理程序代码
    HTREEITEM hSel = m_wndTreeCtrl.GetSelectedItem();
    if (NULL == hSel)
    {
        AfxMessageBox(_T("请选择分段设置参数"));
        return;
    }
    DWORD dwData = m_wndTreeCtrl.GetItemData(hSel);
    DWORD dwSegId = dwData & ~TREE_SEG_FLAG;

    //获取数据库分段的参数信息
    SoftCrossSegParam segParam;
    CSegParamDBManager paramDB;
    paramDB.GetParamBySegId(segParam, dwSegId);

    CSegPartSelDlg dlgPartSel;
    dlgPartSel.m_pSegParam = &segParam;
	CPartConnectDlg dlgPartConnect;
    CSegNodeSelDlg dlgNodeSel;
    dlgNodeSel.m_pSegParam = &segParam;

    CPropertySheet theWizzard;
    theWizzard.AddPage(&dlgPartSel);
	theWizzard.AddPage(&dlgPartConnect);
    theWizzard.AddPage(&dlgNodeSel);

    theWizzard.SetWizardMode();   //将属性表设置成向导样式
    theWizzard.SetActivePage(&dlgPartSel); //设置第一页为第一步
    theWizzard.DoModal(); //显示属性表

    HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
    HTREEITEM hParent = m_wndTreeCtrl.GetParentItem(hItem);
    m_wndTreeCtrl.SelectItem(hParent);
    m_wndTreeCtrl.SelectItem(hItem);//模拟发送消息
}

void CMainFrame::OnSectionManager()
{
    // TODO: 在此添加命令处理程序代码
}

void CMainFrame::OnPartLibManager()
{
    // TODO: 在此添加命令处理程序代码
    CPartLibDlg dlg;
    dlg.DoModal();
}

void CMainFrame::OnPartManager()
{
    // TODO: 在此添加命令处理程序代码
    CPartDlg dlg;
    dlg.DoModal();
}

void CMainFrame::OnNodeManager()
{
    // TODO: 在此添加命令处理程序代码
    CNodeManagerDlg dlg;
    dlg.DoModal();
}

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CMainFrame::OnEditPaste()
{
    // TODO: 在此添加命令处理程序代码
    //if (NULL != CPageSoftCrossing::m_pGrid)
    //{
    //    CPageSoftCrossing::m_pGrid->OnEditPaste();
    //}
}

void CMainFrame::OnEditCopy()
{
    // TODO: 在此添加命令处理程序代码
    //if (NULL != CPageSoftCrossing::m_pGrid)
    //{
    //    CPageSoftCrossing::m_pGrid->OnEditCopy();
    //}
}

void CMainFrame::OnEditCut()
{
    // TODO: 在此添加命令处理程序代码
    //if (NULL != CPageSoftCrossing::m_pGrid)
    //{
    //    CPageSoftCrossing::m_pGrid->OnEditCut();
    //}
}

void CMainFrame::OnUpdateEditCopyOrCut(CCmdUI* pCmdUI) 
{
    //if (NULL != CPageSoftCrossing::m_pGrid)
    //{
    //    pCmdUI->Enable(CPageSoftCrossing::m_pGrid->GetSelectedCount() > 0);
    //}
}

void CMainFrame::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
    // Attach a COleDataObject to the clipboard see if there is any data
    COleDataObject obj;
    pCmdUI->Enable(obj.AttachClipboard() && obj.IsDataAvailable(CF_TEXT)); 
}
#endif

void CMainFrame::OnAdd()
{
    // TODO: 在此添加命令处理程序代码
    HTREEITEM hSel = m_wndTreeCtrl.GetSelectedItem();
    if (NULL == hSel)
    {
        return;
    }
    DWORD dwData = m_wndTreeCtrl.GetItemData(hSel);
    DWORD dwRet = -1;
    DWORD dwSiteId = -1;
    DWORD dwSegId = -1;
    if (0 == dwData)//选中的是根节点，添加站点
    {
        CSiteDBManagerDlg dlg;
        dwRet = dlg.DoModal();
    }
    else if(dwData & TREE_SITE_FLAG)//选中的是站点，添加分段
    {
        dwSiteId = dwData & ~TREE_SITE_FLAG;
        CSegDBManagerDlg dlg;
        dlg.m_strSiteName = m_wndTreeCtrl.GetItemText(hSel);
        dlg.m_dwSiteId = dwSiteId;
        dwRet = dlg.DoModal();
        dwSegId = dlg.m_dwSegId;
    }
    else if(dwData & TREE_SEG_FLAG)//选中的是分段，忽略
    {
    }

    if (dwRet == IDOK)
    {
        RefreshSiteTree();
        if(dwData & TREE_SITE_FLAG)//添加分段后，自动弹出对话框
        {
            HTREEITEM hItemSite = GetTreeItemBySite(dwSiteId);
            HTREEITEM hItemSeg = GetTreeItemBySeg(hItemSite, dwSegId);
            if (NULL != hItemSeg)
            {
                m_wndTreeCtrl.SelectItem(hItemSeg);
                OnSetSegParam();
            }
        }
    }
}

void CMainFrame::OnEdit()
{
    // TODO: 在此添加命令处理程序代码
    HTREEITEM hSel = m_wndTreeCtrl.GetSelectedItem();
    if (NULL == hSel)
    {
        return;
    }
    DWORD dwData = m_wndTreeCtrl.GetItemData(hSel);
    DWORD dwRet = -1;
    if(dwData & TREE_SITE_FLAG)//选中的是站点
    {
        DWORD dwSiteId = dwData & ~TREE_SITE_FLAG;

        CSiteDBManagerDlg siteDlg;
        siteDlg.m_bStateAdd = FALSE;
        siteDlg.m_dwSiteId = dwSiteId;
        siteDlg.m_strSiteName = m_wndTreeCtrl.GetItemText(hSel);
        dwRet = siteDlg.DoModal();
    }
    else if(dwData & TREE_SEG_FLAG)//选中的是分段
    {
        HTREEITEM hSite = m_wndTreeCtrl.GetParentItem(hSel);
        DWORD dwSegId = dwData & ~TREE_SEG_FLAG;

        CSegDBManagerDlg dlg;
        dlg.m_strSiteName = m_wndTreeCtrl.GetItemText(hSite);
        dlg.m_bStateAdd = FALSE;
        dlg.m_strSegName = m_wndTreeCtrl.GetItemText(hSel);
        dlg.m_dwSegId = dwSegId;
        dwRet = dlg.DoModal();
    }
    else
    {
        //忽略
    }
    if (dwRet == IDOK)
    {
        RefreshSiteTree();
    }
}

void CMainFrame::OnDel()
{
    // TODO: 在此添加命令处理程序代码
    HTREEITEM hSel = m_wndTreeCtrl.GetSelectedItem();
    if (NULL == hSel)
    {
        return;
    }
    DWORD dwData = m_wndTreeCtrl.GetItemData(hSel);
    if(dwData & TREE_SITE_FLAG)//选中的是站点
    {
        DWORD dwSiteId = dwData & ~TREE_SITE_FLAG;
        if (::MessageBox(NULL, _T("删除站点将同时删除站点下的所有分段，您确定要删除该站点吗"), _T("提示"), MB_YESNO) == IDYES)
        {
            //如果有分段，同时删除分段
            CSegDBManager segManager;
            vector<SoftCrossSeg> vecSeg;
            segManager.GetAllSegmentBySid(vecSeg, dwSiteId);
            int nSizeSeg = (int)vecSeg.size();
            for (int j = 0; j < nSizeSeg; j++)
            {
                segManager.Del(vecSeg[j].m_dwSegId);
            }

            CSiteDBManager siteManager;
            siteManager.DelSite(dwSiteId);
            RefreshSiteTree();
        }
    }
    else if(dwData & TREE_SEG_FLAG)//选中的是分段
    {
        DWORD dwSegId = dwData & ~TREE_SEG_FLAG;
        if (::MessageBox(NULL, _T("您确定要删除该分段吗"), _T("提示"), MB_YESNO) == IDYES)
        {
            CSegDBManager segManager;
            segManager.Del(dwSegId);
            RefreshSiteTree();
        }
    }
}

void CMainFrame::OnDxfParse()
{
	CFileDialog fileDlg(TRUE, "dxf", "*.dxf", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT);
	if (IDOK != fileDlg.DoModal())
		return ;

	std::vector<std::string> files;
	POSITION pos = fileDlg.GetStartPosition();
	while (NULL != pos)
	{
		CString filename = fileDlg.GetNextPathName(pos);
		files.push_back(filename.GetBuffer());
	}
	//// 标题内容
	//std::vector<std::string> titleList;
	//titleList.push_back("序号");	// 0
	//titleList.push_back("代号");
	//titleList.push_back("名称");
	//titleList.push_back("材料");
	//titleList.push_back("单位");
	//titleList.push_back("数量");	// 5
	//titleList.push_back("单重");
	//titleList.push_back("共重");
	//titleList.push_back("附注");
	//std::vector<DxfComputeRow> rows = getInfoFromDxf(files, titleList, "名称","数量",  "单位");
	//writeInfoToXls("c:\\a.xls", rows);
	CDxfTitleDlg dlg;
	dlg.SetFilenames(files);
	dlg.DoModal();
}

DWORD CMainFrame::GetSelTreeData()
{
    HTREEITEM hSel = m_wndTreeCtrl.GetSelectedItem();
    if (NULL == hSel)
    {
        return 0;
    }
    DWORD dwData = m_wndTreeCtrl.GetItemData(hSel);
    return dwData;
}

void CMainFrame::OnUpdateAdd(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    //pCmdUI->Enable(!(GetSelTreeData() & TREE_SEG_FLAG));
}


void CMainFrame::OnUpdateEdit(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    //pCmdUI->Enable((GetSelTreeData() & TREE_SITE_FLAG)
    //    || (GetSelTreeData() & TREE_SEG_FLAG));
}


void CMainFrame::OnUpdateDel(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    //pCmdUI->Enable((GetSelTreeData() & TREE_SITE_FLAG)
    //    || (GetSelTreeData() & TREE_SEG_FLAG));
}

HTREEITEM CMainFrame::GetTreeItemBySite(DWORD dwSiteId)
{
    HTREEITEM hItem = m_wndTreeCtrl.GetChildItem(m_hSiteRootItem);//先删除子树
    while(NULL != hItem)
    {
        HTREEITEM hItemTemp = hItem;
        DWORD dwItemData = m_wndTreeCtrl.GetItemData(hItemTemp);
        if ((dwItemData & ~TREE_SITE_FLAG) == dwSiteId)
        {
            return hItemTemp;
        }

        hItem = m_wndTreeCtrl.GetNextSiblingItem(hItemTemp);
    } 
    return NULL;
}

HTREEITEM CMainFrame::GetTreeItemBySeg(HTREEITEM hSiteItem, DWORD dwSegId)
{
    if (NULL == hSiteItem)
    {
        return NULL;
    }
    HTREEITEM hItem = m_wndTreeCtrl.GetChildItem(hSiteItem);//先删除子树
    while(NULL != hItem)
    {
        HTREEITEM hItemTemp = hItem;
        DWORD dwItemData = m_wndTreeCtrl.GetItemData(hItemTemp);
        if ((dwItemData & ~TREE_SEG_FLAG) == dwSegId)
        {
            return hItemTemp;
        }

        hItem = m_wndTreeCtrl.GetNextSiblingItem(hItemTemp);
    } 
    return NULL;
}

void CMainFrame::initPartConnectDb(DWORD segId)
{
	CPartConnectDBManager db;
	vector<PartConnectParam> vec = db.GetAllPartConnect(segId);
	if (3 == vec.size())
		return;

	PartConnectParam param;
	param.m_segId = segId;

	param.m_dwId = 0;
	param.m_name = "横承";
	db.Add(param);
	param.m_dwId = 1;
	param.m_name = "上部定位绳";
	db.Add(param);
	param.m_dwId = 2;
	param.m_name = "下部定位绳";
	db.Add(param);
}

