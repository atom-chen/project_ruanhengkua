#include "StdAfx.h"
#include "PartConnectDBManager.h"
#include "../GlobalFun.h"


//----------------------------------------------------------------------
CPartConnectDBManager::CPartConnectDBManager(void)
{
	m_db.open(CDBInit::g_pDbPath);
}
//----------------------------------------------------------------------
CPartConnectDBManager::~CPartConnectDBManager(void)
{
	m_db.close();
}
//----------------------------------------------------------------------
BOOL CPartConnectDBManager::InitDb(void)
{
	CppSQLite3DB db;
	db.open(CDBInit::g_pDbPath);
	CppSQLite3Buffer buffer;
	buffer.format("CREATE TABLE if not exists PartConnectTable(id INTEGER PRIMARY KEY AUTOINCREMENT, \
		seg_id INTEGER, dw_id INTEGER, name TEXT, part_ids TEXT, part_names TEXT)");
	int res = db.execDML(buffer);
	db.close();
	return 0 != res;
}
//----------------------------------------------------------------------
BOOL CPartConnectDBManager::Add(const PartConnectParam& partConnect)
{
	CppSQLite3Buffer buffer;
	buffer.format("insert into PartConnectTable(seg_id, dw_id, name, part_ids, part_names) values('%d', '%d', '%s', '%s', '%s')", 
		partConnect.m_segId, partConnect.m_dwId, partConnect.m_name.c_str(), partConnect.m_partIds.c_str(), partConnect.m_partNames.c_str());
	int res = m_db.execDML(buffer);
	return 0 != res;
}
//----------------------------------------------------------------------
BOOL CPartConnectDBManager::Update(const PartConnectParam& partConnect)
{
	CppSQLite3Buffer buffer;
	buffer.format("update PartConnectTable set name='%s', part_ids='%s', part_names='%s' where seg_id='%d' and dw_id='%d'", 
		partConnect.m_name.c_str(), partConnect.m_partIds.c_str(), partConnect.m_partNames.c_str(), partConnect.m_segId, partConnect.m_dwId);
	int res = m_db.execDML(buffer);
	return 0 != res;
}
//----------------------------------------------------------------------
BOOL CPartConnectDBManager::Del(DWORD segId, DWORD dwId)
{
	CppSQLite3Buffer buffer;
	buffer.format("delete from PartConnectTable where seg_id='%d' and dw_id='%d'", segId, dwId);
	int res = m_db.execDML(buffer);
	return 0 != res;
}
//----------------------------------------------------------------------
PartConnectParam CPartConnectDBManager::GetPartConnectById(DWORD segId, DWORD dwId)
{
	CppSQLite3Buffer buffer;
	buffer.format("select name, part_ids, part_names from PartConnectTable where seg_id='%d' and dw_id='%d'", segId, dwId);
	CppSQLite3Query query = m_db.execQuery(buffer);
	PartConnectParam partConnect;
	if (!query.eof())
	{
		partConnect.m_segId = segId;
		partConnect.m_dwId = dwId;
		partConnect.m_name = query.getStringField(0);
		partConnect.m_partIds = query.getStringField(1);
		partConnect.m_partNames = query.getStringField(2);
	}
	return partConnect;
}
//----------------------------------------------------------------------
vector<PartConnectParam> CPartConnectDBManager::GetAllPartConnect(DWORD segId)
{
	CppSQLite3Buffer buffer;
	buffer.format("select dw_id, name, part_ids, part_names from PartConnectTable where seg_id='%d'", segId);
	CppSQLite3Query query = m_db.execQuery(buffer);
	vector<PartConnectParam> partConnectVec;
	while (!query.eof())
	{
		PartConnectParam partConnect;
		partConnect.m_segId = segId;
		partConnect.m_dwId = query.getIntField(0);
		partConnect.m_name = query.getStringField(1);
		partConnect.m_partIds = query.getStringField(2);
		partConnect.m_partNames = query.getStringField(3);
		partConnectVec.push_back(partConnect);
		query.nextRow();
	}
	return partConnectVec;
}
//----------------------------------------------------------------------
vector<DWORD> CPartConnectDBManager::GetPartIdVecById(DWORD segId, DWORD dwId)
{
	PartConnectParam partConnect = GetPartConnectById(segId, dwId);
	vector<CString> partIdVec;
	CGlobalFun::GetStringVectorByToken(partConnect.m_partIds.c_str(), partIdVec, _T('|'));
	return SeparatePartId(partIdVec);
}
//----------------------------------------------------------------------
vector<DWORD> CPartConnectDBManager::GetPartIdVec(DWORD segId)
{
	vector<PartConnectParam> partConnectVec = GetAllPartConnect(segId);
	vector<CString> partIdVec;
	for (vector<PartConnectParam>::iterator iter = partConnectVec.begin(); partConnectVec.end() != iter; ++iter)
	{
		CGlobalFun::GetStringVectorByToken(iter->m_partIds.c_str(), partIdVec, _T('|'));
	}
	return SeparatePartId(partIdVec);
}
//----------------------------------------------------------------------
BOOL CPartConnectDBManager::FindPartConnect(DWORD segId, DWORD dwId)
{
	PartConnectParam partConnect = GetPartConnectById(segId, dwId);
	return -1 != partConnect.m_dwId;
}
//----------------------------------------------------------------------
vector<DWORD> CPartConnectDBManager::SeparatePartId(const vector<CString>& partIdVec)
{
	vector<DWORD> vec;
	for (vector<CString>::const_iterator iter = partIdVec.begin(); partIdVec.end() != iter; ++iter)
	{
		DWORD partId = atoi(*iter);
		if (!FindPartId(vec, partId))
		{
			vec.push_back(partId);
		}
	}
	return vec;
}
//----------------------------------------------------------------------
BOOL CPartConnectDBManager::FindPartId(const vector<DWORD>& partIdVec, DWORD partId)
{
	for (vector<DWORD>::const_iterator iter = partIdVec.begin(); partIdVec.end() != iter; ++iter)
	{
		if (partId == *iter)
			return TRUE;
	}
	return FALSE;
}
//----------------------------------------------------------------------

