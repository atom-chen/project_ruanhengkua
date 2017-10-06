#include "StdAfx.h"
#include "NodeDBManager.h"

CNodeDBManager::CNodeDBManager(void)
{
    m_db.open(CDBInit::g_pDbPath);
}

CNodeDBManager::~CNodeDBManager(void)
{
    m_db.close();
}

BOOL CNodeDBManager::InitDb()
{
    CppSQLite3DB db;
    db.open(CDBInit::g_pDbPath);
    int nRows = db.execDML("CREATE TABLE if not exists nodes(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, weight TEXT, partlist TEXT)");
    db.close();
    return nRows != 0;
}

BOOL CNodeDBManager::Add(const SoftCrossNode& node)
{
    if (CheckName(node.m_strName))
    {
        CppSQLite3Buffer sqlBuff;
        sqlBuff.format("insert into nodes(name, weight, partlist) values('%s', '%s', '%s')", 
            node.m_strName.c_str(), node.m_strWeight.c_str(), node.m_strPartId.c_str());
        int nRows = m_db.execDML(sqlBuff);
        return nRows != 0;
    }
    else
    {
        return FALSE;
    }
}

BOOL CNodeDBManager::Update(const SoftCrossNode& node, DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("update nodes set weight='%s', partlist='%s' where id=%d", node.m_strWeight.c_str(), node.m_strPartId.c_str(), dwId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CNodeDBManager::Del(DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("delete from nodes where id=%d", dwId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CNodeDBManager::GetAllNodes(vector<SoftCrossNode>& vecNodes)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select id, name, weight, partlist from nodes order by id desc");
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    while (!q.eof())
    {
        SoftCrossNode node;
        node.m_dwId = q.getIntField(0);
        node.m_strName = q.getStringField(1);
        node.m_strWeight = q.getStringField(2);
        node.m_strPartId = q.getStringField(3);
        vecNodes.push_back(node);
        q.nextRow();
    }
    return TRUE;
}

//验证节点名是否重复，返回TRUE表示验证通过，名称不存在
BOOL CNodeDBManager::CheckName(const string& strName)
{
    //调用此函数时默认m_db已打开
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select count(*) from nodes where name='%s'", strName.c_str());
    int nRet = m_db.execScalar(sqlBuff);
    return nRet == 0;
}

BOOL CNodeDBManager::GetNodeById(SoftCrossNode& node, DWORD dwId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select name, weight, partlist from nodes where id=%d", dwId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    if (!q.eof())
    {
        node.m_dwId = dwId;
        node.m_strName = q.getStringField(0);
        node.m_strWeight = q.getStringField(1);
        node.m_strPartId = q.getStringField(2);
        return TRUE;
    }
    return FALSE;
}