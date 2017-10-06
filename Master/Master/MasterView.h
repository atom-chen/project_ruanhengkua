// MasterView.h : interface of the CMasterView class
//


#pragma once


class CMasterView : public CView
{
protected: // create from serialization only
	CMasterView();
	DECLARE_DYNCREATE(CMasterView)

// Attributes
public:
	CMasterDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMasterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MasterView.cpp
inline CMasterDoc* CMasterView::GetDocument() const
   { return reinterpret_cast<CMasterDoc*>(m_pDocument); }
#endif
