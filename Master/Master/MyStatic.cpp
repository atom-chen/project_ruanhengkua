#include "StdAfx.h"
#include "MyStatic.h"


// CMyStatic

IMPLEMENT_DYNAMIC(CMyStatic, CStatic)

CMyStatic::CMyStatic()
{
	b_FillBackColor = 0;   
	m_Font = 0;   
	m_DisBackColro = RGB(255, 0, 0);  
	
}

CMyStatic::~CMyStatic()
{
	if(m_Font)   
	{   
		m_Font->DeleteObject();   
		delete m_Font;   
	}      

	m_BackBrush.DeleteObject();   
	m_DisBackBrush.DeleteObject();  
}


BEGIN_MESSAGE_MAP(CMyStatic, CStatic)

END_MESSAGE_MAP()



// CMyStatic ��Ϣ�������

void CMyStatic::SetFont(LOGFONT *LogFont, BOOL bRedraw)   
{   
	if(m_Font)   
		m_Font->DeleteObject();   

	if(m_Font == NULL)   
		m_Font = new CFont();   

	if(m_Font)   
	{   
		if(!m_Font->CreatePointFontIndirect( LogFont ))   
		{   
			delete m_Font;   
			m_Font = NULL;   
		}   
	}   
}   

void CMyStatic::SetFont(CFont* pFont, BOOL bRedraw)   
{   
	LOGFONT LogFont;   
	pFont->GetLogFont(&LogFont);   
	SetFont(&LogFont, bRedraw);   
}   

void CMyStatic::SetFont(int nHeight, LPCTSTR fontName, BOOL bRedraw)   
{   
	if(m_Font)   
		m_Font->DeleteObject();   

	if(m_Font == NULL)   
		m_Font = new CFont();   

	if(m_Font)   
	{   
		if(!m_Font->CreatePointFont(nHeight, fontName))   
		{   
			delete m_Font;    
			m_Font = NULL;   
		}   
	}   
	if(bRedraw && GetSafeHwnd())   
		Invalidate();   
}   

void CMyStatic::Set_BackColor(COLORREF cr)    
{    
	m_BackColor = cr;    
	b_FillBackColor = true;    
	m_BackBrush.DeleteObject();   
	m_BackBrush.CreateSolidBrush(m_BackColor);   

	m_DisBackBrush.DeleteObject();   
	m_DisBackBrush.CreateSolidBrush(m_DisBackColro);   

	if(GetSafeHwnd())   
		Invalidate();    
}   

HBRUSH CMyStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  �ڴ˸���DC ���κ�����
	pDC->SetBkMode(TRANSPARENT);   
	pDC->SetTextColor(m_TextColor);   

	if(m_Font)   
		pDC->SelectObject(m_Font);   

	HBRUSH hBrush = (HBRUSH) m_BackBrush;   
	if(!IsWindowEnabled())   
		hBrush = (HBRUSH) m_DisBackBrush;   
	return ( hBrush );       
	// TODO:  �����Ӧ���ø����Ĵ�������򷵻طǿջ���
	
}
BOOL CMyStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return CStatic::OnEraseBkgnd(pDC);
}

void CMyStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  ������Ĵ����Ի���ָ����
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	m_fillRect = lpDrawItemStruct->rcItem;
	int nSavedDC = pDC->SaveDC();
	VERIFY(pDC);
	DrawStatic( pDC );                 //�Լ�����CStaitc
	pDC->RestoreDC( nSavedDC );
}

void CMyStatic::DrawStatic(CDC * pDC)
{
	//���Ʊ���
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(255,255,255));	//����ˢ
	pDC->SelectObject(&Brush);
	
	pDC->Rectangle(m_fillRect);
	//���߿�
	CRect Rect;	
	Rect.SetRect( m_fillRect.left-1, m_fillRect.top-1, m_fillRect.right, m_fillRect.bottom);
	pDC->DrawEdge(&Rect, ~BDR_RAISEDINNER | BDR_RAISEDOUTER,BF_RECT);	//���߿�}
}
void CMyStatic::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	DWORD dwStyle = GetStyle();   
	SetWindowLong(GetSafeHwnd(),GWL_STYLE,dwStyle | SS_OWNERDRAW  );   
	CStatic::PreSubclassWindow();
}
