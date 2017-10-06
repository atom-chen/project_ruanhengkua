#include "StdAfx.h"
#include "PartTypeDBManager.h"

CPartTypeDBManager::CPartTypeDBManager(void)
{
    m_db.open(CDBInit::g_pDbPath);
}

CPartTypeDBManager::~CPartTypeDBManager(void)
{
    m_db.close();
}

BOOL CPartTypeDBManager::InitDb()
{
    CppSQLite3DB m_db;
    m_db.open(CDBInit::g_pDbPath);
    BOOL bExist = m_db.tableExists("parttype");
    int nRows = m_db.execDML("CREATE TABLE if not exists parttype(id INTEGER PRIMARY KEY AUTOINCREMENT, flag INTEGER, name TEXT)");
    if (!bExist)
    {
        char* pSql = "insert into sqlite_sequence(seq, name) values(2000, 'parttype')";//设置自增ID从2000开始，防止和固定的类型冲突
        m_db.execDML(pSql);
    }

    m_db.close();
    return nRows != 0;
}

BOOL CPartTypeDBManager::Add(const PartType& seg)
{
    if (CheckName(seg.m_strName))
    {
        CppSQLite3Buffer sqlBuff;
        sqlBuff.format("insert into parttype(flag, name) values(%d, '%s')", seg.m_dwFlag, seg.m_strName);
        int nRows = m_db.execDML(sqlBuff);
        return nRows != 0;
    }
    else
    {
        return FALSE;
    }
}

BOOL CPartTypeDBManager::Update(const PartType& seg, DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("update parttype set name='%s',flag='%d' where id=%d", seg.m_strName, seg.m_dwFlag, dwId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CPartTypeDBManager::Del(DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("delete from parttype where id=%d", dwId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CPartTypeDBManager::GetAllType(vector<PartType>& vecSegs)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select id, flag, name from parttype");
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    while (!q.eof())
    {
        PartType seg;
        seg.m_dwId = q.getIntField(0);
        seg.m_dwFlag = q.getIntField(1);
        seg.m_strName = q.fieldValue(2);
        vecSegs.push_back(seg);
        q.nextRow();
    }
    return TRUE;
}

BOOL CPartTypeDBManager::GetTypeById(PartType& seg, DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select flag, name from parttype where id=%d", dwId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    while (!q.eof())
    {
        seg.m_dwId = dwId;
        seg.m_dwFlag = q.getIntField(0);
        seg.m_strName = q.fieldValue(1);
        q.nextRow();
    }
    return TRUE;
}

//验证站点名是否重复，返回TRUE表示验证通过，名称不存在
BOOL CPartTypeDBManager::CheckName(const CString& strName)
{
    //调用此函数时默认m_db已打开
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select count(*) from parttype where name='%s'", strName);
    int nRet = m_db.execScalar(sqlBuff);
    return nRet == 0;
}
