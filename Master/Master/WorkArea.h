#pragma once

//#include "PageHangLine.h"
//#include "PageWristArm.h"
//#include "PageSoftHangLine.h"
//#include "PageSoftCrossing.h"
#include "UltimateGrid\GridCtrl.h"
#include "SoftCrossCalculatePage.h"
#include "CXTPEx/XTPTabControlEx.h"
#include "DrawGraph.h"
#include "SoftCrossCompute.h"
#include "MyStatic.h"

// CWorkArea frame

class CWorkArea : public CFrameWnd
{
public:
// 	CPageHangLine m_wndPageHangLine;
// 	CPageWristArm m_wndPageWristArm;
// 	CPageSoftHangLine m_wndPageSoftHanging;
	//CPageSoftCrossing m_wndPageSoftCrossing;

	GridCtrl* getSoftCrossGrid(void);
	SoftCrossCalculatePage* getSoftCrossPage(void);
	void setSelTabIndex(int index);
	int getSelTabIndex(void);
	void drawGraph(DWORD segId);

private:
	void createSoftCrossGrid(CWnd *parent);
	void showSoftCrossGrid(bool show = true);
	void createDrawGraph(CWnd *parent);
	void showDrawGraph(bool show = true);
	BOOL clickCtrl(MSG *pMsg);
	void onClickPrintBtn(MSG *pMsg);
	
protected:
	DECLARE_DYNCREATE(CWorkArea)
	CWorkArea();           // protected constructor used by dynamic creation
	virtual ~CWorkArea();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	HWND mLButtonDown;							// 鼠标左键按下的控件句柄
	CXTPTabControlEx *mTabCtrl;					// 标签控件
	GridCtrl *mSoftCrossGrid;					// 格子控件
	SoftCrossCalculatePage *mSoftCrossPage;		// 软跨页
	CMyStatic *mGraphStatic;						// 绘图控件
	CButton *mPrintBtn;							// 打印按钮
	CDrawGraph mDrawGraph;						// 
	draw_param_st mDrawParam;					// 
	CString mCrossBear;							// 横承零件串
	CString mFixRopeUp;							// 上部固定绳零件串
	CString mFixRopeBelow;						// 下部固定绳零件串
	int mCx;
	int mCy;
	int mCurTabIndex;							// 当前标签索引
	DWORD mSegId;								// 
public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


