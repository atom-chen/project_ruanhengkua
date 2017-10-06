/**********************************************************************
* 作者：hezhr
* 时间：2012-9-11
* 描述：数据类型定义
**********************************************************************/
#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#include <string>


/*
零件类型
*/
class MaterialType
{
public:
	MaterialType(std::string na = "", std::string len = "", std::string we = "")
	:name(na)
	,length(len)
	,weight(we)
	{}

public:
	std::string name;					// 名称
	std::string length;					// 长度
	std::string weight;					// 重量
};


/*
节点类型
*/
class NodeType
{
public:
	NodeType(std::string na = "", std::string len1 = "", std::string len2 = "", std::string len3 = "", std::string we = "")
	:name(na)
	,length1(len1)
	,length2(len2)
	,length3(len3)
	,weight(we)
	{}

public:
	std::string name;					// 名称
	std::string length1;				// 横向承力索长度
	std::string length2;				// 上部固定绳长度
	std::string length3;				// 下部固定绳长度
	std::string weight;					// 重量
};


/*
支柱类型
*/
class BraceType
{
public:
	BraceType(std::string num = "", std::string bo = "", std::string sl = "", std::string di = "")
	:number(num)
	,bounds(bo)
	,slope(sl)
	,differ(di)
	{}

public:
	std::string number;					// 支柱号
	std::string bounds;					// 界限
	std::string slope;					// 斜率
	std::string differ;					// 基础面与最高面差
};


/*
股道类型
*/
class StationTrackType
{
public:
	StationTrackType(std::string re = "", std::string de = "", std::string ov = "")
	:reality(re)
	,design(de)
	,overstep(ov)
	{}

public:
	std::string reality;				// 实际拉出值
	std::string design;					// 设计
	std::string overstep;				// 超高
};


/*
支柱枚举
*/
enum BraceEnum
{
	BE_BEGIN = 100000,
	BE_REGION,
	BE_NUM_1,
	BE_NUM_2,
	BE_BOUNDS_1,
	BE_BOUNDS_2,
	BE_SLOPE_1,
	BE_SLOPE_2,
	BE_DIFFER_1,
	BE_DIFFER_2,
	BE_END
};



#define STATION_TRACK_DISTANCE_BEGIN			200000	// 股道间距
#define STATION_TRACK_REALITY_BEGIN				300000	// 股道实际拉出值
#define STATION_TRACK_DESIGN_BEGIN				310000	// 股道设计值
#define STATION_TRACK_OVERSTEP_BEGIN			320000	// 股道超高值



#endif	// _DATA_TYPE_H_


