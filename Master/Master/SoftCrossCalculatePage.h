/**********************************************************************
* 作者：hezhr
* 时间：2012-09-23
* 描述：软横跨计算
**********************************************************************/

#ifndef _SOFT_CROSS_CALCULATE_
#define _SOFT_CROSS_CALCULATE_

#include "DB/SegInputDBManager.h"
#include "DB/SegParamDBManager.h"
#include "DB/SegInFormatter.h"
#include "DB/SegOutDBManager.h"
#include "DB/SegOutFormatter.h"
#include "SoftCrossCompute.h"
#include "UltimateGrid/GridCtrl.h"
#include <string>
#include <set>


class SoftCrossCalculatePage
{
public:
	SoftCrossCalculatePage();
	~SoftCrossCalculatePage();

public:
	/*
	功	能：初始计算页
	参	数：grid - 格子界面；segment_id - 分段id；show - 是否显示
	返回值：void
	*/
	void init(GridCtrl* grid, DWORD segment_id = -1, BOOL show = TRUE);

	/*
	功	能：开始编辑
	参	数：void
	返回值：void
	*/
	void startEdit(void);

	/*
	功	能：结束编辑
	参	数：grid - 格子界面；param - 格子参数
	返回值：void
	*/
	void endEdit(GridCtrl* grid, long param);
	/*
	功	能：计算绘图并得到绘图参数
	参	数：pDrawParam - 得到的绘图参数
	返回值：TRUE - 表示获得成功；FALSE - 失败	
	*/
	BOOL calcAndGetDrawParam(draw_param_st* pDrawParam);

	static bool firstShowMessageBox;

private:
	/*
	功	能：清除计算页
	参	数：void
	返回值：void
	*/
	void clear(void);

	/*
	功	能：初始化零件
	*/
	void initMaterialIdSet(const SoftCrossSegParam& segment_parameter);

	/*
	功	能：初始化零件格子区域
	*/
	void initMaterialGrid(GridCtrl* grid);

	/*
	功	能：初始化节点格子区域
	*/
	void initNodeGrid(GridCtrl* grid);

	/*
	功	能：初始化输入格子区域
	*/
	void initInputGrid(GridCtrl* grid);

	/*
	功	能：初始化计算的参数
	*/
	bool initCalculateParameter(void);

	/*
	功	能：计算单元格中输入的数据参数
	*/
	bool initCalculateInput(void);

	/*
	功	能：验证是否所有需要输入的单元格已输入
	*/
	bool checkInputFinish();

	/*
	功	能：保存结果
	*/
	void saveResult(void);

	/*
	功	能：更新显示结果
	*/
	void updateResult(GridCtrl* grid);

	/*
	功	能：结束支柱编辑
	*/
	void endEditBrace(GridCtrl* grid, long param);

	/*
	功	能：结束股道间距编辑
	*/
	void endEditStationTrackDistance(GridCtrl* grid, long param);

	/*
	功	能：结束股道设计值编辑
	*/
	int endEditStationTrackDesign(GridCtrl* grid, long param);

	/*
	功	能：结束股道超高值编辑
	*/
	int endEditStationOverstep(GridCtrl* grid, long param);

	/*
	功	能：计算股道实际拉出值
	*/
	void endEditStationReality(GridCtrl* grid, size_t index);

	/*
	功	能：结束编辑
	*/
	bool calculateEndEdit(GridCtrl* grid);
	

	void showMessageBox(void);

private:
	CSoftCrossCompute *mCalculate;				// 计算类
	bool mIsParameterInit;						// 设置的参数是否已经被初始化
	bool mIsCalcFinish;							// 是否已经得到计算结果
	DWORD mSegmentId;							// 分段id
	DWORD mEditSegmentId;						// 当前编辑的分段id,防止在输入时直接切换分段导致的数值保存到被切换的分段上
	std::string mSegmentTitle;					// 分段标题
	SoftCrossSegParam mSegmentParameter;		// 分段参数
	SoftCrossSegInput mSegmentInput;			// 分段输入
	SoftCrossSegOut mSegmentOut;				// 分段输出
	std::set<DWORD> mMaterialIdSet;				// 材料id集合
	input_t mInput;								// 输入
	output_t mOutput;							// 输出
};


#endif	// _SOFT_CROSS_CALCULATE_


