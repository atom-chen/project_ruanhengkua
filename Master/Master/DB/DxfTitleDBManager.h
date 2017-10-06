#pragma once


#ifndef _DXFTITLE_DB_MANAGER_
#define _DXFTITLE_DB_MANAGER_


#include "DBInit.h"
#include "../CommonFunction/CommonFunction.h"
#include <vector>


#define DTCPNONE _T("��")
#define DTCPKEY _T("�ؼ���")
#define DTCPUNIT _T("��λ")
#define DTCPVALUE _T("ͳ��ֵ")


enum DxfTitleCellProperty
{
	DTCP_NONE,			// ��
	DTCP_KEY,			// �ؼ���
	DTCP_UNIT,			// ��λ
	DTCP_VALUE			// ͳ��ֵ
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
			str += "(�ؼ���)";
			break;
		case DxfTitleCellProperty::DTCP_UNIT:
			str += "(��λ)";
			break;
		case DxfTitleCellProperty::DTCP_VALUE:
			str += "(ͳ��ֵ)";
			break;
		}
		return str;
	}
};



typedef struct _DXF_TITLE
{
	DWORD dwId;
	std::string titleList;			// ����ͷ(���_1|����_0|����_0|����_0|��λ_2|����_3|����_0|����_0|��ע_0)
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

