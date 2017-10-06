/**********************************************************************
* 作者：hezhr
* 时间：2012-09-23
* 描述：软横跨计算
**********************************************************************/

#include "stdafx.h"
#include "SoftCrossCalculatePage.h"
#include "atldbcli.h"
#include "DB/PartDBManager.h"
#include "DB/PillarDBManager.h"
#include "DB/NodeDBManager.h"
#include "DB/SegDBManager.h"
#include "DB/SiteDBManager.h"
#include "SegParamFormatter.h"
#include "DataValidator.h"
#include "GlobalFun.h"
#include "UltimateGrid/DataType.h"
#include "CommonFunction/CommonFunction.h"


bool SoftCrossCalculatePage::firstShowMessageBox = false;
//----------------------------------------------------------------------
SoftCrossCalculatePage::SoftCrossCalculatePage()
{
	mCalculate = NULL;
	mIsParameterInit = false;
	mIsCalcFinish = false;
	mSegmentId = -1;
	mEditSegmentId = -2;
}
//----------------------------------------------------------------------
SoftCrossCalculatePage::~SoftCrossCalculatePage()
{
	clear();
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::init(GridCtrl* grid, DWORD segment_id /* = -1 */, BOOL show /* = TRUE */)
{
	if (NULL == grid)
		return;
	
	grid->ShowWindow(SW_HIDE);
	grid->clearDatas();
	clear();
	mSegmentId = segment_id;
	
	// 通过segment_id去获取站点信息
	CSegParamDBManager segment_param_db;		// 零件,节点等参数
	if (-1 != segment_id)
	{
		segment_param_db.GetParamBySegId(mSegmentParameter, segment_id);
		initMaterialIdSet(mSegmentParameter);
	}

	// 实例计算类
	mCalculate = new CSoftCrossCompute(mSegmentParameter.m_dwUnitRoad);
	grid->setStationTrackNum(mSegmentParameter.m_dwUnitRoad);

	if (-1 == segment_id)
	{
		mSegmentTitle = "请选择分段进行计算";
		grid->setTitle(mSegmentTitle);
	}
	else
	{
		CSiteDBManager site_db;
		CSegDBManager segment_db;
		SoftCrossSeg cross_segment;
		segment_db.GetSegBySegId(cross_segment, segment_id);
		std::string site_name = site_db.GetNameById(cross_segment.m_dwSiteId).GetBuffer();
		std::string segment_name = cross_segment.m_strName.GetBuffer();
		mSegmentTitle = site_name + ":  " + segment_name;

		grid->setTitle(mSegmentTitle);
		initMaterialGrid(grid);
		initNodeGrid(grid);
	}

	if (-1 != segment_id)
	{
		CSegInputDBManager input_db;								// 数据库中的输入值
		CSegOutDBManager out_db;									// 数据库中的输出值
		input_db.GetInputBySegId(mSegmentInput, segment_id);
		out_db.GetInfoBySegId(mSegmentOut, segment_id);

		initInputGrid(grid);
	}
	grid->refresh();
	grid->ShowWindow(show ? SW_SHOW : SW_HIDE);
	if (-1 != segment_id)
	{
		bool res  = calculateEndEdit(grid);
		if (show && false == res && firstShowMessageBox)
		{
			showMessageBox();
		}
		firstShowMessageBox = false;
	}
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::clear(void)
{
	mSegmentParameter.Reset();
	mSegmentInput.Reset();
	mSegmentOut.Reset();
	mMaterialIdSet.clear();

	if (NULL != mCalculate)
	{
		delete mCalculate;
		mCalculate = NULL;
	}

	mIsParameterInit = false;
	mSegmentId = -1;
	mEditSegmentId = -2;
	mIsCalcFinish = false;
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::startEdit(void)
{
	if (-1 == mSegmentId)
		return;

	mEditSegmentId = mSegmentId;
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::endEdit(GridCtrl* grid, long param)
{
	if (NULL == grid || mEditSegmentId != mSegmentId)
		return;

	endEditBrace(grid, param);
	endEditStationTrackDistance(grid, param);
	int index_design = endEditStationTrackDesign(grid, param);
	int index_overstep = endEditStationOverstep(grid, param);
	int index = -1 == index_design ? index_overstep : index_design;
	if (-1 != index)
	{
		endEditStationReality(grid, index);
	}
	calculateEndEdit(grid);
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::endEditBrace(GridCtrl* grid, long param)
{
	if (NULL == grid)
		return;

	CSegInputDBManager input_db;										// 输入数据库
	std::vector<BraceType> brace_region = grid->getBrace();				// 支柱

	switch (param)
	{
	case BE_REGION:			// 支柱区间名称
		mSegmentInput.m_strName = grid->getBraceRegionName();
		input_db.Update(mSegmentInput, mSegmentId, INPUT_NAME);
		return;
	case BE_NUM_1:			// 左支柱号
		mSegmentInput.m_strPillarNameLeft = brace_region[0].number;
		input_db.Update(mSegmentInput, mSegmentId, INPUT_LEFTNAME);
		return;
	case BE_NUM_2:			// 右支柱号
		mSegmentInput.m_strPillarNameRight = brace_region[1].number;
		input_db.Update(mSegmentInput, mSegmentId, INPUT_RIGHTNAME);
		return;
	case BE_BOUNDS_1:		// 左支柱界限
		mSegmentInput.m_strLimitRangeLeft = brace_region[0].bounds;
		input_db.Update(mSegmentInput, mSegmentId, INPUT_LEFTLIMITRANGE);
		return;
	case BE_BOUNDS_2:		// 右支柱界限
		mSegmentInput.m_strLimitRangeRight = brace_region[1].bounds;
		input_db.Update(mSegmentInput, mSegmentId, INPUT_RIGHTLIMITRANGE);
		return;
	case BE_SLOPE_1:		// 左支柱斜率
		mSegmentInput.m_strSlopeLeft = brace_region[0].slope;
		input_db.Update(mSegmentInput, mSegmentId, INPUT_LEFTSLOPE);
		return;
	case BE_SLOPE_2:		// 右支柱斜率
		mSegmentInput.m_strSlopeRight = brace_region[1].slope;
		input_db.Update(mSegmentInput, mSegmentId, INPUT_RIGHTSLOPE);
		return;
	case BE_DIFFER_1:		// 左支柱界面差值
		mSegmentInput.m_strDeltaSurfaceLeft = brace_region[0].differ;
		input_db.Update(mSegmentInput, mSegmentId, INPUT_LEFTDELTA);
		return;
	case BE_DIFFER_2:		// 右支柱界面差值
		mSegmentInput.m_strDeltaSurfaceRight = brace_region[1].differ;
		input_db.Update(mSegmentInput, mSegmentId, INPUT_RIGHTDELTA);
		return;
	}
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::endEditStationTrackDistance(GridCtrl* grid, long param)
{
	std::vector<std::string> station_track_distance = grid->getStationTrackDistance();		// 股道间距

	if (NULL == grid || param <= STATION_TRACK_DISTANCE_BEGIN || param > STATION_TRACK_DISTANCE_BEGIN + station_track_distance.size())
		return;

	size_t index = param - STATION_TRACK_DISTANCE_BEGIN - 1;								// 索引

	CSegInFormatter in_formatter;															// 输入格式化
	in_formatter.Init(station_track_distance.size());
	CString str_out = mSegmentInput.m_strUnitDistance.c_str();
	in_formatter.SetValue(str_out, station_track_distance[index].c_str(), index);

	mSegmentInput.m_strUnitDistance = str_out.GetBuffer();

	CSegInputDBManager input_db;															// 输入数据库
	input_db.Update(mSegmentInput, mSegmentId, INPUT_UNITDISTANCE);
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::endEditStationReality(GridCtrl* grid, size_t index)
{
	std::vector<StationTrackType> station_track = grid->getStationTrack();					// 股道

	if (NULL == grid || index < 0 || index > STATION_TRACK_REALITY_BEGIN + station_track.size())
		return;

	// 计算实际拉出值
	std::string str_temp = toString(toDouble(station_track[index].design) - toDouble(station_track[index].overstep) * 4);
	CString str_reality = str_temp.c_str();

	CSegOutFormatter out_formatter;															// 输出格式化
	out_formatter.Init(station_track.size());
	out_formatter.ConvertFromString(mSegmentOut.m_strPull.c_str());
	out_formatter.SetNodeSel(str_temp, index);

	CString str_pull;
	out_formatter.ConvertToString(str_pull);
	mSegmentOut.m_strPull = str_pull.GetBuffer();

	CSegOutDBManager out_db;																// 输出数据库
	out_db.Update(mSegmentOut, mSegmentId, SEGOUT_PULL);
}
//----------------------------------------------------------------------
int SoftCrossCalculatePage::endEditStationTrackDesign(GridCtrl* grid, long param)
{
	std::vector<StationTrackType> station_track = grid->getStationTrack();					// 股道设计值

	if (NULL == grid || param <= STATION_TRACK_DESIGN_BEGIN || param > STATION_TRACK_DESIGN_BEGIN + station_track.size())
		return -1;

	size_t index = param - STATION_TRACK_DESIGN_BEGIN - 1;									// 索引

	// 保持股道设计值
	CSegInFormatter in_formatter;															// 输入格式
	in_formatter.Init(station_track.size());
	CString str_out = mSegmentInput.m_strDesign.c_str();
	in_formatter.SetValue(str_out, station_track[index].design.c_str(), index);

	mSegmentInput.m_strDesign = str_out.GetBuffer();

	CSegInputDBManager input_db;															// 输入数据库
	input_db.Update(mSegmentInput, mSegmentId, INPUT_DESIGN);
	return index;
}
//----------------------------------------------------------------------
int SoftCrossCalculatePage::endEditStationOverstep(GridCtrl* grid, long param)
{
	std::vector<StationTrackType> station_track = grid->getStationTrack();					// 股道

	if (NULL == grid || param <= STATION_TRACK_OVERSTEP_BEGIN || param > STATION_TRACK_OVERSTEP_BEGIN + station_track.size())
		return -1;

	size_t index = param - STATION_TRACK_OVERSTEP_BEGIN - 1;								// 索引

	// 保持股道超高值
	CSegInFormatter in_formatter;															// 输入格式
	in_formatter.Init(station_track.size());
	CString str_out = mSegmentInput.m_strHeightEx.c_str();
	in_formatter.SetValue(str_out, station_track[index].overstep.c_str(), index);

	mSegmentInput.m_strHeightEx = str_out.GetBuffer();

	CSegInputDBManager input_db;															// 输入数据库
	input_db.Update(mSegmentInput, mSegmentId, INPUT_HEIGHTEX);
	return index;
}
//----------------------------------------------------------------------
bool SoftCrossCalculatePage::calculateEndEdit(GridCtrl* grid)
{
	if (NULL == grid)
		return false;

	if (checkInputFinish())
	{
		if (NULL == mCalculate)
			return false;

		mCalculate->calc_soft_cross(&mInput, &mOutput);
		mIsCalcFinish = true;
		saveResult();
		updateResult(grid);
		return true;
	}
	mIsCalcFinish = false;
	return false;
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::initMaterialIdSet(const SoftCrossSegParam& segment_parameter)
{
	mMaterialIdSet.insert(segment_parameter.m_dwCrossBear);				// 横承
	mMaterialIdSet.insert(segment_parameter.m_dwFixRopeUp);				// 上部固定绳
	mMaterialIdSet.insert(segment_parameter.m_dwFixRopeBelow);			// 下部固定绳
	mMaterialIdSet.insert(segment_parameter.m_dwHeadStick);				// 杵头杆
	mMaterialIdSet.insert(segment_parameter.m_dwEarRingStick);			// 耳环杆
	mMaterialIdSet.insert(segment_parameter.m_dwSpring);				// 
	mMaterialIdSet.insert(segment_parameter.m_dwEar);					// 

	std::vector<CString> node_vec;
	CGlobalFun::GetStringVectorByToken(segment_parameter.m_strNodeParam.c_str(), node_vec, _T('|'));

	std::vector<CString>::iterator node_iter;
	for (node_iter = node_vec.begin(); node_vec.end() != node_iter; ++node_iter)
	{
		std::vector<CString> material_vec;
		CGlobalFun::GetStringVectorByToken(*node_iter, material_vec, _T('_'));

		DWORD material_id = atoi(material_vec[0]);
		if (-1 != material_id)
		{
			CNodeDBManager node_db;
			SoftCrossNode node;
			node_db.GetNodeById(node, material_id);

			std::vector<CString> material_id_vec;
			CGlobalFun::GetStringVectorByToken(node.m_strPartId.c_str(), material_id_vec, _T('|'));
			
			std::vector<CString>::iterator material_iter;
			for (material_iter = material_id_vec.begin(); material_id_vec.end() != material_iter; ++material_iter)
			{
				mMaterialIdSet.insert(atoi(*material_iter));
			}
		}
	}
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::initMaterialGrid(GridCtrl* grid)
{
	if (NULL == grid)
		return;

	MaterialType material_type;		// 材料类型
	
	// 支柱材料
	PillarParam pillar;
	CPillarDBManager pillar_db;
	
	if (-1 != mSegmentParameter.m_dwPillarLeft)			// 左支柱
	{
		pillar_db.GetPillarInfoById(pillar, mSegmentParameter.m_dwPillarLeft);
		material_type.name = pillar.m_strName;
		material_type.length = pillar.m_strLength;
		grid->addMaterial(material_type);
	}
	
	if (-1 != mSegmentParameter.m_dwPillarRight)		// 右支柱
	{
		pillar_db.GetPillarInfoById(pillar, mSegmentParameter.m_dwPillarRight);
		material_type.name = pillar.m_strName;
		material_type.length = pillar.m_strLength;
		grid->addMaterial(material_type);
	}

	// 其他材料
	PartParam material;
	CPartDBManager material_db;
	std::set<DWORD>::iterator material_id_iter;
	for (material_id_iter = mMaterialIdSet.begin(); mMaterialIdSet.end() != material_id_iter; ++material_id_iter)
	{
		material_db.GetPartInfoById(material, *material_id_iter);	// 横承
		material_type.name = material.m_strName;
		material_type.length = material.m_strLength;
		material_type.weight = material.m_strWeight;
		grid->addMaterial(material_type);
	}
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::initNodeGrid(GridCtrl* grid)
{
	if (NULL == grid)
		return;
	
	SoftCrossNode node;
	CNodeDBManager node_db;

	NodeType node_type;			// 节点类型

	std::vector<CString> node_vec;
	std::vector<CString>::iterator node_iter;
	CGlobalFun::GetStringVectorByToken(mSegmentParameter.m_strNodeParam.c_str(), node_vec, _T('|'));
	for (node_iter = node_vec.begin(); node_vec.end() != node_iter; ++node_iter)
	{
		std::vector<CString> node_id_vec;
		CGlobalFun::GetStringVectorByToken(*node_iter, node_id_vec, _T('_'));
		DWORD node_id = atoi(node_id_vec[0]);
		if (-1 != node_id)
		{
			node_db.GetNodeById(node, node_id);
			node_type.name = node.m_strName;
			node_type.weight = node.m_strWeight;
			grid->addNode(node_type);
		}
	}
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::initInputGrid(GridCtrl* grid)
{
	if (NULL == grid)
		return;

	// 支柱输入
	grid->setBraceRegionName(mSegmentInput.m_strName);

	BraceType brace_type;
	// 左支柱
	brace_type.number = mSegmentInput.m_strPillarNameLeft;
	brace_type.bounds = mSegmentInput.m_strLimitRangeLeft;
	brace_type.slope = mSegmentInput.m_strSlopeLeft;
	brace_type.differ = mSegmentInput.m_strDeltaSurfaceLeft;
	grid->addBrace(1, brace_type);
	// 右支柱
	brace_type.number = mSegmentInput.m_strPillarNameRight;
	brace_type.bounds = mSegmentInput.m_strLimitRangeRight;
	brace_type.slope = mSegmentInput.m_strSlopeRight;
	brace_type.differ = mSegmentInput.m_strDeltaSurfaceRight;
	grid->addBrace(2, brace_type);

	CSegInFormatter in_formatter;
	int station_track_num = mSegmentParameter.m_dwUnitRoad;			// 股道数
	int station_track_distance_num = station_track_num + 1;			// 股道间距数

	// 股道间距
	CString station_track_distance[MAX_UNITNUM + 1];
	in_formatter.Init(station_track_distance_num);
	in_formatter.GetValue(CString(mSegmentInput.m_strUnitDistance.c_str()), station_track_distance);
	for (size_t i = 0; i <station_track_distance_num; ++i)
	{
		grid->addStationTrackDistance(i, station_track_distance[i].GetBuffer());
	}

	// 拉出值
	CString design[MAX_UNITNUM];
	CString overlap[MAX_UNITNUM];
	in_formatter.Init(station_track_num);
	in_formatter.GetValue(CString(mSegmentInput.m_strDesign.c_str()), design);
	in_formatter.GetValue(CString(mSegmentInput.m_strHeightEx.c_str()), overlap);
	for (size_t j = 0; j < station_track_num; ++j)
	{
		StationTrackType stt;
		stt.design = design[j].GetBuffer();
		stt.overstep = overlap[j].GetBuffer();
		grid->addStationTrack(j, stt);
	}
}
//----------------------------------------------------------------------
bool SoftCrossCalculatePage::initCalculateParameter(void)
{
	///////////////////////零件/////////////////////////
	PartParam part;
	CPartDBManager part_db;
	SoftCrossSegParam segment_parameter = mSegmentParameter;
	// 横承
	part_db.GetPartInfoById(part, segment_parameter.m_dwCrossBear);
	mInput.weight_pt.cross_w_per_m = atof(part.m_strWeight.c_str()) * (segment_parameter.m_bDouble ? 2 : 1);
	// 上部固定绳
	part_db.GetPartInfoById(part, segment_parameter.m_dwFixRopeUp);
	mInput.weight_pt.up_fasten_w_per_m = atof(part.m_strWeight.c_str());
	// 下部固定绳
	part_db.GetPartInfoById(part, segment_parameter.m_dwFixRopeBelow);
	mInput.weight_pt.down_fasten_w_per_m = atof(part.m_strWeight.c_str());
	// 杵头杆
	part_db.GetPartInfoById(part, segment_parameter.m_dwHeadStick);
	mInput.component_size_t.head_stick_size = atof(part.m_strLength.c_str());
	// 耳环杆
	part_db.GetPartInfoById(part, segment_parameter.m_dwEarRingStick);
	mInput.component_size_t.ear_ring_stick_size = atof(part.m_strLength.c_str());
	// 弹簧补偿器
	part_db.GetPartInfoById(part, segment_parameter.m_dwSpring);
	mInput.component_size_t.spring_size = atof(part.m_strLength.c_str());
	// 双耳连接器
	part_db.GetPartInfoById(part, segment_parameter.m_dwEar);
	mInput.component_size_t.ear_size = atof(part.m_strLength.c_str());

	return true;
}
//----------------------------------------------------------------------
bool SoftCrossCalculatePage::initCalculateInput(void)
{
	///////////////////////支柱/////////////////////////
	PillarParam pillar;
	CPillarDBManager pillar_db;
	SoftCrossSegParam segment_parameter = mSegmentParameter;
	// 左支柱
	pillar_db.GetPillarInfoById(pillar, segment_parameter.m_dwPillarLeft);
	mInput.pillar_left.pillar_type = pillar.m_dwType;						// 支柱类型
	mInput.pillar_left.pillar_h = atof(pillar.m_strLength.c_str());			// 支柱总长
	mInput.pillar_left.pillar_bottom = atof(pillar.m_strBelow.c_str());		// 下底
	mInput.pillar_left.pillar_top = atof(pillar.m_strUp.c_str());			// 上底
	mInput.pillar_left.width_top = atof(pillar.m_strWidthOfUp.c_str());		// 顶部宽度
	// 以下几个字段需要从输入中获取
	mInput.pillar_left.limit_range = atof(mSegmentInput.m_strLimitRangeLeft.c_str());				// 限界
	mInput.pillar_left.slope = atof(mSegmentInput.m_strSlopeLeft.c_str());							// 斜率
	mInput.pillar_left.delta_of_surface = atof(mSegmentInput.m_strDeltaSurfaceLeft.c_str());		// 基础面与柱面高差
	// 右支柱
	pillar_db.GetPillarInfoById(pillar, segment_parameter.m_dwPillarRight);
	mInput.pillar_right.pillar_type = pillar.m_dwType;						// 支柱类型
	mInput.pillar_right.pillar_h = atof(pillar.m_strLength.c_str());		// 支柱总长
	mInput.pillar_right.pillar_bottom = atof(pillar.m_strBelow.c_str());	// 下底
	mInput.pillar_right.pillar_top = atof(pillar.m_strUp.c_str());			// 上底
	mInput.pillar_right.width_top = atof(pillar.m_strWidthOfUp.c_str());	// 顶部宽度
	// 以下几个字段需要从输入中获取
	mInput.pillar_right.limit_range = atof(mSegmentInput.m_strLimitRangeRight.c_str());				// 限界
	mInput.pillar_right.slope = atof(mSegmentInput.m_strSlopeRight.c_str());						// 斜率
	mInput.pillar_right.delta_of_surface = atof(mSegmentInput.m_strDeltaSurfaceRight.c_str());		// 基础面与柱面高差

	// 
	CSegInFormatter in_formatter;
	int station_track_num = mSegmentParameter.m_dwUnitRoad;				// 股道数
	int station_track_distance_num = station_track_num + 1;				// 股道间距数

	///////////////////////股道间距/////////////////////////
	CString station_track_distance[MAX_UNITNUM + 1];
	in_formatter.Init(station_track_distance_num);
	in_formatter.GetValue(CString(mSegmentInput.m_strUnitDistance.c_str()), station_track_distance);
	mInput.dist.num_of_distance = station_track_distance_num;
	if (NULL != mInput.dist.distance)
	{
		delete[] mInput.dist.distance;
		mInput.dist.distance = NULL;
	}
	mInput.dist.distance = new double[station_track_distance_num];
	if (NULL == mInput.dist.distance)
	{
		return false;
	}
	for (int i = 0; i < station_track_distance_num; ++i)
	{
		if (0 == i)
		{
			mInput.dist.distance[i] = mInput.pillar_left.limit_range;
		}
		else if (i == station_track_distance_num - 1)
		{
			mInput.dist.distance[i] = mInput.pillar_right.limit_range;
		}
		else
		{
			mInput.dist.distance[i] = atof(station_track_distance[i]);
		}
	}

	///////////////////////股道/////////////////////////
	CString design[MAX_UNITNUM];
	CString overlap[MAX_UNITNUM];
	in_formatter.Init(station_track_num);
	in_formatter.GetValue(CString(mSegmentInput.m_strDesign.c_str()), design);
	in_formatter.GetValue(CString(mSegmentInput.m_strHeightEx.c_str()), overlap);

	mInput.pull_value.num_pull_value = station_track_num;
	mInput.pull_value.design_value = atof(design[0]);

	if (NULL != mInput.pull_value.over_value)
	{
		delete[] mInput.pull_value.over_value;
		mInput.pull_value.over_value = NULL;
	}
	mInput.pull_value.over_value = new double[station_track_num];
	if (NULL == mInput.pull_value.over_value)
	{
		return false;
	}
	for (int i = 0; i < station_track_num; ++i)
	{
		mInput.pull_value.over_value[i] = atof(overlap[i]);
	}

	///////////////////////节点设置/////////////////////////
	if (NULL != mInput.q_node)
	{
		delete[] mInput.q_node;
		mInput.q_node = NULL;
	}
	mInput.node_num = segment_parameter.m_dwNodeNum;
	mInput.q_node = new node_t[segment_parameter.m_dwNodeNum];
	if (NULL == mInput.q_node)
	{
		return false;
	}
	CSegParamFormatter segment_paramter_formatter;
	segment_paramter_formatter.Init(segment_parameter.m_dwNodeNum);
	segment_paramter_formatter.ConvertFromString(segment_parameter.m_strNodeParam.c_str());

	CNodeDBManager node_db;		// 解析所有节点参数及其设置信息
	CPartDBManager part_db;
	SoftCrossNode soft_cross_node;
	for (int j = 0; j < (int)segment_parameter.m_dwNodeNum; ++j)
	{
		NodeSelData node_data;
		segment_paramter_formatter.GetNodeSel(node_data, j);
		node_db.GetNodeById(soft_cross_node, node_data.m_dwSelfId);  

		mInput.q_node[j].weight = atof(soft_cross_node.m_strWeight.c_str());
		mInput.q_node[j].dist_to_front_node = atof(node_data.m_strPreCrossLen.c_str());
		mInput.q_node[j].dist_to_rear_node = atof(node_data.m_strNextCrossLen.c_str());
		mInput.q_node[j].is_hang_node = node_data.m_dwIsHang;


		PartParam part;
		part_db.GetPartInfoById(part, node_data.m_dwCatenaryId);
		mInput.q_node[j].catenary_w_per_m = atof(part.m_strWeight.c_str());
		part_db.GetPartInfoById(part, node_data.m_dwTouchLineId);
		mInput.q_node[j].touch_line_w_per_m = atof(part.m_strWeight.c_str());
		mInput.q_node[j].distance2pillar = atof(node_data.m_strDistanceToPillar.c_str());
	}

	return true;
}
//----------------------------------------------------------------------
bool SoftCrossCalculatePage::checkInputFinish(void)
{
	if (mSegmentParameter.isAllSet())
	{
		if (!mIsParameterInit)
		{
			mIsParameterInit = initCalculateParameter();		// 初始化计算参数
		}
	}

	if (mIsParameterInit && mSegmentInput.isAllSet(mSegmentParameter.m_dwUnitRoad))
	{
		initCalculateInput();
		return true;
	}
	return false;
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::saveResult(void)
{
	CString str_temp;
	CSegOutFormatter out_formatter;
	int q_num = mSegmentParameter.m_dwUnitRoad;				// q的数量
	int akb_num = mSegmentParameter.m_dwUnitRoad + 1;		// a,k,b的数量

	// Q
	out_formatter.Init(q_num);
	for (int q = 0; q < q_num; ++q)
	{
		out_formatter.SetNodeSel(mOutput.Q[q], q);
	}
	out_formatter.ConvertToString(str_temp);
	mSegmentOut.m_strQ = str_temp;
	// A
	out_formatter.Init(akb_num);
	for (int a = 0; a < akb_num; ++a)
	{
		out_formatter.SetNodeSel(mOutput.a[a], a);
	}
	out_formatter.ConvertToString(str_temp);
	mSegmentOut.m_strA = str_temp;
	// K
	out_formatter.Init(akb_num);
	for (int k = 0; k < akb_num; ++k)
	{
		out_formatter.SetNodeSel(mOutput.k[k], k);
	}
	out_formatter.ConvertToString(str_temp);
	mSegmentOut.m_strK = str_temp;
	// B
	out_formatter.Init(akb_num);
	for (int b = 0; b < akb_num; ++b)
	{
		out_formatter.SetNodeSel(mOutput.B[b], b);
	}
	out_formatter.ConvertToString(str_temp);
	mSegmentOut.m_strB = str_temp;
	// 
	mSegmentOut.m_dwLowestIndex = mOutput.lowest_pt_idx;

	mSegmentOut.m_f1 = toString(mOutput.f[0]);
	mSegmentOut.m_f2 = toString(mOutput.f[1]);
	mSegmentOut.m_F1 = toString(mOutput.F[0]);
	mSegmentOut.m_F2 = toString(mOutput.F[1]);
	mSegmentOut.m_L1 = toString(mOutput.L[0]);
	mSegmentOut.m_L2 = toString(mOutput.L[1]);
	mSegmentOut.m_M1 = toString(mOutput.M[0]);
	mSegmentOut.m_M2 = toString(mOutput.M[1]);
	mSegmentOut.m_T = toString(mOutput.T);
	mSegmentOut.m_Y = toString(mOutput.Y);

	CSegOutDBManager segment_out_db;						// 输出数据库
	segment_out_db.Update(mSegmentOut, mSegmentId);
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::updateResult(GridCtrl* grid)
{
	if (NULL == grid)
		return;

	std::string str_temp;
	CSegOutFormatter out_formatter;
	int station_track_num = mSegmentParameter.m_dwUnitRoad;			// 股道数
	int q_num = station_track_num;									// q的数量
	int akb_num = station_track_num + 1;							// a,k,b的数量

	grid->setLowest(mOutput.lowest_pt_idx);							// 最低点颜色设置
	// Q
	out_formatter.Init(q_num);
	out_formatter.ConvertFromString(mSegmentOut.m_strQ.c_str());
	for (int q = 0; q < q_num; ++q)
	{
		out_formatter.GetNodeSel(str_temp, q);
		grid->addQ(q, str_temp);
	}
	// A
	out_formatter.Init(akb_num);
	out_formatter.ConvertFromString(mSegmentOut.m_strA.c_str());
	for (int a = 0; a < akb_num; ++a)
	{
		out_formatter.GetNodeSel(str_temp, a);
		grid->addA(a, str_temp);
	}
	// K
	out_formatter.Init(akb_num);
	out_formatter.ConvertFromString(mSegmentOut.m_strK.c_str());
	for (int k = 0; k < akb_num; ++k)
	{
		out_formatter.GetNodeSel(str_temp, k);
		grid->addK(k, str_temp);
	}
	// B
	out_formatter.Init(akb_num);
	out_formatter.ConvertFromString(mSegmentOut.m_strB.c_str());
	for (int b = 0; b < akb_num; ++b)
	{
		out_formatter.GetNodeSel(str_temp, b);
		grid->addB(b, str_temp);
	}
	// FLMTY
	grid->addLowerF1(mSegmentOut.m_f1);
	grid->addLowerF2(mSegmentOut.m_f2);
	grid->addUperF1(mSegmentOut.m_F1);
	grid->addUperF2(mSegmentOut.m_F2);
	grid->addL1(mSegmentOut.m_L1);
	grid->addL2(mSegmentOut.m_L2);
	grid->addM1(mSegmentOut.m_M1);
	grid->addM2(mSegmentOut.m_M2);
	grid->addT(mSegmentOut.m_T);
	grid->addY(mSegmentOut.m_Y);
	// 股道实际拉出值
	out_formatter.Init(station_track_num);
	out_formatter.ConvertFromString(mSegmentOut.m_strPull.c_str());
	for (size_t i = 0; i < station_track_num; ++i)
	{
		out_formatter.GetNodeSel(str_temp, i);
		grid->setStationTrackReality(i, str_temp);
	}
	grid->RedrawAll();
}
//----------------------------------------------------------------------
void SoftCrossCalculatePage::showMessageBox(void)
{
	LPCTSTR content;
#ifdef UNICODE
	content = _T("注意：在红色区域全部输入后才会进行计算。");
#else
	content = "注意：在红色区域全部输入后才会进行计算。";
#endif
	AfxMessageBox(content);
}
//----------------------------------------------------------------------
BOOL SoftCrossCalculatePage::calcAndGetDrawParam(draw_param_st* pDrawParam){
	
	if (NULL == pDrawParam || false == mIsCalcFinish)
		return FALSE;

	for (int i = 0; i< mInput.node_num; ++i)
	{
		mInput.q_node[0].upper_rope_length = 0;
		mInput.q_node[0].horizon_catenary_length = 0;
		mInput.q_node[0].lower_rope_length = 0;
	}
	mInput.designed_stright_hang_size = 0.4;
	mInput.q_node[0].upper_rope_length = 2.41;
	mInput.q_node[0].horizon_catenary_length = 1.59;
	mInput.q_node[0].lower_rope_length = 2.41;
	mInput.q_node[5].upper_rope_length = 0.88;
	mInput.q_node[5].horizon_catenary_length = 1.08;
	mInput.q_node[5].lower_rope_length = 0.88;
	mInput.q_node[mInput.node_num-1].upper_rope_length = 2.435;
	mInput.q_node[mInput.node_num-1].horizon_catenary_length = 1.59;
	mInput.q_node[mInput.node_num-1].lower_rope_length = 2.435;
 
	mCalculate->CalcDrawParam(&mInput,&mOutput);
	mCalculate->GetDrawParm(pDrawParam);
	return TRUE;
}
//----------------------------------------------------------------------

