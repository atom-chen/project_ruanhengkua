#include "StdAfx.h"
#include "PillarDBManager.h"

CPillarDBManager::CPillarDBManager(void)
{
    m_db.open(CDBInit::g_pDbPath);
}

CPillarDBManager::~CPillarDBManager(void)
{
    m_db.close();
}

BOOL CPillarDBManager::InitDb()
{
    CppSQLite3DB m_db;
    m_db.open(CDBInit::g_pDbPath);
    int nRows = m_db.execDML("CREATE TABLE if not exists pillar(id INTEGER PRIMARY KEY AUTOINCREMENT, \
                             type INTEGER, name TEXT, len TEXT, up TEXT, below TEXT, widthOfUp TEXT)");
    m_db.close();
    return nRows != 0;
}

BOOL CPillarDBManager::Add(const PillarParam& part)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("insert into pillar(type,name,len,up,below,widthOfUp) values('%d','%s','%s','%s','%s','%s')"
        , part.m_dwType, part.m_strName.c_str(), part.m_strLength.c_str(), part.m_strUp.c_str()
        , part.m_strBelow.c_str(), part.m_strWidthOfUp.c_str());
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CPillarDBManager::Update(const PillarParam& part, DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("update pillar set type=%d, name='%s', len='%s', \
                   up='%s', below='%s', widthOfUp='%s' where id=%d"
                   , part.m_dwType, part.m_strName.c_str(), part.m_strLength.c_str(), part.m_strUp.c_str()
                   , part.m_strBelow.c_str(), part.m_strWidthOfUp.c_str(), dwId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CPillarDBManager::Del(DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("delete from pillar where id=%d", dwId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CPillarDBManager::GetAllPillars(vector<PillarParam>& vecParts)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select name,len,up,below,widthOfUp,id,type from pillar order by id desc");
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    while (!q.eof())
    {
        PillarParam part;
        part.m_strName = q.getStringField(0);
        part.m_strLength = q.getStringField(1);
        part.m_strUp = q.getStringField(2);
        part.m_strBelow = q.getStringField(3);
        part.m_strWidthOfUp = q.getStringField(4);
        part.m_dwId = q.getIntField(5);
        part.m_dwType = q.getIntField(6);
        vecParts.push_back(part);
        q.nextRow();
    }
    return TRUE;
}

BOOL CPillarDBManager::GetPillarInfoById(PillarParam& part, DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select name,len,up,below,widthOfUp,type from pillar where id=%d", dwId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    if(!q.eof())
    {
        part.m_dwId = dwId;
        part.m_strName = q.getStringField(0);
        part.m_strLength = q.getStringField(1);
        part.m_strUp = q.getStringField(2);
        part.m_strBelow = q.getStringField(3);
        part.m_strWidthOfUp = q.getStringField(4);
        part.m_dwType = q.getIntField(5);
    }
    return TRUE;
}

//验证名称是否重复，返回TRUE表示验证通过，名称不存在
BOOL CPillarDBManager::CheckName(const CString& strName)
{
    //调用此函数时默认m_db已打开
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select count(*) from pillar where name='%s'", strName);
    int nRet = m_db.execScalar(sqlBuff);
    return nRet == 0;
}