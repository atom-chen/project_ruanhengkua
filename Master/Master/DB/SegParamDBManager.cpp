#include "StdAfx.h"
#include "SegParamDBManager.h"

CSegParamDBManager::CSegParamDBManager(void)
{
    m_db.open(CDBInit::g_pDbPath);
}

CSegParamDBManager::~CSegParamDBManager(void)
{
    m_db.close();
}

BOOL CSegParamDBManager::InitDb()
{
    CppSQLite3DB m_db;
    m_db.open(CDBInit::g_pDbPath);
    //�ɵ����� �ڵ�������У��ϲ��̶������²��̶����� �������� �Ӵ���
    //�ڵ�ѡ��Q1...QN: �������ҽڵ�_�Ҳ�����ҽڵ�_(a5-�������ҽڵ�����λ�������ڵ�ˮƽ����)_(a6-�Ҳ�����ҽڵ�����λ�������ڵ�ˮƽ����)|...|...
    int nRows = m_db.execDML("CREATE TABLE if not exists segparam(id INTEGER PRIMARY KEY AUTOINCREMENT, segId INTEGER,unitRoadNum INTEGER, nodeNum INTEGER, \
                             CrossBearId INTEGER, bDouble INTEGER, FixRopeUpId INTEGER, FixRopeBelowId INTEGER, HeadStick INTEGER, EarRingStick INTEGER, Spring INTEGER, Ear INTEGER, \
                             PillarLeft INTEGER, PillarRight INTEGER, nodeParam TEXT)");
    m_db.close();
    return nRows != 0;
}

BOOL CSegParamDBManager::Add(const SoftCrossSegParam& param)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("insert into segparam(segId,unitRoadNum,nodeNum,CrossBearId,bDouble,FixRopeUpId,FixRopeBelowId,HeadStick,EarRingStick, \
                   Spring,Ear,PillarLeft,PillarRight,nodeParam) values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s')",
                   param.m_dwSegId, param.m_dwUnitRoad, param.m_dwNodeNum, param.m_dwCrossBear, param.m_bDouble,
                   param.m_dwFixRopeUp, param.m_dwFixRopeBelow, param.m_dwHeadStick, param.m_dwEarRingStick,
                   param.m_dwSpring, param.m_dwEar,
                   param.m_dwPillarLeft, param.m_dwPillarRight, param.m_strNodeParam.c_str());
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSegParamDBManager::Update(const SoftCrossSegParam& param, DWORD dwSegId, DWORD dwType)
{
    CppSQLite3Buffer sqlBuff;
    if (dwType == UPDATETYPE_ALL)//���������ֶ�
    {
        sqlBuff.format("update segparam set unitRoadNum=%d,nodeNum=%d,CrossBearId=%d,bDouble=%d,FixRopeUpId=%d,FixRopeBelowId=%d,HeadStick=%d,EarRingStick=%d,\
                       Spring=%d,Ear=%d,PillarLeft=%d,PillarRight=%d,nodeParam='%s' where segId=%d",
                       param.m_dwUnitRoad, param.m_dwNodeNum, param.m_dwCrossBear, param.m_bDouble, param.m_dwFixRopeUp,
                       param.m_dwFixRopeBelow, param.m_dwHeadStick, param.m_dwEarRingStick,
                       param.m_dwSpring, param.m_dwEar,
                       param.m_dwPillarLeft, param.m_dwPillarRight, param.m_strNodeParam.c_str(), dwSegId);
    }
    else if (dwType == UPDATETYPE_PART)//���³���"�ڵ���Ϣ"�ֶε������ֶ�
    {
        sqlBuff.format("update segparam set unitRoadNum=%d,nodeNum=%d,CrossBearId=%d,bDouble=%d,FixRopeUpId=%d,FixRopeBelowId=%d,HeadStick=%d,EarRingStick=%d,\
                       Spring=%d,Ear=%d,PillarLeft=%d,PillarRight=%d where segId=%d",
                       param.m_dwUnitRoad, param.m_dwNodeNum, param.m_dwCrossBear, param.m_bDouble, param.m_dwFixRopeUp,
                       param.m_dwFixRopeBelow, param.m_dwHeadStick, param.m_dwEarRingStick,
                       param.m_dwSpring, param.m_dwEar, 
                       param.m_dwPillarLeft, param.m_dwPillarRight, dwSegId);
    }
    else if (dwType == UPDATETYPE_NODE)//����"�ڵ���Ϣ"�ֶ�
    {
        sqlBuff.format("update segparam set nodeParam='%s' where segId=%d", param.m_strNodeParam.c_str(), dwSegId);
    }
    else
    {
        return FALSE;
    }
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSegParamDBManager::Del(DWORD dwSegId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("delete from segparam where segId=%d", dwSegId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSegParamDBManager::GetParamBySegId(SoftCrossSegParam& param, DWORD dwSegId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select unitRoadNum,CrossBearId,FixRopeUpId,FixRopeBelowId,HeadStick,EarRingStick, \
                   Spring,Ear,nodeParam,nodeNum,PillarLeft,PillarRight,bDouble from segparam where segId=%d", dwSegId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    param.m_dwSegId = dwSegId;
    if (!q.eof())
    {
        param.m_dwUnitRoad = q.getIntField(0);
        param.m_dwCrossBear = q.getIntField(1);
        param.m_dwFixRopeUp = q.getIntField(2);
        param.m_dwFixRopeBelow = q.getIntField(3);
        param.m_dwHeadStick = q.getIntField(4);
        param.m_dwEarRingStick = q.getIntField(5);
        param.m_dwSpring = q.getIntField(6);
        param.m_dwEar = q.getIntField(7);
        param.m_strNodeParam = q.getStringField(8);
        param.m_dwNodeNum = q.getIntField(9);
        param.m_dwPillarLeft = q.getIntField(10);
        param.m_dwPillarRight = q.getIntField(11);
        param.m_bDouble = q.getIntField(12);
    }
    else
    {
        if (!Add(param))
        {
            return FALSE;
        }
    }
    return TRUE;
}