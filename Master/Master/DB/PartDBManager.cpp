#include "StdAfx.h"
#include "PartDBManager.h"

CPartDBManager::CPartDBManager(void)
{
    m_db.open(CDBInit::g_pDbPath);
}

CPartDBManager::~CPartDBManager(void)
{
    m_db.close();
}

BOOL CPartDBManager::InitDb()
{
    CppSQLite3DB m_db;
    m_db.open(CDBInit::g_pDbPath);
    int nRows = m_db.execDML("CREATE TABLE if not exists parts(id INTEGER PRIMARY KEY AUTOINCREMENT \
        , tid INTEGER, flag INTEGER, name TEXT, weight TEXT, len TEXT)");
    m_db.close();
    return nRows != 0;
}

BOOL CPartDBManager::Add(const PartParam& part)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("insert into parts(tid,name,flag,weight,len) values(%d,'%s',%d,'%s','%s')"
        , part.m_dwTypeId, part.m_strName.c_str(), part.m_dwFlag, part.m_strWeight.c_str()
        , part.m_strLength.c_str());
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CPartDBManager::Update(const PartParam& part, DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("update parts set tid=%d, name='%s', flag=%d, weight='%s', len='%s' where id=%d"
                    , part.m_dwTypeId, part.m_strName.c_str(), part.m_dwFlag
                    , part.m_strWeight.c_str(), part.m_strLength.c_str(), dwId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CPartDBManager::Del(DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("delete from parts where id=%d", dwId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CPartDBManager::GetPartsByTypeId(vector<PartParam>& vecParts, DWORD dwTypeId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select name,flag,weight,len,id from parts where tid=%d order by id desc", dwTypeId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    while (!q.eof())
    {
        PartParam part;
        part.m_dwTypeId = dwTypeId;
        part.m_strName = q.getStringField(0);
        part.m_dwFlag = q.getIntField(1);
        part.m_strWeight = q.getStringField(2);
        part.m_strLength = q.getStringField(3);
        part.m_dwId = q.getIntField(4);
        vecParts.push_back(part);
        q.nextRow();
    }
    return TRUE;
}

BOOL CPartDBManager::GetPartInfoById(PartParam& part, DWORD dwPartId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select name,flag,weight,len,tid from parts where id=%d", dwPartId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    if(!q.eof())
    {
        part.m_dwId = dwPartId;
        part.m_strName = q.getStringField(0);
        part.m_dwFlag = q.getIntField(1);
        part.m_strWeight = q.getStringField(2);
        part.m_strLength = q.getStringField(3);
        part.m_dwTypeId = q.getIntField(4);
    }
    return TRUE;
}

BOOL CPartDBManager::GetParts(vector<PartParam>& vecParts)
{
	CppSQLite3Buffer sqlBuff;
	sqlBuff.format("select * from parts");
	CppSQLite3Query q = m_db.execQuery(sqlBuff);
	while (!q.eof())
	{
		PartParam part;
		part.m_dwId = q.getIntField(0);
		part.m_dwTypeId = q.getIntField(1);
		part.m_dwFlag = q.getIntField(2);
		part.m_strName = q.getStringField(3);
		part.m_strWeight = q.getStringField(4);
		part.m_strLength = q.getStringField(5);
		vecParts.push_back(part);
		q.nextRow();
	}
	return TRUE;
}

CString CPartDBManager::GetNameById(DWORD dwPartId)
{
    CString strName = _T("");
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select name from parts where id=%d", dwPartId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    if(!q.eof())
    {
        strName = q.getStringField(0);
    }
    return strName;
}