#include "StdAfx.h"
#include "DxfTitleDBManager.h"


DxfTitleDBManager::DxfTitleDBManager()
{
	mDB.open(CDBInit::g_pDbPath);
}

DxfTitleDBManager::~DxfTitleDBManager()
{
	mDB.close();
}

BOOL DxfTitleDBManager::InitDb()
{
	CppSQLite3DB db;
	db.open(CDBInit::g_pDbPath);
	int nRows = db.execDML("CREATE TABLE if not exists dxftitle(id INTEGER PRIMARY KEY AUTOINCREMENT, titles TEXT)");
	db.close();
	return nRows != 0;
}

BOOL DxfTitleDBManager::Add(const DxfTitle& title)
{
	CppSQLite3Buffer sqlBuff;
	sqlBuff.format("insert into dxftitle(titles) values('%s')", title.titleList.c_str());
	int nRows = mDB.execDML(sqlBuff);
	return nRows != 0;
}

BOOL DxfTitleDBManager::Update(const DxfTitle& title, DWORD dwId)
{
	CppSQLite3Buffer sqlBuff;
	sqlBuff.format("update dxftitle set titles='%s' where id=%d", title.titleList.c_str(), dwId);
	int nRows = mDB.execDML(sqlBuff);
	return nRows != 0;
}

BOOL DxfTitleDBManager::Del(DWORD dwId)
{
	CppSQLite3Buffer sqlBuff;
	sqlBuff.format("delete from dxftitle where id=%d", dwId);
	int nRows = mDB.execDML(sqlBuff);
	return nRows != 0;
}

std::vector<DxfTitle> DxfTitleDBManager::GetAllTitles()
{
	CppSQLite3Buffer sqlBuff;
	sqlBuff.format("select id, titles from dxftitle");
	CppSQLite3Query q = mDB.execQuery(sqlBuff);
	std::vector<DxfTitle> vecTitles;
	while (!q.eof())
	{
		DxfTitle title;
		title.dwId = q.getIntField(0);
		title.titleList = q.getStringField(1);
		vecTitles.push_back(title);
		q.nextRow();
	}
	return vecTitles;
}

DxfTitle DxfTitleDBManager::GetTitleById(DWORD dwId)
{
	CppSQLite3Buffer sqlBuff;
	sqlBuff.format("select titles from dxftitle where id=%d", dwId);
	CppSQLite3Query q = mDB.execQuery(sqlBuff);
	DxfTitle title;
	if (!q.eof())
	{
		title.dwId = dwId;
		title.titleList = q.getStringField(0);
	}
	return title;
}

std::vector<DxfTitleCell> DxfTitleDBManager::ChangeToTitleCell(const std::string& title)
{
	std::vector<std::string> titleCells = splitString(title, "|");
	std::vector<DxfTitleCell> cellVector;
	for (size_t i = 0; i<titleCells.size(); ++i)
	{
		DxfTitleCell cell;
		cell.parse(titleCells[i]);
		cellVector.push_back(cell);
	}
	return cellVector;
}

