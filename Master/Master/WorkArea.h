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
	HWND mLButtonDown;							// ���������µĿؼ����
	CXTPTabControlEx *mTabCtrl;					// ��ǩ�ؼ�
	GridCtrl *mSoftCrossGrid;					// ���ӿؼ�
	SoftCrossCalculatePage *mSoftCrossPage;		// ���ҳ
	CMyStatic *mGraphStatic;						// ��ͼ�ؼ�
	CButton *mPrintBtn;							// ��ӡ��ť
	CDrawGraph mDrawGraph;						// 
	draw_param_st mDrawParam;					// 
	CString mCrossBear;							// ��������
	CString mFixRopeUp;							// �ϲ��̶��������
	CString mFixRopeBelow;						// �²��̶��������
	int mCx;
	int mCy;
	int mCurTabIndex;							// ��ǰ��ǩ����
	DWORD mSegId;								// 
public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


