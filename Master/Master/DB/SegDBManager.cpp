#include "StdAfx.h"
#include "SegDBManager.h"

CSegDBManager::CSegDBManager(void)
{
    m_db.open(CDBInit::g_pDbPath);
}

CSegDBManager::~CSegDBManager(void)
{
    m_db.close();
}

BOOL CSegDBManager::InitDb()
{
    CppSQLite3DB m_db;
    m_db.open(CDBInit::g_pDbPath);
    int nRows = m_db.execDML("CREATE TABLE if not exists segments(id INTEGER PRIMARY KEY AUTOINCREMENT, sid INTEGER, name TEXT)");
    m_db.close();
    return nRows != 0;
}

BOOL CSegDBManager::Add(const SoftCrossSeg& seg)
{
    if (CheckSegmentName(seg.m_dwSiteId, seg.m_strName))
    {
        CppSQLite3Buffer sqlBuff;
        sqlBuff.format("insert into segments(sid, name) values(%d, '%s')", seg.m_dwSiteId, seg.m_strName);
        int nRows = m_db.execDML(sqlBuff);
        return nRows != 0;
    }
    else
    {
        return FALSE;
    }
}

BOOL CSegDBManager::Update(const SoftCrossSeg& seg, DWORD dwId)
{
	DWORD siteId = seg.m_dwSiteId;
	SoftCrossSeg temp;
	if (GetSegBySegId(temp, dwId))
	{
		siteId = temp.m_dwSiteId;
	}
    if (CheckSegmentName(siteId, seg.m_strName))
    {
        CppSQLite3Buffer sqlBuff;
        sqlBuff.format("update segments set name='%s' where id=%d", seg.m_strName, dwId);
        int nRows = m_db.execDML(sqlBuff);
        return nRows != 0;
    }
    else
    {
        return FALSE;
    }
}

BOOL CSegDBManager::Del(DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("delete from segments where id=%d", dwId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSegDBManager::GetAllSegmentBySid(vector<SoftCrossSeg>& vecSegs, DWORD dwSiteId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select id, sid, name from segments where sid=%d order by id desc", dwSiteId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    while (!q.eof())
    {
        SoftCrossSeg seg;
        seg.m_dwSegId = q.getIntField(0);
        seg.m_dwSiteId = q.getIntField(1);
        seg.m_strName = q.fieldValue(2);
        vecSegs.push_back(seg);
        q.nextRow();
    }
    return TRUE;
}

BOOL CSegDBManager::GetSegBySegId(SoftCrossSeg& seg, DWORD dwSegId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select sid, name from segments where id=%d", dwSegId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    while (!q.eof())
    {
        seg.m_dwSegId = dwSegId;
        seg.m_dwSiteId = q.getIntField(0);
        seg.m_strName = q.fieldValue(1);
        q.nextRow();
    }
    return TRUE;
}

//验证站点名是否重复，返回TRUE表示验证通过，名称不存在
BOOL CSegDBManager::CheckSegmentName(DWORD dwSiteId, const CString& strName)
{
    //调用此函数时默认m_db已打开
    vector<SoftCrossSeg> vecSegs;
	GetAllSegmentBySid(vecSegs, dwSiteId);
	for (size_t i=0; i<vecSegs.size(); ++i)
	{
		if (0 == strName.Compare(vecSegs[i].m_strName))
			return FALSE;
	}
	return TRUE;
}

DWORD CSegDBManager::GetInsertId()
{
    return (DWORD)m_db.lastRowId();
}
