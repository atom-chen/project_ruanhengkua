// WorkArea.cpp : implementation file
//

#include "stdafx.h"
#include "WorkArea.h"
#include "MainFrm.h"
#include "DB/PartConnectDBManager.h"


#define DRAW_TIMER_ID	0x10000
#define TAB_CTRL_ID		0x10001
#define PRINT_BTN_ID	0x10002
// CWorkArea

IMPLEMENT_DYNCREATE(CWorkArea, CFrameWnd)

CWorkArea::CWorkArea()
{
	mTabCtrl = NULL;
	mSoftCrossGrid = NULL;
	mSoftCrossPage = NULL;
	mGraphStatic = NULL;
	mPrintBtn = NULL;
	mDrawParam.is_get_draw_param = 0;
	mCrossBear = _T("");
	mFixRopeUp = _T("");
	mFixRopeBelow = _T("");
	mCx = 0;
	mCy = 0;
	mCurTabIndex = 0;
	mSegId = -1;
}

CWorkArea::~CWorkArea()
{

}

BOOL CWorkArea::PreTranslateMessage(MSG* pMsg)
{
	if (clickCtrl(pMsg))
	{
		switch (::GetDlgCtrlID(pMsg->hwnd))
		{
		case PRINT_BTN_ID:
			onClickPrintBtn(pMsg);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CWorkArea::clickCtrl(MSG *pMsg)
{
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
		mLButtonDown = pMsg->hwnd;
		break;
	case WM_LBUTTONUP:
		//if (mLButtonDown == mMouseMove)
		CWnd *pWnd = FromHandle(pMsg->hwnd);
		CRect rect;
		pWnd->GetWindowRect(&rect);
		if (pMsg->pt.x > rect.left && pMsg->pt.x < rect.right && pMsg->pt.y > rect.top && pMsg->pt.y < rect.bottom && 
			mLButtonDown == pMsg->hwnd)
		{
			return TRUE;
		}
		break;
	}
	return FALSE;
}


BEGIN_MESSAGE_MAP(CWorkArea, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CWorkArea message handlers


int CWorkArea::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	mTabCtrl = new CXTPTabControlEx(this);
	mTabCtrl->Create(WS_VISIBLE|WS_CHILD|SS_NOTIFY|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, TAB_CTRL_ID);
	mTabCtrl->GetPaintManager()->SetAppearance(xtpTabAppearanceExcel);
	mTabCtrl->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	mTabCtrl->InsertItem(0, _T("软横跨计算"));
	mTabCtrl->InsertItem(1, _T("绘图预览"));

	mSoftCrossPage = new SoftCrossCalculatePage();
	createSoftCrossGrid(mTabCtrl);
	createDrawGraph(mTabCtrl);
	SetTimer(DRAW_TIMER_ID, 1000, NULL);

	return 0;
}

void CWorkArea::OnDestroy()
{
	if (mTabCtrl)
	{
		delete mTabCtrl;
		mTabCtrl = NULL;
	}

	if (mSoftCrossGrid)
	{
		delete mSoftCrossGrid;
		mSoftCrossGrid = NULL;
	}
	if (mSoftCrossPage)
	{
		delete mSoftCrossPage;
		mSoftCrossPage = NULL;
	}
	if (mGraphStatic)
	{
		delete mGraphStatic;
		mGraphStatic = NULL;
	}
	if (mPrintBtn)
	{
		delete mPrintBtn;
		mPrintBtn = NULL;
	}
}

void CWorkArea::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	mCx = cx;
	mCy = cy;

	if (mTabCtrl)
	{
		mTabCtrl->MoveWindow(0, 0, cx, cy);
	}
	showSoftCrossGrid(0 == mCurTabIndex);
	showDrawGraph(1 == mCurTabIndex);
}

void CWorkArea::OnPaint()
{
	CFrameWnd::OnPaint();
	showDrawGraph(1 == mCurTabIndex);
}

void CWorkArea::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(DRAW_TIMER_ID);
	CFrameWnd::OnTimer(nIDEvent);
	showDrawGraph(1 == mCurTabIndex);
}

void CWorkArea::createSoftCrossGrid(CWnd *parent)
{
	if (mSoftCrossGrid)
	{
		delete mSoftCrossGrid;
		mSoftCrossGrid = NULL;
	}
	mSoftCrossGrid = new GridCtrl();
	mSoftCrossGrid->CreateGrid(WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), parent, 10000);
	mSoftCrossGrid->setTitle("请选择分段进行计算");
	mSoftCrossGrid->attachToPage(mSoftCrossPage);
}

void CWorkArea::showSoftCrossGrid(bool show /* = true */)
{
	if (NULL == mSoftCrossGrid)
		return;

	mSoftCrossGrid->MoveWindow(1, 23, mCx - 1, mCy - 23);
	mSoftCrossGrid->ShowWindow(show ? SW_SHOW : SW_HIDE);
}

void CWorkArea::createDrawGraph(CWnd *parent)
{
	if (mGraphStatic)
	{
		delete mGraphStatic;
		mGraphStatic = NULL;
	}
	mGraphStatic = new CMyStatic();
	mGraphStatic->Create(NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), parent, 10001);
    mGraphStatic->Set_BackColor(RGB(0,0,255));
	if (mPrintBtn)
	{
		delete mPrintBtn;
		mPrintBtn = NULL;
	}
	mPrintBtn = new CButton();
	int x = 450, y = 500, w = 100, h = 30;
	mPrintBtn->Create(_T("打印图纸"), WS_CHILD | WS_VISIBLE, CRect(x, y, x + w, y + h), parent, PRINT_BTN_ID); 
}



void CWorkArea::showDrawGraph(bool show /* = true */)
{
	if (NULL == mGraphStatic)
		return;

	mGraphStatic->MoveWindow(1, 23, mCx - 1, mCy - 23);
	mGraphStatic->ShowWindow(show ? SW_SHOW : SW_HIDE);

	mGraphStatic->Invalidate();
	mGraphStatic->UpdateWindow();

	// 
	CDC *dc = mGraphStatic->GetDC();
	if (mDrawParam.is_get_draw_param)
	{
		CRect rct(50, 80, 1037, 449);
		mDrawGraph.drawSoftCrossGraph(&mDrawParam, &rct, mGraphStatic, dc, mCrossBear, mFixRopeUp, mFixRopeBelow);
		mPrintBtn->ShowWindow(show ? SW_SHOW : SW_HIDE);
	}
	else
	{
		dc->TextOutA(100, 100, "必须经过运算方可绘图！");
		mPrintBtn->ShowWindow(SW_HIDE);
	}
	mGraphStatic->ReleaseDC(dc);
}

void CWorkArea::onClickPrintBtn(MSG *pMsg)
{
	//TODO:在此添加打印模块

	mDrawGraph.PrintGraph(mGraphStatic);
}

GridCtrl* CWorkArea::getSoftCrossGrid(void)
{
	if (!mSoftCrossGrid->isEndEdit())
	{
		createSoftCrossGrid(mTabCtrl);
		showSoftCrossGrid();
	}
	return mSoftCrossGrid;
}

SoftCrossCalculatePage* CWorkArea::getSoftCrossPage(void)
{
	return mSoftCrossPage;
}

void CWorkArea::setSelTabIndex(int index)
{
	if (index == mCurTabIndex)
		return;

	mCurTabIndex = index;
	showSoftCrossGrid(0 == index);
	drawGraph(mSegId);
}

int CWorkArea::getSelTabIndex(void)
{
	return mCurTabIndex;
}

void CWorkArea::drawGraph(DWORD segId)
{
	mSegId = segId;
	if (NULL == mSoftCrossPage || NULL == mGraphStatic)
		return;

	mCrossBear = -1 == segId ? _T("") : _T("横承：");
	mFixRopeUp = -1 == segId ? _T("") : _T("上部固定绳：");
	mFixRopeBelow = -1 == segId ? _T("") : _T("下部固定绳：");
	if (-1 != segId)
	{
		CPartConnectDBManager db;
		vector<PartConnectParam> vec = db.GetAllPartConnect(CMainFrame::CurSegId);
		for (size_t i=0; i<vec.size(); ++i)
		{
			CString parts = vec[i].m_partNames.c_str();
			DWORD dwId = vec[i].m_dwId;
			if (0 == dwId)
				mCrossBear += parts;
			else if (1 == dwId)
				mFixRopeUp += parts;
			else if (2 == dwId)
				mFixRopeBelow += parts;
		}
	}
	mDrawParam.is_get_draw_param = 0;
	mSoftCrossPage->calcAndGetDrawParam(&mDrawParam);
	showDrawGraph(1 == mCurTabIndex);
}



//HBRUSH CWorkArea::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  在此更改 DC 的任何特性
//	if ( nCtlColor == CTLCOLOR_STATIC )
//	{
//		static HBRUSH hbrStatic = ::CreateSolidBrush(RGB(255, 0, 255));
//			//COLOR是你想设置的背景颜色 此处必须为静态变量，否则不能实现
//		pDC->SetBkColor(RGB(255, 0, 255));
//		return hbrStatic ; //返回该刷
//	}
//
//	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
//	return hbr;
//}
