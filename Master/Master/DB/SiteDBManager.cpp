#include "StdAfx.h"
#include "SiteDBManager.h"

CSiteDBManager::CSiteDBManager(void)
{
    m_db.open(CDBInit::g_pDbPath);
}

CSiteDBManager::~CSiteDBManager(void)
{
    m_db.close();
}

BOOL CSiteDBManager::InitDb()
{
    CppSQLite3DB m_db;
    char* pSql = "CREATE TABLE if not exists sites(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);";
    m_db.open(CDBInit::g_pDbPath);
    int nRows = m_db.execDML(pSql);
    m_db.close();
    return nRows != 0;
}

BOOL CSiteDBManager::AddSite(const SoftCrossSite& site)
{
    if (CheckSiteName(site.m_strName))
    {
        CppSQLite3Buffer sqlBuff;
        sqlBuff.format("insert into sites(name) values('%s')", site.m_strName);
        int nRows = m_db.execDML(sqlBuff);
        return nRows != 0;
    }
    else
    {
        return FALSE;
    }
}

BOOL CSiteDBManager::UpdateSite(const SoftCrossSite& site, DWORD dwId)
{
    if (CheckSiteName(site.m_strName))
    {
        CppSQLite3Buffer sqlBuff;
        sqlBuff.format("update sites set name='%s' where id=%d", site.m_strName, dwId);
        int nRows = m_db.execDML(sqlBuff);
        return nRows != 0;
    }
    else
    {
        return FALSE;
    }
}

BOOL CSiteDBManager::DelSite(DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("delete from sites where id=%d", dwId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSiteDBManager::GetAllSite(vector<SoftCrossSite>& vecSites)
{
    CppSQLite3Query q = m_db.execQuery("select id, name from sites order by id desc;");
    while (!q.eof())
    {
        SoftCrossSite site;
        site.m_dwSiteId = q.getIntField(0);
        site.m_strName = q.fieldValue(1);
        vecSites.push_back(site);
        q.nextRow();
    }
    return TRUE;
}

CString CSiteDBManager::GetNameById(DWORD dwSiteId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select name from sites where id=%d", dwSiteId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    CString strName;
    while (!q.eof())
    {
        strName = q.getStringField(0);
        q.nextRow();
    }
    return strName;
}

//验证站点名是否重复，返回TRUE表示验证通过，名称不存在
BOOL CSiteDBManager::CheckSiteName(const CString& strName)
{
    //调用此函数时默认m_db已打开
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select count(*) from sites where name='%s'", strName);
    int nRet = m_db.execScalar(sqlBuff);
    return nRet == 0;
}
