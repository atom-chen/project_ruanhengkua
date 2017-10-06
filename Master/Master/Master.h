// Master.h : main header file for the Master application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMasterApp:
// See Master.cpp for the implementation of this class
//

class CMasterApp : public CWinApp
{
public:
	CMasterApp();


// Overrides
public:
	virtual BOOL InitInstance();
    HACCEL m_hAccelerator;

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

class CCategoriesTreeNavi : public CXTPPropertyPageTreeNavigator
{
public:
	CCategoriesTreeNavi()
		: CXTPPropertyPageTreeNavigator(TVS_SHOWSELALWAYS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT)
	{

	}

	BOOL CreateTree()
	{
		//ASSERT(m_pSheet->GetPageCount() == 4);
// 		if (m_pSheet->GetPageCount() != 4)
// 			return FALSE;

		HTREEITEM hItemGeneral = InsertItem(_T("参数"));
		HTREEITEM hItemOptions = InsertItem(_T("计算结果"));

		for (int i = 0; i < m_pSheet->GetPageCount(); i++)
		{
			CXTPPropertyPage* pPage = m_pSheet->GetPage(i);

			CString strCaption = pPage->GetCaption();

			HTREEITEM hItem = InsertItem(strCaption, -1, -1, (i < 2)  ? hItemGeneral : hItemOptions);
			SetItemData(hItem, (DWORD_PTR)pPage);
			pPage->m_dwData = (DWORD_PTR)hItem;
		}

		return TRUE;
	}

};

extern CMasterApp theApp;