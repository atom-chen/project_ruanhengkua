

////////////////////////////// label.h ////////////////////

#pragma once

#include "afxwin.h"
class CMyStatic : public CStatic
{
	DECLARE_DYNAMIC(CMyStatic)

public:
	CMyStatic();
	virtual ~CMyStatic();
// Attributes   
private:   
	BOOL b_FillBackColor;   
	COLORREF m_DisBackColro;   
	COLORREF m_BackColor;   
	COLORREF m_TextColor;   
	CFont *m_Font;   
	CBrush m_BackBrush;   
	CBrush m_DisBackBrush; 

	CRect  m_fillRect;
	// Construction   
public:   
	CFont *GetFont() { return m_Font; }   
	virtual void SetFont(LOGFONT *LogFont, BOOL bRedraw = TRUE);   
	virtual void SetFont(CFont* pFont, BOOL bRedraw = TRUE);   
	virtual void SetFont(int nHeight, LPCTSTR fontName, BOOL bRedraw = true);   
	void Set_BackColor(COLORREF cr);    
	COLORREF Get_BackColor() { return(m_BackColor); };   
	void Set_TextColor(COLORREF cr)    
	{    
		m_TextColor = cr; 
		//得到一个窗口对象（CWnd的派生对象）指针的句柄（HWND）时
		if(GetSafeHwnd())
		{
			//该函数的作用是使整个窗口客户区无效。窗口的客户区无效意味着需要重绘，
			Invalidate();  
		}
	}   
	COLORREF Get_TextColor()    
	{   
		return(m_TextColor);    
	}   
	
	void DrawStatic(CDC * pDC);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
};



