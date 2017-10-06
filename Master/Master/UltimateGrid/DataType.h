/**********************************************************************
* ���ߣ�hezhr
* ʱ�䣺2012-9-11
* �������������Ͷ���
**********************************************************************/
#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#include <string>


/*
�������
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
	std::string name;					// ����
	std::string length;					// ����
	std::string weight;					// ����
};


/*
�ڵ�����
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
	std::string name;					// ����
	std::string length1;				// �������������
	std::string length2;				// �ϲ��̶�������
	std::string length3;				// �²��̶�������
	std::string weight;					// ����
};


/*
֧������
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
	std::string number;					// ֧����
	std::string bounds;					// ����
	std::string slope;					// б��
	std::string differ;					// ��������������
};


/*
�ɵ�����
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
	std::string reality;				// ʵ������ֵ
	std::string design;					// ���
	std::string overstep;				// ����
};


/*
֧��ö��
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



#define STATION_TRACK_DISTANCE_BEGIN			200000	// �ɵ����
#define STATION_TRACK_REALITY_BEGIN				300000	// �ɵ�ʵ������ֵ
#define STATION_TRACK_DESIGN_BEGIN				310000	// �ɵ����ֵ
#define STATION_TRACK_OVERSTEP_BEGIN			320000	// �ɵ�����ֵ



#endif	// _DATA_TYPE_H_


