#pragma once


#ifndef _DXFTITLE_DB_MANAGER_
#define _DXFTITLE_DB_MANAGER_


#include "DBInit.h"
#include "../CommonFunction/CommonFunction.h"
#include <vector>


#define DTCPNONE _T("空")
#define DTCPKEY _T("关键字")
#define DTCPUNIT _T("单位")
#define DTCPVALUE _T("统计值")


enum DxfTitleCellProperty
{
	DTCP_NONE,			// 空
	DTCP_KEY,			// 关键字
	DTCP_UNIT,			// 单位
	DTCP_VALUE			// 统计值
};

class DxfTitleCell
{
public:
	std::string title;
	DxfTitleCellProperty property;

	DxfTitleCell() : title(""), property(DxfTitleCellProperty::DTCP_NONE) {}
	void parse(std::string dbStr)
	{
		std::vector<std::string> vec = splitString(dbStr, "_");
		if (2 != vec.size())
			return;

		title = vec[0];
		property = (DxfTitleCellProperty)toInt(vec[1]);
	}

	std::string toDBString(void)
	{
		return title + "_" + toString(property);
	}

	std::string toShowString(void)
	{
		std::string str = title;
		switch (property)
		{
		case DxfTitleCellProperty::DTCP_KEY:
			str += "(关键字)";
			break;
		case DxfTitleCellProperty::DTCP_UNIT:
			str += "(单位)";
			break;
		case DxfTitleCellProperty::DTCP_VALUE:
			str += "(统计值)";
			break;
		}
		return str;
	}
};



typedef struct _DXF_TITLE
{
	DWORD dwId;
	std::string titleList;			// 标题头(序号_1|代号_0|名称_0|材料_0|单位_2|数量_3|单重_0|共重_0|附注_0)
}DxfTitle;



class DxfTitleDBManager
{
public:
	DxfTitleDBManager();
	~DxfTitleDBManager();

public:
	static BOOL InitDb();
	BOOL Add(const DxfTitle& title);
	BOOL Update(const DxfTitle& title, DWORD dwId);
	BOOL Del(DWORD dwId);
	std::vector<DxfTitle> GetAllTitles();
	DxfTitle GetTitleById(DWORD dwId);
	std::vector<DxfTitleCell> ChangeToTitleCell(const std::string& title);

private:
	CppSQLite3DB mDB;
};


#endif	// _DXFTITLE_DB_MANAGER_

