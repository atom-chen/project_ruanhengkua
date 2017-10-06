#include "StdAfx.h"
#include "SegOutDBManager.h"

CSegOutDBManager::CSegOutDBManager(void)
{
    m_db.open(CDBInit::g_pDbPath);
}

CSegOutDBManager::~CSegOutDBManager(void)
{
    m_db.close();
}

BOOL CSegOutDBManager::InitDb()
{
    CppSQLite3DB m_db;
    m_db.open(CDBInit::g_pDbPath);
    int nRows = m_db.execDML("CREATE TABLE if not exists segOut \
                             (id INTEGER PRIMARY KEY AUTOINCREMENT, segId INTEGER, lowestIndex INTEGER, \
                             f1 TEXT, f2 TEXT, F1_ TEXT, F2_ TEXT, L1 TEXT, L2 TEXT, M1 TEXT, M2 TEXT, \
                             T TEXT, Y TEXT, A TEXT, Q TEXT, K TEXT, pull TEXT, B TEXT)");
    m_db.close();
    return nRows != 0;
}

BOOL CSegOutDBManager::Add(const SoftCrossSegOut& output)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("insert into segOut(segId,lowestIndex,f1,f2,F1_,F2_,L1,L2,M1,M2,T,Y,A,Q,K,pull,B)\
                   values(%d,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
                   output.m_dwSegId, output.m_dwLowestIndex, output.m_f1.c_str(),
                   output.m_f2.c_str(), output.m_F1.c_str(), output.m_F2.c_str(), output.m_L1.c_str(),
                   output.m_L2.c_str(), output.m_M1.c_str(), output.m_M2.c_str(), output.m_T.c_str(),
                   output.m_Y.c_str(), output.m_strA.c_str(), output.m_strQ.c_str(), output.m_strK.c_str(),
                   output.m_strPull.c_str(), output.m_strB.c_str());
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSegOutDBManager::Update(const SoftCrossSegOut& output, DWORD dwSegId, DWORD dwType)
{
    CppSQLite3Buffer sqlBuff;
    switch(dwType)
    {
    case SEGOUT_ALL://更新所有字段
        sqlBuff.format("update segOut set lowestIndex='%d',f1='%s',f2='%s',F1_='%s',F2_='%s', \
                       L1='%s',L2='%s',M1='%s',M2='%s',T='%s',Y='%s',A='%s',Q='%s',K='%s',pull='%s',B='%s' where segId=%d",
                       output.m_dwLowestIndex, output.m_f1.c_str(), output.m_f2.c_str(), output.m_F1.c_str(),
                       output.m_F2.c_str(), output.m_L1.c_str(), output.m_L2.c_str(), output.m_M1.c_str(),
                       output.m_M2.c_str(), output.m_T.c_str(), output.m_Y.c_str(), 
                       output.m_strA.c_str(), output.m_strQ.c_str(), output.m_strK.c_str(), output.m_strPull.c_str(),
                       output.m_strB.c_str(), dwSegId);
        break;
    case SEGOUT_PULL://更新实际拉出值字段
        sqlBuff.format("update segOut set pull='%s' where segId=%d", output.m_strPull.c_str(), dwSegId);
        break;
    case SEGOUT_RES: //更新除了"实际拉出值"字段以外的所有字段
        sqlBuff.format("update segOut set lowestIndex='%d',f1='%s',f2='%s',F1_='%s',F2_='%s', \
                       L1='%s',L2='%s',M1='%s',M2='%s',T='%s',Y='%s',A='%s',Q='%s',K='%s',B='%s' where segId=%d",
                       output.m_dwLowestIndex, output.m_f1.c_str(), output.m_f2.c_str(), output.m_F1.c_str(),
                       output.m_F2.c_str(), output.m_L1.c_str(), output.m_L2.c_str(), output.m_M1.c_str(),
                       output.m_M2.c_str(), output.m_T.c_str(), output.m_Y.c_str(), 
                       output.m_strA.c_str(), output.m_strQ.c_str(), output.m_strK.c_str(), output.m_strB.c_str(), dwSegId);
        break;
    }
   
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSegOutDBManager::Del(DWORD dwSegId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("delete from segOut where segId=%d", dwSegId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSegOutDBManager::GetInfoBySegId(SoftCrossSegOut& output, DWORD dwSegId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select segId,lowestIndex,f1,f2,F1_,F2_,L1,L2,M1,M2,T,Y,A,Q,K,pull,B \
                   from segOut where segId=%d", dwSegId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    output.m_dwSegId = dwSegId;
    if (!q.eof())
    {
        output.m_dwSegId = q.getIntField(0);
        output.m_dwLowestIndex = q.getIntField(1);
        output.m_f1 = q.getStringField(2);
        output.m_f2 = q.getStringField(3);
        output.m_F1 = q.getStringField(4);

        output.m_F2 = q.getStringField(5);
        output.m_L1 = q.getStringField(6);
        output.m_L2 = q.getStringField(7);
        output.m_M1 = q.getStringField(8);

        output.m_M2 = q.getStringField(9);
        output.m_T = q.getStringField(10);
        output.m_Y = q.getStringField(11);
        output.m_strA = q.getStringField(12);
        output.m_strQ = q.getStringField(13);
        output.m_strK = q.getStringField(14);
        output.m_strPull = q.getStringField(15);
        output.m_strB = q.getStringField(16);
    }
    else
    {
        if (!Add(output))
        {
            return FALSE;
        }
    }
    return TRUE;
}