#include "StdAfx.h"
#include "DrawGraph.h"
#define TOP_PILLAR_SIZE  20
#define TRIGLE_LINE		10
#define APPR_VALUE	    15



CDrawGraph::CDrawGraph(void)
{
	int i = 0;
	m_arrayX = new int *[3];
	m_arrayY = new int*[3];
	for(i = 0; i<3; i++){
		m_arrayX[i] = new int[20];
		m_arrayY[i] = new int[20];
		memset(m_arrayX[i],0,20*sizeof(int));
		memset(m_arrayY[i],0,20*sizeof(int));
	}
}


CDrawGraph::~CDrawGraph(void)
{
	delete []m_arrayX;
	delete []m_arrayY;
	m_arrayX = NULL;
	m_arrayY = NULL;
	mWidthOfBMP = mHeightOfBMP = 0;
}
void CDrawGraph::drawTrigleLeft(CDC* pCtrlDC,int top_x,int top_y,int line_size)
{
	pCtrlDC->MoveTo(top_x,top_y);
	pCtrlDC->LineTo(top_x+0.86*line_size,top_y+line_size/2);
	pCtrlDC->LineTo(top_x+0.86*line_size,top_y-line_size/2);
	pCtrlDC->LineTo(top_x,top_y);
	//

}
void CDrawGraph::drawTrigleRight(CDC* pCtrlDC,int top_x,int top_y,int line_size)
{
	pCtrlDC->MoveTo(top_x,top_y);
	pCtrlDC->LineTo(top_x-0.86*line_size,top_y+line_size/2 );
	pCtrlDC->LineTo(top_x-0.86*line_size,top_y-line_size/2);
	pCtrlDC->LineTo(top_x,top_y);
}

void CDrawGraph::drawLine(CDC* pCtrlDC,int from_x,int from_y,int to_x,int to_y,
	int isShowText,int x_text_offset,int y_text_offset)
{
	int text_x = 0;
	int text_y = 0;

	text_x = from_x + (to_x - from_x)/2 + x_text_offset;
	text_y = from_y + (to_y-from_y)/2 + y_text_offset;

	pCtrlDC->MoveTo(from_x,from_y);

	pCtrlDC->LineTo(to_x,to_y);

	

	if(isShowText){
		pCtrlDC->SetBkMode(TRANSPARENT);
		pCtrlDC->TextOutA(text_x,text_y,m_showText);
	}
	
}


double GetSum(double* arr, int num)
{
	int i = 0 ;
	double sum = 0.0;
	for(i = 0 ; i< num ; i++)
		sum += arr[i];
	return sum;
}
void CDrawGraph::calculateAllXY(int** arrayX,int** arrayY,draw_param_st* pDrawParam,CRect* drawRect)
{ 
	int j = 0;
	double sum = 0.0; //下部固定绳求和
	double stright_hang_sum = 0.0;//直吊子求和
	double X1 = 0.0;
	double X2 = 0.0;
	//bottom line & top line
	sum = GetSum(pDrawParam->lower_position_ropes,pDrawParam->hang_num + 2);
	stright_hang_sum = GetSum(pDrawParam->lower_position_ropes,pDrawParam->hang_num);
	X1 = (double)(drawRect->right - 6*TRIGLE_LINE - 2*TOP_PILLAR_SIZE - arrayX[2][0]);
	X2 = (double)(drawRect->right - 6*TRIGLE_LINE - 2*TOP_PILLAR_SIZE - arrayX[1][0]);
	for(j = 1; j< pDrawParam->hang_num+4; j++){
		if(j <= pDrawParam->lowest_pt_idx + 1){
			arrayX[2][j] = arrayX[2][j-1] + pDrawParam->lower_position_ropes[j-1]*X1/sum;
			arrayX[1][j] = arrayX[1][j-1] + pDrawParam->upper_position_ropes[j-1]*X2/sum;
			arrayY[1][j] = arrayY[1][0];
		    arrayY[2][j] = arrayY[2][0];
			arrayX[0][j] = arrayX[1][j];
			if(j < pDrawParam->lowest_pt_idx )
				arrayY[0][j] = arrayY[1][j] - pDrawParam->stright_hang_values[j-1]*(X2+50)/sum;
			else
				arrayY[0][j] = arrayY[0][pDrawParam->lowest_pt_idx-1]+APPR_VALUE;
			continue;
		}
		if(j == pDrawParam->lowest_pt_idx + 2 ){
			arrayX[2][j] = arrayX[2][j-1] + 3*TRIGLE_LINE;
			arrayX[1][j] = arrayX[1][j-1] + 3*TRIGLE_LINE;
			arrayY[1][j] = arrayY[1][0];
		    arrayY[2][j] = arrayY[2][0];
			arrayX[0][j] = arrayX[1][j];
			arrayY[0][j] = arrayY[0][j-1];
			continue;
		}
		arrayX[2][j] = arrayX[2][j-1] + pDrawParam->lower_position_ropes[j-2]*X1/sum;
		arrayX[1][j] = arrayX[1][j-1] + pDrawParam->upper_position_ropes[j-2]*X2/sum;
		arrayY[1][j] = arrayY[1][0];
		arrayY[2][j] = arrayY[2][0];
		if(j != pDrawParam->hang_num + 3){
			arrayX[0][j] = arrayX[1][j];
			if(j == pDrawParam->lowest_pt_idx + 3)
				arrayY[0][j] = arrayY[0][j-1];
			else
				arrayY[0][j] = arrayY[1][j] - pDrawParam->stright_hang_values[j-3]*(X2+50)/sum;
		}else{//横承最后一个点坐标为（X-top,0）
			arrayX[0][j] = drawRect->right - TOP_PILLAR_SIZE;
			arrayY[0][j] = drawRect->top;
		}
	}
}
int CDrawGraph::drawSoftCrossGraph(draw_param_st* pDrawParam,CRect* drawRect,CWnd* pWnd,CDC* pCtrlDC, CString crossBear, CString fixRopeUp, CString fixRopeBelow)
{
	if(!pDrawParam->is_get_draw_param)
		return false;
	int X = drawRect->right;
	int Y = drawRect->bottom;
	int LX = drawRect->left;
	int LY =drawRect->top;
	int nowX = LX; 
	int nowY = LY;
	int** arrayX = NULL;
	int** arrayY = NULL;
	int i = 0;
	int j = 0;
	int k = 0;

	if(pDrawParam->B4draw[0] > 100)
		m_strFormat = "%0.0f";
	else
		m_strFormat = "%0.3f";

//	arrayX = (int**)m_arrayX;
//	arrayY = (int**)m_arrayY;

	
	//draw left pillar

	drawLine(pCtrlDC,LX,LY,LX,Y);
	drawLine(pCtrlDC,LX,LY,TOP_PILLAR_SIZE+LX,LY);
	drawLine(pCtrlDC,TOP_PILLAR_SIZE+LX,LY,LX+TOP_PILLAR_SIZE*2,Y);
	m_arrayX[0][0] = LX+TOP_PILLAR_SIZE;
	m_arrayY[0][0] = LY;
	//upper line start from (0,7*Y/15)
	nowX = LX;
	nowY = LY+7*Y/15;
	m_showText = "20";
	drawLine(pCtrlDC,nowX,nowY,nowX+TOP_PILLAR_SIZE*2,nowY);
	nowX+=TOP_PILLAR_SIZE*2;
	pCtrlDC->Rectangle(nowX,nowY - 3,nowX + 10,nowY + 3);
	nowX+=10;
	drawTrigleLeft(pCtrlDC,nowX, nowY ,TRIGLE_LINE);
	nowX+=TRIGLE_LINE;
	drawTrigleLeft(pCtrlDC,nowX, nowY ,TRIGLE_LINE);
	nowX+=TRIGLE_LINE;
	drawTrigleLeft(pCtrlDC,nowX, nowY ,TRIGLE_LINE);
	nowX+=TRIGLE_LINE;
	m_arrayX[1][0] = nowX;
	m_arrayY[1][0] = nowY;
	//lower line start from (0,9*Y/15)
	nowX = LX;
	nowY = LY+9*Y/15;
	drawLine(pCtrlDC,nowX,nowY,nowX+TOP_PILLAR_SIZE*2,nowY);
	nowX+=TOP_PILLAR_SIZE*2;
	pCtrlDC->Rectangle(nowX,nowY - 3,nowX + 10,nowY + 3);
	nowX+=10;
	drawTrigleLeft(pCtrlDC,nowX, nowY ,TRIGLE_LINE);
	nowX+=TRIGLE_LINE;
	drawTrigleLeft(pCtrlDC,nowX, nowY ,TRIGLE_LINE);
	nowX+=TRIGLE_LINE;
	drawTrigleLeft(pCtrlDC,nowX, nowY ,TRIGLE_LINE);
	nowX+=TRIGLE_LINE;
	m_arrayX[2][0] = nowX;
	m_arrayY[2][0] = nowY;

	//设置零件串文本
	pCtrlDC->TextOutA(nowX + 10, nowY + 60, crossBear);
	pCtrlDC->TextOutA(nowX + 10, nowY + 90, fixRopeUp);
	pCtrlDC->TextOutA(nowX + 10, nowY + 120, fixRopeBelow);
	
	//根据这些起点可以计算所有的坐标
	calculateAllXY(m_arrayX,m_arrayY,pDrawParam,drawRect);
	
	//得到坐标后，接下来的工作就是把这些点用线段连起来
	//先连接横承线段,坐标点个数 = hang_num + 3
	double* tmp = NULL;
	for(i = 0; i<3 ; i++)
	{
		k = 0;
		for(j = 0; j< pDrawParam->hang_num+4; j++)
		{
			if(j == pDrawParam->lowest_pt_idx + 1){
				//画中间的三个三角形
				drawTrigleLeft(pCtrlDC,m_arrayX[i][j],m_arrayY[i][j],TRIGLE_LINE);
				drawTrigleLeft(pCtrlDC,m_arrayX[i][j]+TRIGLE_LINE,m_arrayY[i][j],TRIGLE_LINE);
				drawTrigleLeft(pCtrlDC,m_arrayX[i][j]+2*TRIGLE_LINE,m_arrayY[i][j],TRIGLE_LINE);
				continue;
			}
			
			if(j == pDrawParam->hang_num + 3){
				if(i!=0){//画右边的三个三角形
					drawLine(pCtrlDC,m_arrayX[i][j],m_arrayY[i][j],m_arrayX[i][j]+TEXT_OFFSET,m_arrayY[i][j]);
					drawTrigleRight(pCtrlDC,m_arrayX[i][j]+TEXT_OFFSET+TRIGLE_LINE,m_arrayY[i][j],TRIGLE_LINE);
					drawTrigleRight(pCtrlDC,m_arrayX[i][j]+TEXT_OFFSET+2*TRIGLE_LINE,m_arrayY[i][j],TRIGLE_LINE);
					drawTrigleRight(pCtrlDC,m_arrayX[i][j]+TEXT_OFFSET+3*TRIGLE_LINE,m_arrayY[i][j],TRIGLE_LINE);
					continue;
				}else{
					continue;
				}
			}
			switch(i){
				case 0:
					m_showText.Format(m_strFormat.GetString(),pDrawParam->B4draw[k]);
					drawLine(pCtrlDC,m_arrayX[i][j],m_arrayY[i][j],m_arrayX[i][j+1],m_arrayY[i][j+1],1,-10,-20);
					k++;
					break;
				case 1:
					m_showText.Format(m_strFormat.GetString(),pDrawParam->upper_position_ropes[k]);
					drawLine(pCtrlDC,m_arrayX[i][j],m_arrayY[i][j],m_arrayX[i][j+1],m_arrayY[i][j+1],1,-10);
					k++;
					break;
				case 2:
					m_showText.Format(m_strFormat.GetString(),pDrawParam->lower_position_ropes[k]);
					drawLine(pCtrlDC,m_arrayX[i][j],m_arrayY[i][j],m_arrayX[i][j+1],m_arrayY[i][j+1],1,-10);
					k++;
					break;
				default:
					break;
			}
			
		}
	}
	//画横线
	for(i = 0; i< pDrawParam->hang_num; i++){
	
		m_showText.Format(m_strFormat.GetString(),pDrawParam->stright_hang_values[i]);
		if(i< pDrawParam->lowest_pt_idx){
			drawLine(pCtrlDC,m_arrayX[0][i+1],m_arrayY[0][i+1],m_arrayX[1][i+1],m_arrayY[1][i+1],1,10,-5);
			drawLine(pCtrlDC,m_arrayX[1][i+1],m_arrayY[1][i+1],m_arrayX[2][i+1],m_arrayY[2][i+1]);
		}else{
			drawLine(pCtrlDC,m_arrayX[0][i+3],m_arrayY[0][i+3],m_arrayX[1][i+3],m_arrayY[1][i+3],1,10,-5);
			drawLine(pCtrlDC,m_arrayX[1][i+3],m_arrayY[1][i+3],m_arrayX[2][i+3],m_arrayY[2][i+3]);
		}
	}
	j = pDrawParam->hang_num + 3;
	//画右边支柱
	nowX = X;
	nowY = LY;
	drawLine(pCtrlDC,m_arrayX[1][j]+3*TRIGLE_LINE+TEXT_OFFSET,m_arrayY[1][j],X,m_arrayY[1][j]);
	drawLine(pCtrlDC,m_arrayX[2][j]+3*TRIGLE_LINE+TEXT_OFFSET,m_arrayY[2][j],X,m_arrayY[2][j]);
	drawLine(pCtrlDC,nowX,nowY,nowX-TOP_PILLAR_SIZE,nowY);
	drawLine(pCtrlDC,nowX,nowY,nowX,Y);
	drawLine(pCtrlDC,nowX-TOP_PILLAR_SIZE,nowY,nowX-2*TOP_PILLAR_SIZE,Y);
	
	//save result to bitmap


	SaveDraw2Bitmap(pCtrlDC,drawRect);
	return true;
}
BOOL CDrawGraph::SaveDraw2Bitmap(CDC* pDC,CRect *pRect)
{

    CBitmap bitmap;   
    CDC memDC ;   
   
    memDC.CreateCompatibleDC(pDC);   

    bitmap.CreateCompatibleBitmap(pDC,pRect->Width()+60,pRect->Height()+120);   
    memDC.SelectObject(&bitmap);   
    memDC.BitBlt(0,0,pRect->Width()+60,pRect->Height()+120,pDC,10,10,SRCCOPY); 

    CFile file(_T("./tmp.bmp"),CFile::modeCreate|CFile::modeWrite);   

    BITMAP bInfo;   
    bitmap.GetBitmap(&bInfo);   

    //计算调色板大小   
    int panelsize = 0;   
    if (bInfo.bmBitsPixel<24) //非真彩色   
    {   
        //panelsize = pow((double)2,bInfo.bmBitsPixel)*sizeof(RGBQUAD);   
    }   

    //定义位图信息   
    BITMAPINFO*  bMapInfo = (BITMAPINFO*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);   
    bMapInfo->bmiHeader.biBitCount = bInfo.bmBitsPixel;   
    bMapInfo->bmiHeader.biClrImportant = 0;   
    bMapInfo->bmiHeader.biCompression = 0;   
    bMapInfo->bmiHeader.biHeight = bInfo.bmHeight;   
    bMapInfo->bmiHeader.biPlanes = bInfo.bmPlanes;   
    bMapInfo->bmiHeader.biSize = sizeof(BITMAPINFO);   
    bMapInfo->bmiHeader.biSizeImage = bInfo.bmHeight*bInfo.bmWidthBytes;   
    bMapInfo->bmiHeader.biWidth = bInfo.bmWidth;   
    bMapInfo->bmiHeader.biXPelsPerMeter = 0;   
    bMapInfo->bmiHeader.biYPelsPerMeter = 0;   
	mWidthOfBMP = bInfo.bmHeight;
	mHeightOfBMP = bInfo.bmWidth;
    //获取位图的实际数据   
    char* pData = new char[bMapInfo->bmiHeader.biSizeImage];   
    int len = GetDIBits(pDC->m_hDC,bitmap,0,bInfo.bmHeight,pData,bMapInfo,DIB_RGB_COLORS);   

    BITMAPFILEHEADER bFileHeader;   
    bFileHeader.bfType = 0x4D42;   
    bFileHeader.bfReserved1 = 0;   
    bFileHeader.bfReserved2 = 0;   
    bFileHeader.bfSize = sizeof(BITMAPFILEHEADER);    
    bFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+panelsize;   

    //向文件中写入位图数据   
    file.Write(&bFileHeader,sizeof(BITMAPFILEHEADER));   
    file.Write(&bMapInfo->bmiHeader,sizeof(BITMAPINFOHEADER));   
    file.Write(pData,bMapInfo->bmiHeader.biSizeImage+panelsize);   
    file.Close();   
    delete pData;   
    LocalFree(bMapInfo);
    bitmap.DeleteObject();
	return true;
}

void CDrawGraph::PrintGraph(CWnd* pWnd)
{
	    PRINTDLG pd;  //该结构包含打印对话框中的所有信息
		LPDEVMODE  lpDevMode; 
		if(AfxGetApp()->GetPrinterDeviceDefaults(&pd)) //获得默认的打印机的信息
		{
			 lpDevMode=(LPDEVMODE)GlobalLock(pd.hDevMode); 
			 if(lpDevMode)
			 {   
				  lpDevMode->dmPaperSize=DMPAPER_A4;    //将打印纸设置为A4
				  lpDevMode->dmOrientation=DMORIENT_LANDSCAPE; //将打印机设置为横向打印。
				  lpDevMode->dmPrintQuality = 1200;
				//  lpDevMode->dmPrintQualITy=800;   //打印分辨率为600dpi
			 }
			 GlobalUnlock(pd.hDevMode);
		}

		CDC   memDC;   
		CClientDC   dc(pWnd);   
    
		int   bmpWidth   =   mHeightOfBMP;   
		int   bmpHeight   =   mWidthOfBMP;   
		memDC.CreateCompatibleDC(   &dc   );   
		CBitmap*   bitmap   =   new   CBitmap();   
    
		HBITMAP hBitmap = NULL;     
		hBitmap = (HBITMAP)LoadImage(NULL,   "./tmp.bmp",   IMAGE_BITMAP,   0,   0,     
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);     
		bitmap->Attach(hBitmap);     
    
		CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(bitmap);   
		if   (pOldBitmap   ==   NULL)   //   if   bitmap   is   very   big,   better   check   this   !   
		{   
			memDC.DeleteDC();   
			delete   bitmap;  
			return;   
		}   
    
		CDC   prtDC;   
		CPrintInfo   printInfo;   
		CSize   size;   
		DOCINFO   di;   
		CString   szPortName,   szAppName,   szPrintError;   
		szAppName.LoadString(AFX_IDS_APP_TITLE);   
		szPrintError   =   "";   
    
		CSize   paper_size;     //printer   paper   size   in   mm   
		int   xLogPPI   =   0;   
		int   yLogPPI   =   0;   
    
		if(   AfxGetApp()->GetPrinterDeviceDefaults(&printInfo.m_pPD->m_pd)   )   
		{   
			HDC   hDC   =   printInfo.m_pPD->m_pd.hDC;   
			if   (hDC   ==   NULL)   
			hDC   =   printInfo.m_pPD->CreatePrinterDC();   
			if(hDC   !=NULL)   
			{   
				prtDC.Attach(hDC);   
				paper_size.cx   =   prtDC.GetDeviceCaps(HORZSIZE);   
				paper_size.cy   =   prtDC.GetDeviceCaps(VERTSIZE);   
				xLogPPI   =   prtDC.GetDeviceCaps(LOGPIXELSX);   
				yLogPPI   =   prtDC.GetDeviceCaps(LOGPIXELSY);   
			}   
			else     
			{   
				AfxMessageBox("Can   not   find   printer.   Please   check   installed/default   printers.");   
		
				return;   
			}   
		}   
		int   scr_xLogPPI   =   dc.GetDeviceCaps(LOGPIXELSX);   
		int   scr_yLogPPI   =   dc.GetDeviceCaps(LOGPIXELSY);   
		int   paper_width   =   (int)   ((double)   paper_size.cx   *   (double)   xLogPPI   /   25.4);       //width   of   a   printed   page   in   pixels   
		int   paper_height   =   (int)   ((double)   paper_size.cy   *   (double)   yLogPPI   /   25.4);   
		double   ratio_x   =   (double)   xLogPPI   /   (double)   scr_xLogPPI;   
		double   ratio_y   =   (double)   yLogPPI   /   (double)   scr_yLogPPI;   
    
		CString   strPageNumber   =   "";   
    
		int   page_info_left   =   (int)   (   (double)   paper_width   *   0.99);   
		int   page_info_right   =   paper_width;   
		int   page_info_top   =   (int)   (   (double)   paper_height   *   0.99);   
		int   page_info_bottom   =   paper_height;   
		CRect   page_info_rect   =   CRect(page_info_left, page_info_top,     
										   page_info_right, page_info_bottom   );   
		int   printed_pages   =   0;   
		int   total_print_pages   =   0;   
		BOOL   bAbort_print   =   FALSE;   
    
		//   calculate   pages   
		int   total_pages   =   (bmpWidth   *   ratio_x   +   paper_width   -   1   )   /   paper_width;   
		//pop   up   printer   dialog   
		CPrintDialog   prtDlg(FALSE,   PD_PAGENUMS);   
    
		prtDlg.m_pd.nMinPage   =   1;   
		prtDlg.m_pd.nMaxPage   =   total_pages;   
		prtDlg.m_pd.nFromPage   =   1;   
		prtDlg.m_pd.nToPage   =   total_pages;   
    
		if(prtDlg.DoModal()   ==   IDOK   )   
		{   
			memset(&di,   0,   sizeof(DOCINFO));   
			di.cbSize   =   sizeof(DOCINFO);   
			di.lpszDocName   =   szAppName;   
			szPortName   =   prtDlg.GetPortName();   
			di.lpszOutput   =   szPortName;   
			prtDC.m_bPrinting   =   TRUE;   
		}   
		else
		{
		
			return;     //Cancel   button   pressed,   don't   forget   this!
		}
    
		if(prtDC.StartDoc(&di)   ==   -1)   
		{   
			AfxMessageBox("Printing   error   occured.   Unable   to   find   printer.");   
			prtDC.Detach();   
			prtDC.DeleteDC(); 
		
			return;   
		}   
    
		prtDC.SetMapMode(MM_TEXT);   
    
		int   i   =   0;   
		for(i   =   0;   i   <   total_pages;   i++)   
		{   
			prtDC.StartPage();   
			strPageNumber.Format("Page:%d   of   %d",   ++printed_pages,   total_print_pages   );   
    
			if   (   i   ==   (total_pages   -   1)   &&   total_pages   >   1   )   //last   page   
			{   
				int   last_bmpWidth   =   bmpWidth   -   paper_width   /   ratio_x   *   i;   
				prtDC.StretchBlt(0,   0,   last_bmpWidth   *   ratio_x,   bmpHeight*   ratio_y,   &memDC,   
					  paper_width   *   i   /   ratio_x,   0,   last_bmpWidth,   bmpHeight,   SRCCOPY);   
			}   
			else   
			prtDC.StretchBlt(0,   0,   paper_width,   bmpHeight*   ratio_y,   &memDC,   
				  paper_width   *   i   /   ratio_x,   0,   paper_width   /   ratio_x   ,   bmpHeight,   SRCCOPY);   
			prtDC.TextOut(page_info_rect.left,   page_info_rect.top,   strPageNumber   );   
    
			prtDC.EndPage();   
		}   
		memDC.SelectObject(pOldBitmap);   
		delete   bitmap;   
		memDC.DeleteDC();   
    
		prtDC.EndDoc();   
		prtDC.Detach();   
		prtDC.DeleteDC();   
		return;
}