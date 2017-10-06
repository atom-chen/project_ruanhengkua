// MasterDoc.h : interface of the CMasterDoc class
//


#pragma once


class CMasterDoc : public CDocument
{
protected: // create from serialization only
	CMasterDoc();
	DECLARE_DYNCREATE(CMasterDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMasterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
