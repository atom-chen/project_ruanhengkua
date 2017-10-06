#pragma once
#include "SoftCrossCompute.h"
#include "stdafx.h"
#define TEXT_OFFSET		10

class CDrawGraph
{
public:
	CDrawGraph(void);
	~CDrawGraph(void);
	/*
		@pDrawParam: draw parameter structure
		@drawRect: size of the place for draw
		@pWnd : the Windows for draw
		@pCtrlDC : control DC
	*/
	int drawSoftCrossGraph(draw_param_st* pDrawParam,CRect* drawRect,CWnd* pWnd,CDC* pCtrlDC, CString crossBear, CString fixRopeUp, CString fixRopeBelow);
    void PrintGraph(CWnd* pWnd);
	//  int printGraph(CDC* printDC,CDC* srcDC);
private:
	void drawTrigleLeft(CDC* pCtrlDC,int top_x,int top_y,int line_size);
	void drawTrigleRight(CDC* pCtrlDC,int top_x,int top_y,int line_size);
	void drawLine(CDC* pCtrlDC,int from_x,int from_y,int to_x,int to_y,
			int isShowText = 0,int x_text_offset = 0,int y_text_offset = 0);
	void calculateAllXY(int** arrayX,int** arrayY,draw_param_st* pDrawParam,CRect* drawRect);
	BOOL SaveDraw2Bitmap(CDC* pDC,CRect* pRect);
	int** m_arrayX;
	int** m_arrayY;
	int mWidthOfBMP;
	int mHeightOfBMP;
	CString m_showText;
	CString m_strFormat;


};

