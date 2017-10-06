// MasterView.cpp : implementation of the CMasterView class
//

#include "stdafx.h"
#include "Master.h"

#include "MasterDoc.h"
#include "MasterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMasterView

IMPLEMENT_DYNCREATE(CMasterView, CView)

BEGIN_MESSAGE_MAP(CMasterView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMasterView construction/destruction

CMasterView::CMasterView()
{
	// TODO: add construction code here

}

CMasterView::~CMasterView()
{
}

BOOL CMasterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMasterView drawing

void CMasterView::OnDraw(CDC* /*pDC*/)
{
	CMasterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMasterView printing

BOOL CMasterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMasterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMasterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMasterView diagnostics

#ifdef _DEBUG
void CMasterView::AssertValid() const
{
	CView::AssertValid();
}

void CMasterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMasterDoc* CMasterView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMasterDoc)));
	return (CMasterDoc*)m_pDocument;
}
#endif //_DEBUG


// CMasterView message handlers
