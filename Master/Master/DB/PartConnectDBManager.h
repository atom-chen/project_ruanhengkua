#pragma once

#include <vector>
#include "DBInit.h"

using namespace std;


//零件连接情况数据
class PartConnectParam
{
public:
	PartConnectParam(void)
	{
		m_dwId = -1;
	}

public:
	DWORD m_segId;				// 分段id
	DWORD m_dwId;				// 连接情况id,0:横承,1:上部固定绳,2:下部固定绳
	string m_name;				// 连接情况名称
	string m_partIds;			// 零件id串
	string m_partNames;			// 零件名字串
};


//零件连接情况数据库管理
class CPartConnectDBManager
{
public:
	CPartConnectDBManager(void);
	~CPartConnectDBManager(void);

public:
	// 初始数据库
	static BOOL InitDb(void);
	// 添加数据
	BOOL Add(const PartConnectParam& partConnect);
	// 更新数据
	BOOL Update(const PartConnectParam& partConnect);
	// 删除数据
	BOOL Del(DWORD segId, DWORD dwId);
	// 获取单条数据
	PartConnectParam GetPartConnectById(DWORD segId, DWORD dwId);
	// 数据全部数据
	vector<PartConnectParam> GetAllPartConnect(DWORD segId);
	// 获取单条数据的零件id(不重复)
	vector<DWORD> GetPartIdVecById(DWORD segId, DWORD dwId);
	// 获取全部数据的零件id(不重复)
	vector<DWORD> GetPartIdVec(DWORD segId);
	// 是否存在某条数据
	BOOL FindPartConnect(DWORD segId, DWORD dwId);

private:
	// 从零件id字符数组中获取不重复的零件id
	vector<DWORD> SeparatePartId(const vector<CString>& partIdVec);
	// 从数组中查找是否存在零件id
	BOOL FindPartId(const vector<DWORD>& partIdVec, DWORD partId);

private:
	CppSQLite3DB m_db;
};


