// MasterDoc.cpp : implementation of the CMasterDoc class
//

#include "stdafx.h"
#include "Master.h"

#include "MasterDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMasterDoc

IMPLEMENT_DYNCREATE(CMasterDoc, CDocument)

BEGIN_MESSAGE_MAP(CMasterDoc, CDocument)
END_MESSAGE_MAP()


// CMasterDoc construction/destruction

CMasterDoc::CMasterDoc()
{
	// TODO: add one-time construction code here

}

CMasterDoc::~CMasterDoc()
{
}

BOOL CMasterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMasterDoc serialization

void CMasterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMasterDoc diagnostics

#ifdef _DEBUG
void CMasterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMasterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMasterDoc commands
