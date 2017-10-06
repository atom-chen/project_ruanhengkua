/**********************************************************************
* ���ߣ�hezhr
* ʱ�䣺2012-09-04
* ���������ӿ�����
**********************************************************************/
#include "stdafx.h"
#include "GridCtrl.h"
#include "..\CommonFunction\CommonFunction.h"
#include "..\SoftCrossCalculatePage.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(GridCtrl, CUGCtrl)
	//{{AFX_MSG_MAP(GridCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
#define TOTAL_COLS									11	// ������
//
#define MATERIAL_COLS								9	// ����ÿ������
#define MATERIAL_START_COL							2	// ���Ͽ�ʼ�к�
#define MATERIAL_LINE_ROWS							3	// ����ÿ�е�ʵ������
//
#define NODE_COLS									9	// �ڵ�ÿ������
#define NODE_START_COL								2	// �ڵ㿪ʼ�к�
#define NODE_LINE_ROWS								5	// �ڵ�ÿ�е�ʵ������
//
#define BRACE_LINES									2	// ֧������
//
#define STATION_TRACK_DISTANCE_COLS					9	// �ɵ����ÿ������
#define STATION_TRACK_DISTANCE_START_COL			2	// �ɵ���࿪ʼ�к�
#define STATION_TRACK_DISTANCE_LINE_ROWS			2	// �ɵ����ÿ�е�ʵ������
//
#define STATION_TRACK_COLS							8	// �ɵ�ÿ������
#define STATION_TRACK_START_COL						3	// �ɵ���ʼ�к�
#define STATION_TRACK_LINE_ROWS						4	// �ɵ�ÿ�е�ʵ������
//
#define OUTPUT_COLS									10	// ���ÿ������
#define OUTPUT_START_COL							1	// �����ʼ�к�
#define OUTPUT_LINE_ROWS							2	// ���ÿ�е�ʵ������
//
#define INIT_STATION_TRACK_NUM						5	// ��ʼ�ɵ�����

//----------------------------------------------------------------------
// ���캯��
GridCtrl::GridCtrl()
{
	mSoftCrossPage = NULL;
	mTotalRow = 0;
	mIsEndEdit = true;
	// 
	for (int b=0; b<BRACE_LINES; ++b)
	{
		BraceType bt;
		mBrace.push_back(bt);
	}
	// 
	mStationTrackDistance.push_back("");
	StationTrackType stt;
	mStationTrack.push_back(stt);
	setStationTrackNum(INIT_STATION_TRACK_NUM);
}
//----------------------------------------------------------------------
// ��������
GridCtrl::~GridCtrl()
{
	clearDatas();
}
//----------------------------------------------------------------------
// ��  �ܣ�	This function is called just after the grid window is created or attached to a dialog item.
//			It can be used to initially setup the grid
// ��  ����	<none>
// ����ֵ��	<none>
void GridCtrl::OnSetup()
{
	refresh();
	setTitle();
	SetMultiSelectMode(TRUE);
	SetHScrollMode(UG_SCROLLTRACKING);
	SetVScrollMode(UG_SCROLLTRACKING);
}
//----------------------------------------------------------------------
// ��  �ܣ�	This notification is called for each additional sheet that the grid
//			might contain, here you can customize each sheet in the grid.
// ��  ����	sheetNumber - index of current sheet
// ����ֵ��	<none>
void GridCtrl::OnSheetSetup(int sheetNumber)
{
	UNREFERENCED_PARAMETER(sheetNumber);
}
//----------------------------------------------------------------------
// ��  �ܣ�	is sent when a cell change action was instigated (user clicked on another cell, used keyboard arrows,
//			or Goto[...] function was called).
// ��  ����	oldcol, oldrow - 
//			newcol, newrow - cell that is gaining focus
// ����ֵ��	TRUE - to allow the move
//			FALSE - to prevent new cell from gaining focus
int GridCtrl::OnCanMove(int oldcol, long oldrow, int newcol, long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(newrow);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	is called when current grid's view is about to be scrolled.
// ��  ����	oldcol, oldrow - coordinates of original top-left cell
//			newcol, newrow - coordinates of top-left cell that is gaining the focus
// ����ֵ��	TRUE - to allow for the scroll
//			FALSE - to prevent the view from scrolling
int GridCtrl::OnCanViewMove(int oldcol, long oldrow, int newcol, long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(newrow);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	This notification allows for dynamic row loading, it will be called
//			when the grid's drawing function has hit the last row.  It allows the grid
//			to ask the datasource/developer if there are additional rows to be displayed.
// ��  ����	numrows - known number of rows in the grid
//			rowspast - number of extra rows that the grid is looking for in the datasource
//			rowsfound - number of rows actually found, usually equal to rowspast or zero.
// ����ֵ��	<none>
void GridCtrl::OnHitBottom(long numrows, long rowspast, long rowsfound)
{
	UNREFERENCED_PARAMETER(numrows);
	UNREFERENCED_PARAMETER(rowspast);
	UNREFERENCED_PARAMETER(rowsfound);
	// used by the datasources
	/*if ( rowsfound > 0 )
	{
		SetNumberRows( numrows + rowsfound, FALSE );
	}*/
}
//----------------------------------------------------------------------
// ��  �ܣ�	Is called when the user has scrolled all the way to the top of the grid.
// ��  ����	numrows - known number of rows in the grid
//			rowspast	- number of extra rows that the grid is looking for in the datasource
// ����ֵ��	<none>
void GridCtrl::OnHitTop(long numrows, long rowspast)
{
	UNREFERENCED_PARAMETER(numrows);
	UNREFERENCED_PARAMETER(rowspast);
}
//----------------------------------------------------------------------
// ��  �ܣ�	is sent when the mouse was moved in the area between columns on the top heading, indicating that the user
//			might want to resize a column.
// ��  ����	col - identifies column number that will be sized
// ����ֵ��	TRUE - to allow sizing
//			FALSE - to prevent sizing
int GridCtrl::OnCanSizeCol(int col)
{
	UNREFERENCED_PARAMETER(col);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	is sent continuously when user is sizing a column. This notification is ideal to provide min/max width checks.
// ��  ����	col - column currently sizing
//			width - current new column width
// ����ֵ��	<none>
void GridCtrl::OnColSizing(int col, int *width)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*width);
}
//----------------------------------------------------------------------
// ��  �ܣ�	This is sent when the user finished sizing the given column.(see above for more information)
// ��  ����	col - column sized
//			width - new column width
// ����ֵ��	<none>
void GridCtrl::OnColSized(int col, int *width)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*width);
}
//----------------------------------------------------------------------
// ��  �ܣ�	is sent when the mouse was moved in the area between rows on the side heading, indicating that the user
//			might want to resize a row.
// ��  ����	row - identifies row number that will be sized
// ����ֵ��	TRUE - to allow sizing
//			FALSE - to prevent sizing
int GridCtrl::OnCanSizeRow(long row)
{
	UNREFERENCED_PARAMETER(row);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent during user sizing of a row, can provide feed back on current height
// ��  ����	row - row sizing
//			height - pointer to current new row height
// ����ֵ��	<none>
void GridCtrl::OnRowSizing(long row, int *height)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*height);
}
//----------------------------------------------------------------------
// ��  �ܣ�	This is sent when the user is finished sizing the given row.
// ��  ����	row - row sized
//			height - pointer to current new row height
// ����ֵ��	<none>
void GridCtrl::OnRowSized(long row, int *height)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*height);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent when the user is about to start sizing of the side heading
// ��  ����	<none>
// ����ֵ��	TRUE - to allow sizing
//			FALSE - to prevent sizing
int GridCtrl::OnCanSizeSideHdg()
{
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent while the heading is being sized
// ��  ����	width - pointer to current new width of the side heading
// ����ֵ��	TRUE - to accept current new size
//			FALSE - to stop sizing, the size is either too large or too small
int GridCtrl::OnSideHdgSizing(int *width)
{
	UNREFERENCED_PARAMETER(*width);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent when the user has completed the sizing of the side heading
// ��  ����	width - pointer to new width
// ����ֵ��	TRUE - to accept new size
//			FALSE - to revert to old size
int GridCtrl::OnSideHdgSized(int *width)
{
	UNREFERENCED_PARAMETER(*width);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent when the user is about to start sizing of the top heading
// ��  ����	<none>
// ����ֵ��	TRUE - to allow sizing
//			FALSE - to prevent sizing
int GridCtrl::OnCanSizeTopHdg()
{
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent while the top heading is being sized
// ��  ����	height - pointer to current new height of the top heading
// ����ֵ��	TRUE - to accept current new size
//			FALSE - to stop sizing, the size is either too large or too small
int GridCtrl::OnTopHdgSizing(int *height)
{
	UNREFERENCED_PARAMETER(*height);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent when the user has completed the sizing of the top heading
// ��  ����	height - pointer to new height
// ����ֵ��	TRUE - to accept new size
//			FALSE - to revert to old size
int GridCtrl::OnTopHdgSized(int *height)
{
	UNREFERENCED_PARAMETER(*height);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the current column changes
// ��  ����	oldcol - column that is loosing the focus
//			newcol - column that the user move into
// ����ֵ��	<none>
void GridCtrl::OnColChange(int oldcol, int newcol)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the current row changes
// ��  ����	oldrow - row that is loosing the locus
//			newrow - row that user moved into
// ����ֵ��	<none>
void GridCtrl::OnRowChange(long oldrow, long newrow)
{
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the current cell changes
// ��  ����	oldcol, oldrow - coordinates of cell that is loosing the focus
//			newcol, newrow - coordinates of cell that is gaining the focus
// ����ֵ��	<none>
void GridCtrl::OnCellChange(int oldcol, int newcol, long oldrow, long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the left visible column in the grid changes, indicating that the view was scrolled horizontally
// ��  ����	oldcol - column that used to be on the left
//			newcol - new column that is going to be the first visible column from the left
// ����ֵ��	<none>
void GridCtrl::OnLeftColChange(int oldcol, int newcol)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the top visible row in the grid changes, indicating that the view was scrolled vertically
// ��  ����	oldrow - row that used to be on the top
//			newrow - new row that is going to be the first visible row from the top
// ����ֵ��	<none>
void GridCtrl::OnTopRowChange(long oldrow, long newrow)
{
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}
//----------------------------------------------------------------------
// ��  �ܣ�	This notification is fired after the current viewing area was scrolled.
// ��  ����	nScrolDir - UG_VSCROLL, UG_HSCROLL
//			if the nScrolDir == UG_VSCROLL
//				oldPos - row that used to be on the top
//				newPos - row that is now the first visible row from the top
//			if the nScrolDir == UG_VSCROLL
//				oldPos - column that used to be on the left
//				newPos - column that is now the first visible column from the left
// ����ֵ��	<none>
void GridCtrl::OnViewMoved( int nScrolDir, long oldPos, long newPos )
{
	UNREFERENCED_PARAMETER(nScrolDir);
	UNREFERENCED_PARAMETER(oldPos);
	UNREFERENCED_PARAMETER(newPos);
}
//----------------------------------------------------------------------
// ��  �ܣ�	The OnSelectionChanged notification is called by the multiselect
//			class when a change occurred in current selection (i.e. user clicks
//			on a new cell, drags a mouse selecting range of cells, or uses
//			CTRL/SHIFT - left click key combination to select cells.)
// ��  ����	startCol, startRow	- coordinates of the cell starting the selection block
//			endCol, endRow - coordinates of the cell ending the selection block
//			blockNum - block number representing this range, this will always represent total number of selection blocks.
// ����ֵ��	<none>
void GridCtrl::OnSelectionChanged(int startCol, long startRow, int endCol, long endRow, int blockNum)
{
	UNREFERENCED_PARAMETER(startCol);
	UNREFERENCED_PARAMETER(startRow);
	UNREFERENCED_PARAMETER(endCol);
	UNREFERENCED_PARAMETER(endRow);
	UNREFERENCED_PARAMETER(blockNum);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user clicks the left mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
// ��  ����	col, row - coordinates of a cell that received mouse click event
//			updn - is TRUE if mouse button is 'down' and FALSE if button is 'up'
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��	<none>
void GridCtrl::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, int processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user clicks the right mouse button within the grid
//			this message is sent when the button goes down then again when the button goes up
// ��  ����	col, row - coordinates of a cell that received mouse click event
//			updn - is TRUE if mouse button is 'down' and FALSE if button is 'up'
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��<none>
void GridCtrl::OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, int processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user double clicks the left mouse button within the grid
// ��  ����col, row	- coordinates of a cell that received mouse click event
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��<none>
void GridCtrl::OnDClicked(int col, long row, RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);

	boolean can_edit = false;
	long param = QuickGetParamEx(col, row);

	if (param > BE_BEGIN && param < BE_END)				// �༭֧��
	{
		can_edit = true;
	}
	else if (param > STATION_TRACK_DISTANCE_BEGIN && param <= STATION_TRACK_DISTANCE_BEGIN + mStationTrackDistanceNum)		// �༭�ɵ����
	{
		can_edit = true;
	}
	else if (param > STATION_TRACK_REALITY_BEGIN && param <= STATION_TRACK_REALITY_BEGIN + mStationTrackNum)		// �༭�ɵ�ʵ������ֵ
	{
		// ���ɱ༭
		//can_edit = true;
	}
	else if (param > STATION_TRACK_DESIGN_BEGIN && param <= STATION_TRACK_DESIGN_BEGIN + mStationTrackNum)		// �༭�ɵ����ֵ
	{
		can_edit = true;
	}
	else if (param > STATION_TRACK_OVERSTEP_BEGIN && param <= STATION_TRACK_OVERSTEP_BEGIN + mStationTrackNum)		// �༭�ɵ�����ֵ
	{
		can_edit = true;
	}

	if (can_edit)		// �ɱ༭,��ʼ�༭
	{
		if (mSoftCrossPage)
		{
			mSoftCrossPage->startEdit();
		}
		StartEdit();
	}
}
//----------------------------------------------------------------------
// ��  �ܣ�	is sent when the user moves mouse over the grid area.
// ��  ����	col, row - coordinates of a cell that received mouse click event
//			point - represents the screen point where the mouse event was detected
//			nFlags - 
//			processed - indicates if current event was processed by other control in the grid.
// ����ֵ��<none>
void GridCtrl::OnMouseMove(int col, long row, POINT *point, UINT nFlags, BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user clicks the left mouse button within the top heading
//			this message is sent when the button goes down then again when the button goes up
// ��  ����	col, row - coordinates of a cell that received mouse click event
//			updn - is TRUE if mouse button is 'down' and FALSE if button is 'up'
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��	<none>
void GridCtrl::OnTH_LClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user clicks the right mouse button within the top heading
//			this message is sent when the button goes down then again when the button goes up
// ��  ����	col, row - coordinates of a cell that received mouse click event
//			updn - is TRUE if mouse button is 'down' and FALSE if button is 'up'
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��	<none>
void GridCtrl::OnTH_RClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user double clicks the left mouse button within the top heading
// ��  ����	col, row - coordinates of a cell that received mouse click event
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��	<none>
void GridCtrl::OnTH_DClicked(int col, long row, RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user clicks the left mouse button within the side heading
//			this message is sent when the button goes down then again when the button goes up
// ��  ����	col, row - coordinates of a cell that received mouse click event
//			updn - is TRUE if mouse button is 'down' and FALSE if button is 'up'
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��	<none>
void GridCtrl::OnSH_LClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user clicks the right mouse button within the side heading
//			this message is sent when the button goes down then again when the button goes up
// ��  ����	col, row - coordinates of a cell that received mouse click event
//			updn - is TRUE if mouse button is 'down' and FALSE if button is 'up'
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��	<none>
void GridCtrl::OnSH_RClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user double clicks the left mouse button within the side heading
// ��  ����	col, row - coordinates of a cell that received mouse click event
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��	<none>
void GridCtrl::OnSH_DClicked(int col, long row, RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user clicks the left mouse button within the top corner button
//			this message is sent when the button goes down then again when the button goes up
// ��  ����	updn - is TRUE if mouse button is 'down' and FALSE if button is 'up'
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��	<none>
void GridCtrl::OnCB_LClicked(int updn, RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user clicks the right mouse button within the top corner button
//			this message is sent when the button goes down then again when the button goes up
// ��  ����	updn - is TRUE if mouse button is 'down' and FALSE if button is 'up'
//			processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��	<none>
void GridCtrl::OnCB_RClicked(int updn, RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent whenever the user double clicks the left mouse button within the top corner button
// ��  ����	processed - indicates if current event was processed by other control in the grid.
//			rect - represents the CDC rectangle of cell in question
//			point - represents the screen point where the mouse event was detected
// ����ֵ��	<none>
void GridCtrl::OnCB_DClicked(RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent when grid received a WM_KEYDOWN message, usually as a result of a user pressing any key on the keyboard.
// ��  ����	vcKey - virtual key code of the key user has pressed
//			processed - indicates if current event was processed by other control in the grid.
// ����ֵ��	<none>
void GridCtrl::OnKeyDown(UINT *vcKey, BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent when grid received a WM_KEYUP message, usually as a result of a user releasing a key.
// ��  ����	vcKey - virtual key code of the key user has pressed
//			processed - indicates if current event was processed by other control in the grid.
// ����ֵ��	<none>
void GridCtrl::OnKeyUp(UINT *vcKey, BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent when grid received a WM_CHAR message, usually as a result
//			of a user pressing any key that represents a printable character.
// ��  ����	vcKey - virtual key code of the key user has pressed
//			processed - indicates if current event was processed by other control in the grid.
// ����ֵ��	<none>
void GridCtrl::OnCharDown(UINT *vcKey, BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}
//----------------------------------------------------------------------
// ��  �ܣ�	This message is sent every time the grid needs to draw a cell in the grid. At this point the cell
//			object has been populated with all of the information that will be used to draw the cell. This information
//			can now be changed before it is used for drawing.
//			Warning: This notification is called for each cell that needs to be painted
//					Placing complicated calculations here will slowdown the refresh speed.
// ��  ����	col, row - coordinates of cell currently drawing
//			cell - pointer to the cell object that is being drawn
// ����ֵ��	<none>
void GridCtrl::OnGetCell(int col, long row, CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}
//----------------------------------------------------------------------
// ��  �ܣ�	This notification is sent every time a cell is set, here you have a chance to make final modification
//			to the cell's content before it is saved to the data source.
// ��  ����	col, row - coordinates of cell currently saving
//			cell - pointer to the cell object that is being saved
// ����ֵ��	<none>
void GridCtrl::OnSetCell(int col, long row, CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}
//----------------------------------------------------------------------
// ��  �ܣ�	This message is sent from a data source and this message depends on the data source
//			check the information on the data source(s) being used The ID of the Data source is also returned
// ��  ����	ID - datasource ID
//			msg - message ID to identify current process
//			param - additional information or object that might be needed
// ����ֵ��	<none>
void GridCtrl::OnDataSourceNotify(int ID, long msg, long param)
{
	UNREFERENCED_PARAMETER(ID);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(param);
}
//----------------------------------------------------------------------
// ��  �ܣ�	This notification is sent by the celltype and it is different from cell-type
//			to celltype and even from notification to notification.  It is usually used to
//			provide the developer with some feed back on the cell events and sometimes to
//			ask the developer if given event is to be accepted or not
// ��  ����	ID - celltype ID
//			col, row - co-ordinates cell that is processing the message
//			msg - message ID to identify current process
//			param - additional information or object that might be needed
// ����ֵ��TRUE - to allow celltype event
//			FALSE - to disallow the celltype event
int GridCtrl::OnCellTypeNotify(long ID, int col, long row, long msg, long param)
{
	UNREFERENCED_PARAMETER(ID);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(param);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	This message is sent whenever the grid is ready to start editing a cell
// ��  ����	col, row - location of the cell that edit was requested over
//			edit - pointer to a pointer to the edit control, allows for swap of edit control
//				if edit control is swapped permanently (for the whole grid) is it better
//				to use 'SetNewEditClass' function.
// ����ֵ��	TRUE - to allow the edit to start
//			FALSE - to prevent the edit from starting
int GridCtrl::OnEditStart(int col, long row, CWnd **edit)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(**edit);
	mIsEndEdit = false;
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	This notification is sent every time the user hits a key while in edit mode.
//			It is mostly used to create custom behavior of the edit control, because it is
//			so easy to allow or disallow keys hit.
// ��  ����	col, row - location of the edit cell
//			edit - pointer to the edit control
//			vcKey - virtual key code of the pressed key
// ����ֵ��	TRUE - to accept pressed key
//			FALSE - to do not accept the key
int GridCtrl::OnEditVerify(int col, long row, CWnd *edit, UINT *vcKey)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*edit);
	UNREFERENCED_PARAMETER(*vcKey);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	This notification is sent when the edit is being finished
// ��  ����	col, row - coordinates of the edit cell
//			edit - pointer to the edit control
//			string - actual string that user typed in
//			cancelFlag - indicates if the edit is being canceled
// ����ֵ��	TRUE - to allow the edit to proceed
//			FALSE - to force the user back to editing of that same cell
int GridCtrl::OnEditFinish(int col, long row, CWnd *edit, LPCTSTR string, BOOL cancelFlag)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*edit);
	UNREFERENCED_PARAMETER(string);
	UNREFERENCED_PARAMETER(cancelFlag);

	std::string str;
	LPCTSTR tip;

#ifdef UNICODE
	str = UnicodeToANSI(string);
	tip = _T("�����ʽ�������������롣");
#else
	str = string;
	tip = "�����ʽ�������������롣";
#endif

	int ret = false;
	long param = QuickGetParamEx(col, row);
	if (editBraceFinish(param, str))
	{
		ret = true;
		QuickSetBackColor(col, row, ""==str?RGB(255, 0, 0):RGB(220, 220, 110));
	}
	else if (editStationTrackDistanceFinish(param, str))
	{
		ret = true;
		QuickSetBackColor(col, row, ""==str?RGB(255, 0, 0):RGB(160, 200, 80));
	}
	//else if (editStationTrackRealityFinish(param, str))
	//{
	//	ret = true;
	//}
	else if (editStationTrackDesignFinish(param, str))
	{
		ret = true;
		QuickSetBackColor(col, row, ""==str?RGB(255, 0, 0):RGB(220, 220, 110));
	}
	else if (editStationTrackOverstepFinish(param, str))
	{
		ret = true;
		QuickSetBackColor(col, row, ""==str?RGB(255, 0, 0):RGB(220, 220, 110));
	}

	if (false == ret)
	{
		MessageBox(tip);
		return FALSE;
	}
	mSoftCrossPage->endEdit(this, param);
	mIsEndEdit = true;
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	This notification is called when the user pressed 'tab' or 'enter' keys
//			Here you have a chance to modify the destination cell
// ��  ����	oldcol, oldrow - edit cell that is loosing edit focus
//			newcol, newrow - cell that the edit is going into, by changing their values you are able to change where to edit next
// ����ֵ��	TRUE - allow the edit to continue
//			FALSE - to prevent the move, the edit will be stopped
int GridCtrl::OnEditContinue(int oldcol, long oldrow, int *newcol, long *newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(*newcol);
	UNREFERENCED_PARAMETER(*newrow);
	return FALSE;	// ���ﷵ��FALSE,�Է�ֹ��Enter���������༭��һ����Ԫ��
}
//----------------------------------------------------------------------
// ��  �ܣ�	This notification is called when the user has selected a menu item in the pop-up menu.
// ��  ����	col, row - the cell coordinates of where the menu originated from
//			section - identify for which portion of the gird the menu is for. possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID, UG_HSCROLL, UG_VSCROLL, UG_CORNERBUTTON
//			item - ID of the menu item selected
// ����ֵ��	<none>
void GridCtrl::OnMenuCommand(int col, long row, int section, int item)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);
	UNREFERENCED_PARAMETER(item);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Is called when the pop up menu is about to be shown
// ��  ����	col, row - the cell coordinates of where the menu originated from
//			setcion - identify for which portion of the gird the menu is for. possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID, UG_HSCROLL, UG_VSCROLL, UG_CORNERBUTTON
// ����ֵ��	TRUE - to allow menu to show
//			FALSE - to prevent the menu from poping up
int GridCtrl::OnMenuStart(int col, long row, int section)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	this notification is called as result of context help bing activated
//			over the UG area and should return context help ID to be displayed
// ��  ����	col, row - coordinates of cell which received the message
//			section	- grid section which received this message
// ����ֵ��	Context help ID to be shown in the help.
DWORD GridCtrl::OnGetContextHelpID(int col, long row, int section)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);
	return 0;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Is called whent the hint is about to be displayed on the main grid area,
//			here you have a chance to set the text that will be shown
// ��  ����	col, row - the cell coordinates of where the menu originated from
//			string - pointer to the string that will be shown
// ����ֵ��	TRUE - to show the hint
//			FALSE - to prevent the hint from showing
int GridCtrl::OnHint(int col, long row, int section, CString *string)
{
	UNREFERENCED_PARAMETER(section);
	string->Format(_T("Col:%d Row:%ld"), col, row);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Is called when the hint is about to be displayed on the vertical scroll bar,
//			here you have a chance to set the text that will be shown
// ��  ����	row - current top row
//			string - pointer to the string that will be shown
// ����ֵ��	TRUE - to show the hint
//			FALSE - to prevent the hint from showing
int GridCtrl::OnVScrollHint(long row, CString *string)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*string);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Is called whent the hint is about to be displayed on the horizontal scroll bar,
//			here you have a chance to set the text that will be shown
// ��  ����	col - current left col
//			string - pointer to the string that will be shown
// ����ֵ��	TRUE - to show the hint
//			FALSE - to prevent the hint from showing
int GridCtrl::OnHScrollHint(int col, CString *string)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*string);
	return TRUE;
}
//----------------------------------------------------------------------
#ifdef __AFXOLE_H__
//----------------------------------------------------------------------
// OLE��	following 3 functions are to be used with drag and drop functionality
//----------------------------------------------------------------------
// ��  �ܣ�	
// ��  ����	pDataObject - 
// ����ֵ��	DROPEFFECT_NONE - no drag and drop
//			DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT GridCtrl::OnDragEnter(COleDataObject* pDataObject)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	return DROPEFFECT_NONE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	
// ��  ����	col, row -
//			pDataObject - 
// ����ֵ��	DROPEFFECT_NONE - no drag and drop
//			DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT GridCtrl::OnDragOver(COleDataObject* pDataObject, int col, long row)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	return DROPEFFECT_NONE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	
// ��  ����	col, row -
//			pDataObject - 
// ����ֵ��	DROPEFFECT_NONE - no drag and drop
//			DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT GridCtrl::OnDragDrop(COleDataObject* pDataObject, int col, long row)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	return DROPEFFECT_NONE;
}
#endif
//----------------------------------------------------------------------
// ��  �ܣ�	Is called when each of the components are painted.
// ��  ����	dc - pointer to the current CDC object
//			section	- section of the grid painted. possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID, UG_HSCROLL, UG_VSCROLL, UG_CORNERBUTTON
// ����ֵ��	<none>
void GridCtrl::OnScreenDCSetup(CDC *dc, int section)
{
	UNREFERENCED_PARAMETER(*dc);
	UNREFERENCED_PARAMETER(section);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent as a result of the 'SortBy' call, this is called for each cell
//			comparison and allows for customization of the sorting routines.
//			We provide following code as example of what could be done here,
//			you might have to modify it to give your application customized sorting.
// ��  ����	cell1, cell2 - pointers to cells that are compared
//			flags - identifies sort direction
// ����ֵ��	value less than zero to identify that the cell1 comes before cell2
//			value equal to zero to identify that the cell1 and cell2 are equal
//			value greater than zero to identify that the cell1 comes after cell2
int GridCtrl::OnSortEvaluate(CUGCell *cell1, CUGCell *cell2, int flags)
{
	// if one of the cells is NULL, do not compare its text
	if (NULL == cell1 && NULL == cell2)
		return 0;
	else if (NULL == cell1)
		return 1;
	else if (NULL == cell2)
		return -1;

	if (flags  & UG_SORT_DESCENDING)
	{
		CUGCell *ptr = cell1;
		cell1 = cell2;
		cell2 = ptr;
	}

	// initialize variables for numeric check
	double num1, num2;
	// compare the cells, with respect to the cell's datatype
	switch (cell1->GetDataType())
	{
		case UGCELLDATA_STRING:
			if (NULL == cell1->GetText() && NULL == cell2->GetText())
				return 0;
			if (NULL == cell1->GetText())
				return 1;
			if (NULL == cell2->GetText())
				return -1;
			return _tcscmp(cell1->GetText(), cell2->GetText());	
		case UGCELLDATA_NUMBER:
		case UGCELLDATA_BOOL:
		case UGCELLDATA_CURRENCY:
			num1 = cell1->GetNumber();
			num2 = cell2->GetNumber();
			if (num1 < num2)
				return -1;
			if (num1 > num2)
				return 1;
			return 0;
		default:
			// if datatype is not recognized, compare cell's text
			if (NULL == cell1->GetText())
				return 1;
			if (NULL == cell2->GetText())
				return -1;
			return _tcscmp(cell1->GetText(), cell2->GetText());	
	}
}
//----------------------------------------------------------------------
// ��  �ܣ�	Called when the user selects one of the tabs on the bottom of the screen
// ��  ����	ID - id of selected tab
// ����ֵ��	<none>
void GridCtrl::OnTabSelected(int ID)
{
	UNREFERENCED_PARAMETER(ID);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Called when the grid components are being created, sized, and arranged.  This notification provides
//			us with ability to further customize the way the grid will be presented to the user.
// ��  ����	grid, topHdg, sideHdg, cnrBtn, vScroll, hScroll, 
//			tabs - sizes and location of each of the grid components
// ����ֵ��	<none>
void GridCtrl::OnAdjustComponentSizes(RECT *grid, RECT *topHdg, RECT *sideHdg, RECT *cnrBtn, RECT *vScroll, RECT *hScroll, RECT *tabs)
{
	UNREFERENCED_PARAMETER(*grid);
	UNREFERENCED_PARAMETER(*topHdg);
	UNREFERENCED_PARAMETER(*sideHdg);
	UNREFERENCED_PARAMETER(*cnrBtn);
	UNREFERENCED_PARAMETER(*vScroll);
	UNREFERENCED_PARAMETER(*hScroll);
	UNREFERENCED_PARAMETER(*tabs);
} 
//----------------------------------------------------------------------
// ��  �ܣ�	Called when the focus rect needs to be painted.
// ��  ����	dc - pointer to the current CDC object
//			rect - rect of the cell that requires the focus rect
// ����ֵ��	<none>
void GridCtrl::OnDrawFocusRect(CDC *dc, RECT *rect)
{
	//DrawExcelFocusRect(dc, rect);
	
	rect->bottom--;
	rect->right--;	
	dc->DrawFocusRect(rect);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent when the area behind the grid needs to be painted.
// ��  ����	section - Id of the grid section that requested this color possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
// ����ֵ��	RGB value representing the color of choice
COLORREF GridCtrl::OnGetDefBackColor(int section)
{
	UNREFERENCED_PARAMETER(section);
	return RGB(255, 255, 255);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent when the grid is gaining focus. The grid will loose focus when the edit is started, or drop list shown
// ��  ����	section - Id of the grid section gaining focus, usually UG_GRID possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
// ����ֵ��	<none>
void GridCtrl::OnSetFocus(int section)
{
	UNREFERENCED_PARAMETER(section);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Sent when the grid is loosing focus.
// ��  ����	section - Id of the grid section loosing focus, usually UG_GRID possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
//			pNewWnd	- pointer to the window that is gaining focus
// ����ֵ��	<none>
void GridCtrl::OnKillFocus(int section, CWnd *pNewWnd)
{
	UNREFERENCED_PARAMETER(section);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Called to inform the grid that the col swap was started on given col.
// ��  ����	col - identifies the column
// ����ֵ��	TRUE - to allow for the swap to take place
//			FALSE - to disallow the swap
BOOL GridCtrl::OnColSwapStart(int col)
{
	UNREFERENCED_PARAMETER(col);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Called when col swap is about to take place, here you can 'lock' certain
//			cols in place so that are not swappable.
// ��  ����	fromCol - where the col originated from
//			toCol - where the col will be located if the swap is allowed
// ����ֵ��	TRUE - to allow for the swap to take place
//			FALSE - to disallow the swap
BOOL GridCtrl::OnCanColSwap(int fromCol, int toCol)
{
	UNREFERENCED_PARAMETER(fromCol);
	UNREFERENCED_PARAMETER(toCol);
	return TRUE;
}
//----------------------------------------------------------------------
// ��  �ܣ�	Called just after column-swap operation was completed.
// ��  ����	fromCol - where the col originated from
//			toCol - where the col will be located if the swap is allowed
// ����ֵ��	<none>
void GridCtrl::OnColSwapped(int fromCol, int toCol)
{
	UNREFERENCED_PARAMETER(fromCol);
	UNREFERENCED_PARAMETER(toCol);
}
//----------------------------------------------------------------------
// ��  �ܣ�	Called to notify the grid that the tracking window was moved
// ��  ����	origRect - from
//			newRect - to location and size of the track window
// ����ֵ��	<none>
void GridCtrl::OnTrackingWindowMoved(RECT *origRect, RECT *newRect)
{
	UNREFERENCED_PARAMETER(*origRect);
	UNREFERENCED_PARAMETER(*newRect);
}
//----------------------------------------------------------------------
void GridCtrl::attachToPage(SoftCrossCalculatePage* page)
{
	mSoftCrossPage = page;
}
//----------------------------------------------------------------------
void GridCtrl::setTitle(const std::string& title /* = ""*/)
{
	QuickSetTextEx(0, -1, title);
	JoinCells(0, -1, TOTAL_COLS - 1, -1);
	RedrawCell(0, -1);
}
//----------------------------------------------------------------------
void GridCtrl::refresh(void)
{
	// �ػ���
	refreshFrame();
	// �ػ��������
	for (size_t i=0; i<mMaterials.size(); ++i)
	{
		int i_row = mMaterialStartRow + MATERIAL_LINE_ROWS * (i / MATERIAL_COLS);
		int i_col = MATERIAL_START_COL + (i % MATERIAL_COLS);
		setMaterial(i_col, i_row, mMaterials[i]);
	}
	// �ػ����нڵ�
	for (size_t j=0; j<mNodes.size(); ++j)
	{
		int j_row = mNodeStartRow + NODE_LINE_ROWS * (j / NODE_COLS);
		int j_col = NODE_START_COL + (j % NODE_COLS);
		setNode(j_col, j_row, mNodes[j]);
	}
	// �ػ�����֧��
	QuickSetTextEx(2, mInputStartRow + 1, mBraceRegionName);
	QuickSetBackColor(2, mInputStartRow + 1, ""==mBraceRegionName?RGB(255, 0, 0):RGB(220, 220, 110));
	for (size_t k=0; k<mBrace.size(); ++k)
	{
		BraceType bt = mBrace[k];
		int k_row = mInputStartRow + 1;
		QuickSetTextEx(3, k_row + k, bt.number);
		QuickSetTextEx(4, k_row + k, bt.bounds);
		QuickSetTextEx(5, k_row + k, bt.slope);
		QuickSetTextEx(6, k_row + k, bt.differ);
		QuickSetBackColor(3, k_row + k, ""==bt.number?RGB(255, 0, 0):RGB(220, 220, 110));
		QuickSetBackColor(4, k_row + k, ""==bt.bounds?RGB(255, 0, 0):RGB(220, 220, 110));
		QuickSetBackColor(5, k_row + k, ""==bt.slope?RGB(255, 0, 0):RGB(220, 220, 110));
		QuickSetBackColor(6, k_row + k, ""==bt.differ?RGB(255, 0, 0):RGB(220, 220, 110));
	}
	// �ػ�����ɵ����
	for (size_t m=0; m<mStationTrackDistance.size(); ++m)
	{
		int m_row = mStationTrackDistanceStartRow + STATION_TRACK_DISTANCE_LINE_ROWS * (m / STATION_TRACK_DISTANCE_COLS) + 1;
		int m_col = STATION_TRACK_DISTANCE_START_COL + (m % STATION_TRACK_DISTANCE_COLS);
		QuickSetTextEx(m_col,  m_row, mStationTrackDistance[m]);
		QuickSetBackColor(m_col, m_row, ""==mStationTrackDistance[m]?RGB(255, 0, 0):RGB(160, 200, 80));
	}
	// �ػ�����ɵ�
	for (size_t n=0; n<mStationTrack.size(); ++n)
	{
		StationTrackType stt = mStationTrack[n];
		int n_row = mStationTrackStartRow + STATION_TRACK_LINE_ROWS * (n / STATION_TRACK_COLS) + 1;
		int n_col = STATION_TRACK_START_COL + (n % STATION_TRACK_COLS);
		QuickSetTextEx(n_col,  n_row, stt.reality);
		QuickSetTextEx(n_col,  n_row + 1, stt.design);
		QuickSetTextEx(n_col,  n_row + 2, stt.overstep);
		QuickSetBackColor(n_col, n_row + 1, ""==stt.design?RGB(255, 0, 0):RGB(220, 220, 110));
		QuickSetBackColor(n_col, n_row + 2, ""==stt.overstep?RGB(255, 0, 0):RGB(220, 220, 110));
	}
	// �ػ�
	RedrawAll();
}
//----------------------------------------------------------------------
void GridCtrl::addMaterial(MaterialType mt)
{
	mMaterials.push_back(mt);
}
//----------------------------------------------------------------------
void GridCtrl::addNode(NodeType nt)
{
	mNodes.push_back(nt);
}
//----------------------------------------------------------------------
void GridCtrl::setBraceRegionName(const std::string& name /* = */)
{
	mBraceRegionName = name;
}
//----------------------------------------------------------------------
void GridCtrl::addBrace(int index, BraceType bt)
{
	if (index < 1 || index > BRACE_LINES)
		return;

	mBrace[index - 1].number = bt.number;
	mBrace[index - 1].bounds = bt.bounds;
	mBrace[index - 1].slope = bt.slope;
	mBrace[index - 1].differ = bt.differ;
}
//----------------------------------------------------------------------
void GridCtrl::setStationTrackNum(int num)
{
	mStationTrackNum = num;
	mStationTrackDistanceNum = num + 1;
	mQNum = num;
	mANum = num + 1;
	mKNum = num + 1;
	mBNum = num + 1;

	mStationTrackDistance.resize(mStationTrackDistanceNum);
	mStationTrack.resize(mStationTrackNum);
}
//----------------------------------------------------------------------
void GridCtrl::addStationTrackDistance(int index, const std::string& st /* = "" */)
{
	if (index < 0 || index >= mStationTrackDistanceNum)
		return;

	mStationTrackDistance[index] = st;
}
//----------------------------------------------------------------------
void GridCtrl::addStationTrack(int index, StationTrackType stt)
{
	if (index < 0 || index >= mStationTrackNum)
		return;

	mStationTrack[index] = stt;
}
//----------------------------------------------------------------------
void GridCtrl::setStationTrackReality(int index, const std::string& reality /* = "" */)
{
	if (index < 0 || index >= mStationTrackNum)
		return;

	int r_row = mStationTrackStartRow + 1 + STATION_TRACK_LINE_ROWS * (index / STATION_TRACK_COLS);
	int r_col = STATION_TRACK_START_COL + (index % STATION_TRACK_COLS);
	QuickSetTextEx(r_col, r_row, reality);

	mStationTrack[index].reality = reality;
}
//----------------------------------------------------------------------
void GridCtrl::setLowest(int index)
{
	int row = mOutputStartRow + OUTPUT_LINE_ROWS * (index / OUTPUT_COLS);
	int col = OUTPUT_START_COL + (index % OUTPUT_COLS);
	QuickSetBackColor(col, row, RGB(0, 0, 255));
	QuickSetBackColor(col, row + 1, RGB(0, 0, 255));
}
//----------------------------------------------------------------------
void GridCtrl::addQ(int index, const std::string& q /* = "" */)
{
	if (index < 0 || index >= mQNum)
		return;

	int q_row = mOutputStartRow + 1 + OUTPUT_LINE_ROWS * (index / OUTPUT_COLS);
	int q_col = OUTPUT_START_COL + (index % OUTPUT_COLS);
	QuickSetTextEx(q_col,  q_row, q);
}
//----------------------------------------------------------------------
void GridCtrl::addA(int index, const std::string& a /* = "" */)
{
	if (index < 0 || index >= mANum)
		return;

	int a_row = mAStartRow + 1 + OUTPUT_LINE_ROWS * (index / OUTPUT_COLS);
	int a_col = OUTPUT_START_COL + (index % OUTPUT_COLS);
	QuickSetTextEx(a_col,  a_row, a);
}
//----------------------------------------------------------------------
void GridCtrl::addK(int index, const std::string& k /* = "" */)
{
	if (index < 0 || index >= mKNum)
		return;

	int k_row = mKStartRow + 1 + OUTPUT_LINE_ROWS * (index / OUTPUT_COLS);
	int k_col = OUTPUT_START_COL + (index % OUTPUT_COLS);
	QuickSetTextEx(k_col,  k_row, k);
}
//----------------------------------------------------------------------
void GridCtrl::addB(int index, const std::string& b /* = "" */)
{
	if (index < 0 || index >= mBNum)
		return;

	int b_row = mBStartRow + 1 + OUTPUT_LINE_ROWS * (index / OUTPUT_COLS);
	int b_col = OUTPUT_START_COL + (index % OUTPUT_COLS);
	QuickSetTextEx(b_col,  b_row, b);
}
//----------------------------------------------------------------------
void GridCtrl::addLowerF1(const std::string& f1 /* = "" */)
{
	int f1_row = mZStartRow + 1;
	int f1_col = OUTPUT_START_COL;
	QuickSetTextEx(f1_col,  f1_row, f1);
}
//----------------------------------------------------------------------
void GridCtrl::addLowerF2(const std::string& f2 /* = "" */)
{
	int f2_row = mZStartRow + 1;
	int f2_col = OUTPUT_START_COL + 1;
	QuickSetTextEx(f2_col,  f2_row, f2);
}
//----------------------------------------------------------------------
void GridCtrl::addUperF1(const std::string& f1 /* = "" */)
{
	int F1_row = mZStartRow + 1;
	int F1_col = OUTPUT_START_COL + 2;
	QuickSetTextEx(F1_col,  F1_row, f1);
}
//----------------------------------------------------------------------
void GridCtrl::addUperF2(const std::string& f2 /* = "" */)
{
	int F2_row = mZStartRow + 1;
	int F2_col = OUTPUT_START_COL + 3;
	QuickSetTextEx(F2_col,  F2_row, f2);
}
//----------------------------------------------------------------------
void GridCtrl::addL1(const std::string& l1 /* = "" */)
{
	int l1_row = mZStartRow + 1;
	int l1_col = OUTPUT_START_COL + 4;
	QuickSetTextEx(l1_col,  l1_row, l1);
}
//----------------------------------------------------------------------
void GridCtrl::addL2(const std::string& l2 /* = "" */)
{
	int l2_row = mZStartRow + 1;
	int l2_col = OUTPUT_START_COL + 5;
	QuickSetTextEx(l2_col,  l2_row, l2);
}
//----------------------------------------------------------------------
void GridCtrl::addM1(const std::string& m1 /* = "" */)
{
	int m1_row = mZStartRow + 1;
	int m1_col = OUTPUT_START_COL + 6;
	QuickSetTextEx(m1_col,  m1_row, m1);
}
//----------------------------------------------------------------------
void GridCtrl::addM2(const std::string& m2 /* = "" */)
{
	int m2_row = mZStartRow + 1;
	int m2_col = OUTPUT_START_COL + 7;
	QuickSetTextEx(m2_col,  m2_row, m2);
}
//----------------------------------------------------------------------
void GridCtrl::addT(const std::string& t /* = "" */)
{
	int t_row = mZStartRow + 1;
	int t_col = OUTPUT_START_COL + 8;
	QuickSetTextEx(t_col,  t_row, t);
}
//----------------------------------------------------------------------
void GridCtrl::addY(const std::string& y /* = "" */)
{
	int y_row = mZStartRow + 1;
	int y_col = OUTPUT_START_COL + 9;
	QuickSetTextEx(y_col,  y_row, y);
}
//----------------------------------------------------------------------
void GridCtrl::SetColWidthEx(int col, const std::string& text, bool default /* = false */)
{
	if (default)
	{
		SetColWidth(col, 60);
		return;
	}
	// ����һ�����ַ���Ч�ʵ�
	//CSize size;
	//GetTextExtentPoint32(GetDC()->GetSafeHdc(), text.c_str(), text.length(), &size);
	//if (size.cx > GetColWidth(col))
	//{
	//	SetColWidth(col, size.cx);
	//}
	// ��������Ч�ʽϸߣ�ÿ������ռ15�����أ��൱������Ӣ����ĸ
	int width = 15*text.size()/2 + 25;
	if (width > GetColWidth(col))
	{
		SetColWidth(col, width);
	}
}
//----------------------------------------------------------------------
void GridCtrl::QuickSetTextEx(int col, long row, const std::string str /* = "" */, bool redraw /* = false */)
{
	SetColWidthEx(col, str);
	QuickSetAlignment(col, row, UG_ALIGNVCENTER | UG_ALIGNCENTER);
#ifdef UNICODE
	QuickSetText(col, row, ANSIToUnicode(str.c_str()).c_str());
#else
	QuickSetText(col, row, str.c_str());
#endif
	if (redraw)
	{
		RedrawCol(col);
	}
}
//----------------------------------------------------------------------
void GridCtrl::QuickSetNumberEx(int col, long row, double num /* = 0.0 */, bool redraw /* = false */)
{
	if (0.0 == num)
		return;

	QuickSetAlignment(col, row, UG_ALIGNVCENTER | UG_ALIGNCENTER);
	QuickSetNumber(col, row, num);
	if (redraw)
	{
		RedrawCell(col, row);
	}
}
//----------------------------------------------------------------------
void GridCtrl::QuickSetParamEx(int col, int row, long param /* = 0 */)
{
	CUGCell cell;
	GetCell(col, row, &cell);
	cell.SetParam(param);
	SetCell(col, row, &cell);
}
//----------------------------------------------------------------------
long GridCtrl::QuickGetParamEx(int col, int row)
{
	CUGCell cell;
	GetCell(col, row, &cell);
	return cell.GetParam();
}
//----------------------------------------------------------------------
double GridCtrl::QuickGetNumberEx(int col, int row)
{
	CUGCell cell;
	GetCell(col, row, &cell);
	return cell.GetNumber();
}
//----------------------------------------------------------------------
std::string GridCtrl::QuickGetTextEx(int col, int row)
{
	CUGCell cell;
	GetCell(col, row, &cell);
	LPCTSTR text = cell.GetText();
#ifdef UNICODE
	return UnicodeToANSI(text);
#else
	return text;
#endif
}
//----------------------------------------------------------------------
void GridCtrl::clearDatas(void)
{
	mBraceRegionName = "";

	// 
	mMaterials.clear();
	mNodes.clear();
	mBrace.clear();
	mBrace.resize(BRACE_LINES);
	// 
	mStationTrackDistance.clear();
	mStationTrack.clear();
	setStationTrackNum(INIT_STATION_TRACK_NUM);
}
//----------------------------------------------------------------------
void GridCtrl::clearGrids(int total_row, int total_col)
{
	// �������
	for (int col=0; col<total_col; ++col)
	{
		SetColWidthEx(col, "", true);
		for (int row=0; row<total_row; ++row)
		{
			CUGCell cell;
			if (UG_SUCCESS == GetCell(col, row, &cell))
			{
				cell.ClearAll();
				SetCell(col, row, &cell);
			}
		}
	}
}
//----------------------------------------------------------------------
void GridCtrl::refreshFrame(void)
{
	clearGrids(mTotalRow, TOTAL_COLS);
	createGrids();
}
//----------------------------------------------------------------------
void GridCtrl::createGrids(void)
{
	// �������
	mMaterialStartRow = 0;
	size_t material_lines = ceilUInt(mMaterials.size(), MATERIAL_COLS);
	material_lines += 0 == material_lines ? 1 : 0;
	int material_total_rows = material_lines * MATERIAL_LINE_ROWS;
	// ����ڵ�
	mNodeStartRow = mMaterialStartRow + material_total_rows + 1;
	size_t node_lines = ceilUInt(mNodes.size(), NODE_COLS);
	node_lines += 0 == node_lines ? 1 : 0;
	int node_total_rows = node_lines * NODE_LINE_ROWS;
	// ��������
	mInputStartRow = mNodeStartRow + node_total_rows + 1;
	// ����֧����ռ����
	int brace_total_rows = BRACE_LINES + 1;
	// ����ɵ������ռ����
	mStationTrackDistanceStartRow = mInputStartRow + brace_total_rows;
	size_t station_track_distance_lines = ceilUInt(mStationTrackDistanceNum, STATION_TRACK_DISTANCE_COLS);
	int station_track_distance_total_rows = STATION_TRACK_DISTANCE_LINE_ROWS * station_track_distance_lines;
	// ����ɵ���ռ����
	mStationTrackStartRow = mStationTrackDistanceStartRow + station_track_distance_total_rows;
	size_t station_track_lines = ceilUInt(mStationTrackNum, STATION_TRACK_COLS);
	size_t station_track_total_rows = STATION_TRACK_LINE_ROWS * station_track_lines;
	int input_total_rows = brace_total_rows + station_track_distance_total_rows + station_track_total_rows;
	// �������
	mOutputStartRow = mInputStartRow + input_total_rows + 1;
	size_t q_lines = ceilUInt(mQNum, OUTPUT_COLS);
	size_t q_total_rows = OUTPUT_LINE_ROWS * q_lines;
	mAStartRow = mOutputStartRow + q_total_rows;
	size_t a_lines = ceilUInt(mANum, OUTPUT_COLS);
	size_t a_total_rows = OUTPUT_LINE_ROWS * a_lines;
	mKStartRow = mAStartRow + a_total_rows;
	size_t k_lines = ceilUInt(mKNum, OUTPUT_COLS);
	size_t k_total_rows = OUTPUT_LINE_ROWS * k_lines;
	mBStartRow = mKStartRow + k_total_rows;
	size_t b_lines = ceilUInt(mBNum, OUTPUT_COLS);
	size_t b_total_rows = OUTPUT_LINE_ROWS * b_lines;
	mZStartRow = mBStartRow + b_total_rows;
	int out_total_rows = q_total_rows + a_total_rows + k_total_rows + b_total_rows + OUTPUT_LINE_ROWS;
	// ������
	mTotalRow = material_total_rows + 1 + node_total_rows + 1 + input_total_rows + 1 + out_total_rows;
	// ��ʼ����
	SetNumberCols(TOTAL_COLS);
	SetNumberRows(mTotalRow);
	// ��ѡ���
	createMaterialGrids(mMaterialStartRow, material_lines, MATERIAL_LINE_ROWS);
	// �ָ���
	QuickSetBackColor(0, mNodeStartRow -1, RGB(0xbe, 0xc0, 0xc2)); 
	JoinCells(0, mNodeStartRow -1, TOTAL_COLS -1 , mNodeStartRow - 1);
	// �ڵ����
	createNodeGrids(mNodeStartRow, node_lines, NODE_LINE_ROWS);
	//// �ָ���
	QuickSetBackColor(0, mInputStartRow -1, RGB(0xbe, 0xc0, 0xc2)); 
	JoinCells(0, mInputStartRow -1, TOTAL_COLS -1 , mInputStartRow - 1);
	// ��ֵ������
	createInputRegionGrids(mInputStartRow, input_total_rows);
	// �ָ���
	QuickSetBackColor(0, mOutputStartRow -1, RGB(225, 225, 30)); 
	JoinCells(0, mOutputStartRow - 1, TOTAL_COLS -1 , mOutputStartRow - 1);
	// �����
	createOutputRegionGrids(mOutputStartRow, out_total_rows);
}
//----------------------------------------------------------------------
void GridCtrl::createMaterialGrids(int start_row, size_t material_lines, size_t line_rows)
{
	size_t total_rows = line_rows * material_lines;

	QuickSetBackColor(0, start_row, RGB(255, 190, 70));
	QuickSetTextEx(0, start_row, "��ѡ���");
	JoinCells(0, start_row, 0, start_row + total_rows - 1);
	for (size_t i=0; i<material_lines; ++i)
	{
		size_t row = start_row + i*line_rows;
		size_t col = 1;

		QuickSetTextEx(col, row, "��������");
		QuickSetTextEx(col, row + 1, "����");
		QuickSetTextEx(col, row + 2, "����");
		// ������ɫ
		COLORREF pre_color = RGB(235, 235, 90);
		COLORREF suf_color = RGB(220, 220, 110);
		if (0 == i % 2)
		{
			pre_color = RGB(190, 220, 150);
			suf_color = RGB(160, 200, 80);
		}
		for (size_t j=0; j<line_rows; ++j)
		{
			QuickSetBackColor(col, row + j, pre_color);
			for (int m=2; m<=10; ++m)
			{
				QuickSetBackColor(m, row + j, suf_color);
			}
		}
	}
}
//----------------------------------------------------------------------
void GridCtrl::createNodeGrids(int start_row, size_t node_lines, size_t line_rows)
{
	size_t total_rows = line_rows * node_lines;

	QuickSetBackColor(0, start_row, RGB(255, 190, 70));
	QuickSetTextEx(0, start_row, "�ڵ����");
	JoinCells(0, start_row, 0, start_row + total_rows - 1);
	for (size_t i=0; i<node_lines; ++i)
	{
		size_t row = start_row + i*line_rows;
		size_t col = 1;

		QuickSetTextEx(col, row, "�ڵ�����");
		QuickSetTextEx(col, row + 1, "�������������");
		QuickSetTextEx(col, row + 2, "�ϲ��̶�������");
		QuickSetTextEx(col, row + 3, "�²��̶�������");
		QuickSetTextEx(col, row + 4, "�ڵ�����");
		// ������ɫ
		COLORREF pre_color = RGB(235, 235, 90);
		COLORREF suf_color = RGB(160, 200, 80);
		if (0 == i % 2)
		{
			pre_color = RGB(190, 220, 150);
			suf_color = RGB(220, 220, 110);
		}
		for (size_t j=0; j<line_rows; ++j)
		{
			QuickSetBackColor(col, row + j, pre_color);
			for (int m=2; m<=10; ++m)
			{
				QuickSetBackColor(m, row + j, suf_color);
			}
		}
	}
}
//----------------------------------------------------------------------
void GridCtrl::createInputRegionGrids(int start_row, size_t total_row)
{
	// ����
	QuickSetBackColor(0, start_row, RGB(255, 190, 70));
	QuickSetTextEx(0, start_row, "��ֵ������");
	JoinCells(0, start_row, 0, start_row + total_row - 1);
	// ֧��
	size_t brace_rows = createBraceGrids(start_row, BRACE_LINES);
	// �ɵ����
	size_t distrance_rows = createStationTrackDistanceGrids(start_row + BRACE_LINES + 1, mStationTrackDistanceNum, STATION_TRACK_DISTANCE_LINE_ROWS);
	// �ɵ�
	size_t station_track_rows = createStationTrackGrids(start_row + BRACE_LINES + distrance_rows + 1, mStationTrackNum, STATION_TRACK_LINE_ROWS);
}
//----------------------------------------------------------------------
size_t GridCtrl::createBraceGrids(int start_row, size_t brace_lines)
{
	size_t total_rows = brace_lines + 1;

	QuickSetBackColor(1, start_row, RGB(235, 235, 90));
	QuickSetTextEx(1, start_row, "֧��");
	JoinCells(1, start_row, 1, start_row + total_rows - 1);
	QuickSetTextEx(2, start_row, "֧������");
	QuickSetTextEx(3, start_row, "֧����");
	QuickSetTextEx(4, start_row, "����");
	QuickSetTextEx(5, start_row, "б��");
	QuickSetTextEx(6, start_row, "��������������");
	JoinCells(2, start_row + 1, 2, start_row + 2);
	// ���ñ�ʶ,�Ա��������
	QuickSetParamEx(2, start_row + 1, BE_REGION);
	QuickSetParamEx(3, start_row + 1, BE_NUM_1);
	QuickSetParamEx(3, start_row + 2, BE_NUM_2);
	QuickSetParamEx(4, start_row + 1, BE_BOUNDS_1);
	QuickSetParamEx(4, start_row + 2, BE_BOUNDS_2);
	QuickSetParamEx(5, start_row + 1, BE_SLOPE_1);
	QuickSetParamEx(5, start_row + 2, BE_SLOPE_2);
	QuickSetParamEx(6, start_row + 1, BE_DIFFER_1);
	QuickSetParamEx(6, start_row + 2, BE_DIFFER_2);
	// ������ɫ
	for (int i=2; i<=10; ++i)
	{
		QuickSetBackColor(i, start_row, RGB(220, 220, 110));
		QuickSetBackColor(i, start_row + 1, RGB(220, 220, 110));
		QuickSetBackColor(i, start_row + 2, RGB(220, 220, 110));
	}

	return total_rows;
}
//----------------------------------------------------------------------
size_t GridCtrl::createStationTrackDistanceGrids(int start_row, size_t distance_num, size_t line_rows)
{
	size_t distance_lines = ceilUInt(distance_num, STATION_TRACK_DISTANCE_COLS);
	size_t total_rows = line_rows * distance_lines;

	QuickSetBackColor(1, start_row, RGB(190, 220, 150));
	QuickSetTextEx(1, start_row, "�ɵ����");
	JoinCells(1, start_row, 1, start_row + total_rows - 1);

	for (size_t i=0; i<distance_num; ++i)
	{
		int row = i / STATION_TRACK_DISTANCE_COLS;
		int col = i % STATION_TRACK_DISTANCE_COLS;
		
		row = start_row + row*line_rows;
		col = STATION_TRACK_DISTANCE_START_COL + col;
		
		QuickSetTextEx(col, row, "���" + toString(i + 1));
		QuickSetParamEx(col, row + 1, STATION_TRACK_DISTANCE_BEGIN + i + 1);
		// ������ɫ
		for (int j=2; j<=10; ++j)
		{
			QuickSetBackColor(j, row, RGB(160, 200, 80));
			QuickSetBackColor(j, row + 1, RGB(160, 200, 80));
		}
	}

	return total_rows;
}
//----------------------------------------------------------------------
size_t GridCtrl::createStationTrackGrids(int start_row, size_t station_track_num, size_t line_rows)
{
	size_t station_track_lines = ceilUInt(station_track_num, STATION_TRACK_COLS);
	size_t total_rows = line_rows * station_track_lines;

	QuickSetBackColor(1, start_row, RGB(235, 235, 90));
	QuickSetTextEx(1, start_row, "����ֵ");
	JoinCells(1, start_row, 1, start_row + total_rows - 1);

	for (size_t i=0; i<station_track_lines; ++i)
	{
		int i_row = start_row + i*line_rows;
		int i_col = 2;
		
		QuickSetTextEx(i_col, i_row, "�ɵ�");
		QuickSetTextEx(i_col, i_row + 1, "ʵ������ֵ");
		QuickSetTextEx(i_col, i_row + 2, "���");
		QuickSetTextEx(i_col, i_row + 3, "����");
		// ������ɫ
		COLORREF i_color = RGB(240, 240, 140);
		for (int m=0; m<=3; ++m)
		{
			QuickSetBackColor(2, i_row + m, i_color);
		}
	}

	for (size_t j=0; j<station_track_num; ++j)
	{
		int j_row = j / STATION_TRACK_COLS;
		int j_col = j % STATION_TRACK_COLS;

		j_row = start_row + j_row*line_rows;
		j_col = STATION_TRACK_START_COL + j_col;

		QuickSetTextEx(j_col, j_row, toString(j + 1) + "��");
		QuickSetParamEx(j_col, j_row + 1, STATION_TRACK_REALITY_BEGIN + j + 1);
		QuickSetParamEx(j_col, j_row + 2, STATION_TRACK_DESIGN_BEGIN + j + 1);
		QuickSetParamEx(j_col, j_row + 3, STATION_TRACK_OVERSTEP_BEGIN + j + 1);
		// ������ɫ
		COLORREF j_color = RGB(220, 220, 110);
		for (int q=3; q<=10; ++q)
		{
			for (size_t n=0; n<line_rows; ++n)
			{
				QuickSetBackColor(q, j_row + n, j_color);
			}
		}
	}

	return total_rows;
}
//----------------------------------------------------------------------
void GridCtrl::createOutputRegionGrids(int start_row, size_t total_row)
{
	QuickSetBackColor(0, start_row, RGB(255, 190, 70));
	QuickSetTextEx(0, start_row, "�����");
	JoinCells(0, start_row, 0, start_row + total_row - 1);

	size_t q_total_rows = createOutputLineGrids(start_row, mQNum, OUTPUT_LINE_ROWS, "Q");
	size_t a_total_rows = createOutputLineGrids(start_row + q_total_rows, mANum, OUTPUT_LINE_ROWS, "a");
	size_t k_total_rows = createOutputLineGrids(start_row + q_total_rows + a_total_rows, mKNum, OUTPUT_LINE_ROWS, "k");
	size_t b_total_rows = createOutputLineGrids(start_row + q_total_rows + a_total_rows + k_total_rows, mBNum, OUTPUT_LINE_ROWS, "B");
	size_t z_start_row = start_row + q_total_rows + a_total_rows + k_total_rows + b_total_rows;
	QuickSetTextEx(OUTPUT_START_COL, z_start_row, "f1");
	QuickSetTextEx(OUTPUT_START_COL + 1, z_start_row, "f2");
	QuickSetTextEx(OUTPUT_START_COL + 2, z_start_row, "F1");
	QuickSetTextEx(OUTPUT_START_COL + 3, z_start_row, "F2");
	QuickSetTextEx(OUTPUT_START_COL + 4, z_start_row, "L1");
	QuickSetTextEx(OUTPUT_START_COL + 5, z_start_row, "L2");
	QuickSetTextEx(OUTPUT_START_COL + 6, z_start_row, "M1");
	QuickSetTextEx(OUTPUT_START_COL + 7, z_start_row, "M2");
	QuickSetTextEx(OUTPUT_START_COL + 8, z_start_row, "T");
	QuickSetTextEx(OUTPUT_START_COL + 9, z_start_row, "Y");
	// ������ɫ
	for (int i=1; i<=OUTPUT_COLS; ++i)
	{
		for (size_t q=start_row; q<start_row+q_total_rows; ++q)
		{
			QuickSetBackColor(i, q, RGB(220, 220, 110));
			QuickSetBackColor(i, q, RGB(220, 220, 110));
		}
		for (size_t a=start_row+q_total_rows; a<start_row+q_total_rows+a_total_rows; ++a)
		{
			QuickSetBackColor(i, a, RGB(160, 200, 80));
			QuickSetBackColor(i, a, RGB(160, 200, 80));
		}
		for (size_t k=start_row+q_total_rows+a_total_rows; k<start_row+q_total_rows+a_total_rows+k_total_rows; ++k)
		{
			QuickSetBackColor(i, k, RGB(220, 220, 110));
			QuickSetBackColor(i, k, RGB(220, 220, 110));
		}
		for (size_t b=start_row+q_total_rows+a_total_rows+k_total_rows; b<start_row+q_total_rows+a_total_rows+k_total_rows+b_total_rows; ++b)
		{
			QuickSetBackColor(i, b, RGB(160, 200, 80));
			QuickSetBackColor(i, b, RGB(160, 200, 80));
		}
		for (size_t j=start_row+q_total_rows+a_total_rows+k_total_rows+b_total_rows; j<start_row+q_total_rows+a_total_rows+k_total_rows+b_total_rows+OUTPUT_LINE_ROWS; ++j)
		{
			QuickSetBackColor(i, j, RGB(220, 220, 110));
			QuickSetBackColor(i, j, RGB(220, 220, 110));
		}
	}
}
//----------------------------------------------------------------------
size_t GridCtrl::createOutputLineGrids(int start_row, size_t num, size_t line_rows, std::string head)
{
	size_t lines = ceilUInt(num, OUTPUT_COLS);
	size_t total_rows = lines * line_rows;

	for (size_t i=0; i<num; ++i)
	{
		int row = i / OUTPUT_COLS;
		int col = i % OUTPUT_COLS;

		row = start_row + row*line_rows;
		col = OUTPUT_START_COL + col;

		QuickSetTextEx(col, row, head + toString(i + 1));
	}

	return total_rows;
}
//----------------------------------------------------------------------
void GridCtrl::setMaterial(int col, int row, const MaterialType& mt)
{
	QuickSetTextEx(col, row, mt.name);
	QuickSetTextEx(col, row + 1, mt.length);
	QuickSetTextEx(col, row + 2, mt.weight);
}
//----------------------------------------------------------------------
void GridCtrl::setNode(int col, int row, const NodeType& nt)
{
	QuickSetTextEx(col, row, nt.name);
	QuickSetTextEx(col, row + 1, nt.length1);
	QuickSetTextEx(col, row + 2, nt.length2);
	QuickSetTextEx(col, row + 3, nt.length3);
	QuickSetTextEx(col, row + 4, nt.weight);
}
//----------------------------------------------------------------------
bool GridCtrl::editBraceFinish(long param, const std::string& str)
{
	if (param <= BE_BEGIN || param > BE_END - 1)
		return false;
	
	switch (param)
	{
	case BE_REGION:
		mBraceRegionName = str;
		return true;
	case BE_NUM_1:
		mBrace[0].number = str;
		return true;
	case BE_NUM_2:
		mBrace[1].number = str;
		return true;
	}

	if (0 != str.compare("") && false == isDigit(str))
		return false;

	switch (param)
	{
	case BE_BOUNDS_1:
		mBrace[0].bounds = str;
		break;
	case BE_BOUNDS_2:
		mBrace[1].bounds = str;
		break;
	case BE_SLOPE_1:
		mBrace[0].slope = str;
		break;
	case BE_SLOPE_2:
		mBrace[1].slope = str;
		break;
	case BE_DIFFER_1:
		mBrace[0].differ = str;
		break;
	case BE_DIFFER_2:
		mBrace[1].differ = str;
		break;
	}
	return true;
}
//----------------------------------------------------------------------
bool GridCtrl::editStationTrackDistanceFinish(long param, const std::string& str)
{
	if (param <= STATION_TRACK_DISTANCE_BEGIN || param > STATION_TRACK_DISTANCE_BEGIN + mStationTrackDistanceNum)
		return false;

	if (0 != str.compare("") && false == isDigit(str))
		return false;

	mStationTrackDistance[param - STATION_TRACK_DISTANCE_BEGIN - 1] = str;
	return true;
}
//----------------------------------------------------------------------
bool GridCtrl::editStationTrackRealityFinish(long param, const std::string& str)
{
	if (param <= STATION_TRACK_REALITY_BEGIN || param > STATION_TRACK_REALITY_BEGIN + mStationTrackNum)
		return false;

	if (0 != str.compare("") && false == isDigit(str))
		return false;

	mStationTrack[param - STATION_TRACK_REALITY_BEGIN - 1].reality = str;
	return true;
}
//----------------------------------------------------------------------
bool GridCtrl::editStationTrackDesignFinish(long param, const std::string& str)
{
	if (param <= STATION_TRACK_DESIGN_BEGIN || param > STATION_TRACK_DESIGN_BEGIN + mStationTrackNum)
		return false;

	if (0 != str.compare("") && false == isDigit(str))
		return false;

	mStationTrack[param - STATION_TRACK_DESIGN_BEGIN - 1].design = str;
	return true;
}
//----------------------------------------------------------------------
bool GridCtrl::editStationTrackOverstepFinish(long param, const std::string& str)
{
	if (param <= STATION_TRACK_OVERSTEP_BEGIN || param > STATION_TRACK_OVERSTEP_BEGIN + mStationTrackNum)
		return false;

	if (0 != str.compare("") && false == isDigit(str))
		return false;

	mStationTrack[param - STATION_TRACK_OVERSTEP_BEGIN - 1].overstep = str;
	return true;
}
//----------------------------------------------------------------------
const std::string& GridCtrl::getBraceRegionName(void)
{
	return mBraceRegionName;
}
//----------------------------------------------------------------------
const std::vector<BraceType>& GridCtrl::getBrace(void)
{
	return mBrace;
}
//----------------------------------------------------------------------
const std::vector<std::string>& GridCtrl::getStationTrackDistance(void)
{
	return mStationTrackDistance;
}
//----------------------------------------------------------------------
const std::vector<StationTrackType>& GridCtrl::getStationTrack(void)
{
	return mStationTrack;
}
//----------------------------------------------------------------------


