/**********************************************************************
* 作者：hezhr
* 时间：2012-09-04
* 描述：格子控制器
**********************************************************************/
#ifndef _GRID_CTRL_H_
#define _GRID_CTRL_H_

#include "UGCtrl.h"
#include "DataType.h"
#include <string>
#include <vector>

class SoftCrossCalculatePage;

class GridCtrl : public CUGCtrl
{
public:
	GridCtrl();
	virtual ~GridCtrl();

private:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

	
	//{{AFX_MSG(GridCtrl)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//***** Over-ridable Notify Functions *****
	virtual void OnSetup();
	virtual void OnSheetSetup(int sheetNumber);

	// 移动和调整大小
	virtual int  OnCanMove(int oldcol,long oldrow,int newcol,long newrow);
	virtual int  OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow);
	virtual void OnHitBottom(long numrows,long rowspast,long rowsfound);
	virtual void OnHitTop(long numrows,long rowspast);
	
	virtual int  OnCanSizeCol(int col);
	virtual void OnColSizing(int col,int *width);
	virtual void OnColSized(int col,int *width);
	virtual int  OnCanSizeRow(long row);
	virtual void OnRowSizing(long row,int *height);
	virtual void OnRowSized(long row,int *height);

	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual int  OnTopHdgSizing(int *height);
	virtual int  OnSideHdgSizing(int *width);
	virtual int  OnTopHdgSized(int *height);
	virtual int  OnSideHdgSized(int *width);
		
	virtual void OnColChange(int oldcol,int newcol);
	virtual void OnRowChange(long oldrow,long newrow);
	virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);
	virtual void OnLeftColChange(int oldcol,int newcol);
	virtual void OnTopRowChange(long oldrow,long newrow);
	virtual void OnViewMoved( int nScrolDir, long oldPos, long newPos );
	virtual void OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum);

	// 鼠标和键盘敲击
	virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed);
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed=0);
	virtual void OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_DClicked(RECT *rect,POINT *point,BOOL processed=0);
	
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);
	virtual void OnKeyUp(UINT *vcKey,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	
	// GetCellIndirect notification
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	// SetCell notification
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	
	// data source notifications
	virtual void OnDataSourceNotify(int ID,long msg,long param);

	// cell type notifications
	virtual int OnCellTypeNotify(long ID,int col,long row,long msg,long param);

	// 编辑
	virtual int OnEditStart(int col, long row,CWnd **edit);
	virtual int OnEditVerify(int col,long row,CWnd *edit,UINT *vcKey);
	virtual int OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual int OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow);

	// 菜单通知
	virtual void OnMenuCommand(int col,long row,int section,int item);
	virtual int  OnMenuStart(int col,long row,int section);

	// Context help 
	virtual DWORD OnGetContextHelpID( int col, long row, int section );

	// hints
	virtual int OnHint(int col,long row,int section,CString *string);
	virtual int OnVScrollHint(long row,CString *string);
	virtual int OnHScrollHint(int col,CString *string);


	#ifdef __AFXOLE_H__  // OLE must be included

	// drag and drop
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject,int col,long row);
	virtual DROPEFFECT OnDragDrop(COleDataObject* pDataObject,int col,long row);

	#endif

	// 排序
	virtual int OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);
	
	// DC setup
	virtual void OnScreenDCSetup(CDC *dc,int section);
	
	virtual void OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,RECT *cnrBtn, RECT *vScroll,RECT *hScroll,RECT *tabs);

	virtual void OnTabSelected(int ID);
	
	virtual COLORREF OnGetDefBackColor(int section);

	// focus rect setup
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnSetFocus(int section);
	virtual void OnKillFocus(int section, CWnd *pNewWnd);

	// column swapping
	virtual BOOL OnColSwapStart(int col);
	virtual BOOL OnCanColSwap(int fromCol,int toCol);
	virtual void OnColSwapped(int fromCol,int toCol);

	// trackig window
	virtual void OnTrackingWindowMoved(RECT *origRect,RECT *newRect);

public:
	/*
	功	能：依附到计算页面
	*/
	void attachToPage(SoftCrossCalculatePage* page);

	/*
	功	能：设置标题头
	*/
	void setTitle(const std::string& title = "");

	/*
	功	能：添加零件
	*/
	void addMaterial(MaterialType mt);

	/*
	功	能：添加节点
	*/
	void addNode(NodeType nt);

	/*
	功	能：设置支柱区间名称
	*/
	void setBraceRegionName(const std::string& name = "");

	/*
	功	能：添加支柱
	*/
	void addBrace(int index, BraceType bt);

	/*
	功	能：设置股道数量
	*/
	void setStationTrackNum(int num);

	/*
	功	能：添加股道间距
	*/
	void addStationTrackDistance(int index, const std::string& st = "");

	/*
	功	能：添加股道
	*/
	void addStationTrack(int index, StationTrackType stt);

	/*
	功	能：设置股道实际拉出值
	*/
	void setStationTrackReality(int index, const std::string& reality = "");

	void setLowest(int index);
	void addQ(int index, const std::string& q = "");
	void addA(int index, const std::string& a = "");
	void addK(int index, const std::string& k = "");
	void addB(int index, const std::string& b = "");
	void addLowerF1(const std::string& f1 = "");
	void addLowerF2(const std::string& f2 = "");
	void addUperF1(const std::string& f1 = "");
	void addUperF2(const std::string& f2 = "");
	void addL1(const std::string& l1 = "");
	void addL2(const std::string& l2 = "");
	void addM1(const std::string& m1 = "");
	void addM2(const std::string& m2 = "");
	void addT(const std::string& t = "");
	void addY(const std::string& y = "");

	/*
	功	能：清除数据
	*/
	void clearDatas(void);

	/*
	功	能：刷新
	*/
	void refresh(void);

	/*
	功	能：获取支柱区间名称
	*/
	const std::string& getBraceRegionName(void);

	/*
	功	能：获取支柱
	*/
	const std::vector<BraceType>& getBrace(void);

	/*
	功	能：获取股道间距
	*/
	const std::vector<std::string>& getStationTrackDistance(void);

	/*
	功	能：获取股道
	*/
	const std::vector<StationTrackType>& getStationTrack(void);

	bool isEndEdit(void) { return mIsEndEdit; }

private:
	/*
	功	能：根据文本设置单元格列宽
	*/
	void SetColWidthEx(int col, const std::string& text, bool default = false);

	/*
	功	能：设置单元格文本内容
	*/
	void QuickSetTextEx(int col, long row, const std::string str = "", bool redraw = false);

	/*
	功	能：设置单元格数字内容
	*/
	void QuickSetNumberEx(int col, long row, double num = 0.0, bool redraw = false);

	/*
	功	能：设置单元格参数
	*/
	void QuickSetParamEx(int col, int row, long param = 0);

	/*
	功	能：获取单元格文本内容
	*/
	std::string QuickGetTextEx(int col, int row);

	/*
	功	能：获取单元格数字内容
	*/
	double QuickGetNumberEx(int col, int row);

	/*
	功	能：获取单元格参数
	*/
	long QuickGetParamEx(int col, int row);

	/*
	功	能：清除所有单元格内容
	*/
	void clearGrids(int total_row, int total_col);

	/*
	功	能：刷新格子界面
	*/
	void refreshFrame(void);

	/*
	功	能：创建格子
	*/
	void createGrids(void);
	void createMaterialGrids(int start_row, size_t material_lines, size_t line_rows);
	void createNodeGrids(int start_row, size_t node_lines, size_t line_rows);
	void createInputRegionGrids(int start_row, size_t total_row);
	size_t createBraceGrids(int start_row, size_t brace_lines);
	size_t createStationTrackDistanceGrids(int start_row, size_t distance_num, size_t line_rows);
	size_t createStationTrackGrids(int start_row, size_t station_track_num, size_t line_rows);
	void createOutputRegionGrids(int start_row, size_t total_row);
	size_t createOutputLineGrids(int start_row, size_t num, size_t line_rows, std::string head);

	void setMaterial(int col, int row, const MaterialType& mt);
	void setNode(int col, int row, const NodeType& nt);

	bool editBraceFinish(long param, const std::string& str);
	bool editStationTrackDistanceFinish(long param, const std::string& str);
	bool editStationTrackRealityFinish(long param, const std::string& str);
	bool editStationTrackDesignFinish(long param, const std::string& str);
	bool editStationTrackOverstepFinish(long param, const std::string& str);

private:
	SoftCrossCalculatePage *mSoftCrossPage;					// 软横跨计算页
	int mTotalRow;											// 总行数

	// 零件
	int mMaterialStartRow;									// 已选零件开始行号（实际的行号）
	std::vector<MaterialType> mMaterials;					// 零件数据容器

	// 节点
	int mNodeStartRow;										// 节点情况开始行号
	std::vector<NodeType> mNodes;							// 节点数据容器

	// 输入区
	int mInputStartRow;										// 数值输入区开始行号

	std::string mBraceRegionName;							// 支柱区间名称
	std::vector<BraceType> mBrace;							// 支柱

	int mStationTrackDistanceStartRow;						// 股道间距开始行号
	int mStationTrackDistanceNum;							// 股道间距数量
	std::vector<std::string> mStationTrackDistance;			// 股道间距

	int mStationTrackStartRow;								// 股道开始行号
	int mStationTrackNum;									// 股道数量
	std::vector<StationTrackType> mStationTrack;			// 股道

	// 输出区
	int mOutputStartRow;									// 输出区开始行号，也是Q的开始行号
	int mQNum;												// Q的数量

	int mAStartRow;											// A的开始行号
	int mANum;												// A的数量

	int mKStartRow;											// K的开始行号
	int mKNum;												// K的数量

	int mBStartRow;											// B的开始行号
	int mBNum;												// B的数量
	
	int mZStartRow;											// 最后行的开始行号

	// 其他标识
	bool mIsEndEdit;										// 是否结束编辑
};



#endif	// _GRID_CTRL_H_


