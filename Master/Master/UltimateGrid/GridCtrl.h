/**********************************************************************
* ���ߣ�hezhr
* ʱ�䣺2012-09-04
* ���������ӿ�����
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

	// �ƶ��͵�����С
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

	// ���ͼ����û�
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

	// �༭
	virtual int OnEditStart(int col, long row,CWnd **edit);
	virtual int OnEditVerify(int col,long row,CWnd *edit,UINT *vcKey);
	virtual int OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual int OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow);

	// �˵�֪ͨ
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

	// ����
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
	��	�ܣ�����������ҳ��
	*/
	void attachToPage(SoftCrossCalculatePage* page);

	/*
	��	�ܣ����ñ���ͷ
	*/
	void setTitle(const std::string& title = "");

	/*
	��	�ܣ�������
	*/
	void addMaterial(MaterialType mt);

	/*
	��	�ܣ���ӽڵ�
	*/
	void addNode(NodeType nt);

	/*
	��	�ܣ�����֧����������
	*/
	void setBraceRegionName(const std::string& name = "");

	/*
	��	�ܣ����֧��
	*/
	void addBrace(int index, BraceType bt);

	/*
	��	�ܣ����ùɵ�����
	*/
	void setStationTrackNum(int num);

	/*
	��	�ܣ���ӹɵ����
	*/
	void addStationTrackDistance(int index, const std::string& st = "");

	/*
	��	�ܣ���ӹɵ�
	*/
	void addStationTrack(int index, StationTrackType stt);

	/*
	��	�ܣ����ùɵ�ʵ������ֵ
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
	��	�ܣ��������
	*/
	void clearDatas(void);

	/*
	��	�ܣ�ˢ��
	*/
	void refresh(void);

	/*
	��	�ܣ���ȡ֧����������
	*/
	const std::string& getBraceRegionName(void);

	/*
	��	�ܣ���ȡ֧��
	*/
	const std::vector<BraceType>& getBrace(void);

	/*
	��	�ܣ���ȡ�ɵ����
	*/
	const std::vector<std::string>& getStationTrackDistance(void);

	/*
	��	�ܣ���ȡ�ɵ�
	*/
	const std::vector<StationTrackType>& getStationTrack(void);

	bool isEndEdit(void) { return mIsEndEdit; }

private:
	/*
	��	�ܣ������ı����õ�Ԫ���п�
	*/
	void SetColWidthEx(int col, const std::string& text, bool default = false);

	/*
	��	�ܣ����õ�Ԫ���ı�����
	*/
	void QuickSetTextEx(int col, long row, const std::string str = "", bool redraw = false);

	/*
	��	�ܣ����õ�Ԫ����������
	*/
	void QuickSetNumberEx(int col, long row, double num = 0.0, bool redraw = false);

	/*
	��	�ܣ����õ�Ԫ�����
	*/
	void QuickSetParamEx(int col, int row, long param = 0);

	/*
	��	�ܣ���ȡ��Ԫ���ı�����
	*/
	std::string QuickGetTextEx(int col, int row);

	/*
	��	�ܣ���ȡ��Ԫ����������
	*/
	double QuickGetNumberEx(int col, int row);

	/*
	��	�ܣ���ȡ��Ԫ�����
	*/
	long QuickGetParamEx(int col, int row);

	/*
	��	�ܣ�������е�Ԫ������
	*/
	void clearGrids(int total_row, int total_col);

	/*
	��	�ܣ�ˢ�¸��ӽ���
	*/
	void refreshFrame(void);

	/*
	��	�ܣ���������
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
	SoftCrossCalculatePage *mSoftCrossPage;					// �������ҳ
	int mTotalRow;											// ������

	// ���
	int mMaterialStartRow;									// ��ѡ�����ʼ�кţ�ʵ�ʵ��кţ�
	std::vector<MaterialType> mMaterials;					// �����������

	// �ڵ�
	int mNodeStartRow;										// �ڵ������ʼ�к�
	std::vector<NodeType> mNodes;							// �ڵ���������

	// ������
	int mInputStartRow;										// ��ֵ��������ʼ�к�

	std::string mBraceRegionName;							// ֧����������
	std::vector<BraceType> mBrace;							// ֧��

	int mStationTrackDistanceStartRow;						// �ɵ���࿪ʼ�к�
	int mStationTrackDistanceNum;							// �ɵ��������
	std::vector<std::string> mStationTrackDistance;			// �ɵ����

	int mStationTrackStartRow;								// �ɵ���ʼ�к�
	int mStationTrackNum;									// �ɵ�����
	std::vector<StationTrackType> mStationTrack;			// �ɵ�

	// �����
	int mOutputStartRow;									// �������ʼ�кţ�Ҳ��Q�Ŀ�ʼ�к�
	int mQNum;												// Q������

	int mAStartRow;											// A�Ŀ�ʼ�к�
	int mANum;												// A������

	int mKStartRow;											// K�Ŀ�ʼ�к�
	int mKNum;												// K������

	int mBStartRow;											// B�Ŀ�ʼ�к�
	int mBNum;												// B������
	
	int mZStartRow;											// ����еĿ�ʼ�к�

	// ������ʶ
	bool mIsEndEdit;										// �Ƿ�����༭
};



#endif	// _GRID_CTRL_H_


