#include "StdAfx.h"
#include "SegInputDBManager.h"

CSegInputDBManager::CSegInputDBManager(void)
{
    m_db.open(CDBInit::g_pDbPath);
}

CSegInputDBManager::~CSegInputDBManager(void)
{
    m_db.close();
}

BOOL CSegInputDBManager::InitDb()
{
    CppSQLite3DB m_db;
    m_db.open(CDBInit::g_pDbPath);
    //股道数，横承，上部固定绳，下部固定绳， 承力索， 接触线
    //节点选择Q1...QN: 左侧非悬挂节点_右侧非悬挂节点_(a5-左侧非悬挂节点重心位置至本节点水平距离)_(a6-右侧非悬挂节点重心位置至本节点水平距离)|...|...
    int nRows = m_db.execDML("CREATE TABLE if not exists segInput(id INTEGER PRIMARY KEY AUTOINCREMENT, segId INTEGER, name TEXT, \
                             leftName TEXT, leftLimitRane TEXT, leftSlope TEXT, leftDelta TEXT, \
                             rightName TEXT, rightLimitRane TEXT, rightSlope TEXT, rightDelta TEXT, \
                             unitDistance TEXT, design TEXT, heightEx TEXT)");
    m_db.close();
    return nRows != 0;
}

BOOL CSegInputDBManager::Add(const SoftCrossSegInput& input)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("insert into segInput(segId,name,leftName,leftLimitRane,leftSlope,leftDelta,rightName,rightLimitRane,rightSlope,rightDelta,unitDistance,design,heightEx)\
                   values(%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
                   input.m_dwSegId, input.m_strName.c_str(),
                   input.m_strPillarNameLeft.c_str(), input.m_strLimitRangeLeft.c_str(), input.m_strSlopeLeft.c_str(), input.m_strDeltaSurfaceLeft.c_str(),
                   input.m_strPillarNameRight.c_str(), input.m_strLimitRangeRight.c_str(), input.m_strSlopeRight.c_str(), input.m_strDeltaSurfaceRight.c_str(),
                   input.m_strUnitDistance.c_str(), input.m_strDesign.c_str(), input.m_strHeightEx.c_str());
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSegInputDBManager::Update(const SoftCrossSegInput& input, DWORD dwSegId, DWORD dwType)
{
    CppSQLite3Buffer sqlBuff;
    switch(dwType)
    {
    case INPUT_ALL://更新所有字段
        sqlBuff.format("update segInput set name='%s',leftName='%s',leftLimitRane='%s',leftSlope='%s',leftDelta='%s', \
                       rightName='%s',rightLimitRane='%s',rightSlope='%s',rightDelta='%s', \
                       unitDistance='%s',design='%s',heightEx='%s' where segId=%d",
                       input.m_strName.c_str(),
                       input.m_strPillarNameLeft.c_str(), input.m_strLimitRangeLeft.c_str(), input.m_strSlopeLeft.c_str(), input.m_strDeltaSurfaceLeft.c_str(),
                       input.m_strPillarNameRight.c_str(), input.m_strLimitRangeRight.c_str(), input.m_strSlopeRight.c_str(), input.m_strDeltaSurfaceRight.c_str(),
                       input.m_strUnitDistance.c_str(), input.m_strDesign.c_str(), input.m_strHeightEx.c_str(), dwSegId);
        break;
    case INPUT_LEFTNAME://更新左支柱名称
        sqlBuff.format("update segInput set leftName='%s' where segId=%d", input.m_strPillarNameLeft.c_str(), dwSegId);
        break;
    case INPUT_LEFTLIMITRANGE://更新左支柱限界
        sqlBuff.format("update segInput set leftLimitRane='%s' where segId=%d", input.m_strLimitRangeLeft.c_str(), dwSegId);
        break;
    case INPUT_LEFTSLOPE://更新左支柱斜率
        sqlBuff.format("update segInput set leftSlope='%s' where segId=%d", input.m_strSlopeLeft.c_str(), dwSegId);
        break;
    case INPUT_LEFTDELTA://更新左支柱基础面与柱面高差
        sqlBuff.format("update segInput set leftDelta='%s' where segId=%d", input.m_strDeltaSurfaceLeft.c_str(), dwSegId);
        break;
    case INPUT_RIGHTNAME://更新右支柱名称
        sqlBuff.format("update segInput set rightName='%s' where segId=%d", input.m_strPillarNameRight.c_str(), dwSegId);
        break;
    case INPUT_RIGHTLIMITRANGE://更新右支柱限界
        sqlBuff.format("update segInput set rightLimitRane='%s' where segId=%d", input.m_strLimitRangeRight.c_str(), dwSegId);
        break;
    case INPUT_RIGHTSLOPE://更新右支柱斜率
        sqlBuff.format("update segInput set rightSlope='%s' where segId=%d", input.m_strSlopeRight.c_str(), dwSegId);
        break;
    case INPUT_RIGHTDELTA://更新右支柱基础面与柱面高差
        sqlBuff.format("update segInput set rightDelta='%s' where segId=%d", input.m_strDeltaSurfaceRight.c_str(), dwSegId);
        break;
    case INPUT_UNITDISTANCE://更新股道间距
        sqlBuff.format("update segInput set unitDistance='%s' where segId=%d", input.m_strUnitDistance.c_str(), dwSegId);
        break;
    case INPUT_DESIGN://更新拉出值->设计
        sqlBuff.format("update segInput set design='%s' where segId=%d", input.m_strDesign.c_str(), dwSegId);
        break;
    case INPUT_HEIGHTEX://更新拉出值->超高
        sqlBuff.format("update segInput set heightEx='%s' where segId=%d", input.m_strHeightEx.c_str(), dwSegId);
        break;
    case INPUT_NAME://更新支柱区间名称
        sqlBuff.format("update segInput set name='%s' where segId=%d", input.m_strName.c_str(), dwSegId);
        break;
    default:
        return FALSE;
    }
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSegInputDBManager::Del(DWORD dwSegId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("delete from segInput where segId=%d", dwSegId);
    int nRows = m_db.execDML(sqlBuff);
    return nRows != 0;
}

BOOL CSegInputDBManager::GetInputBySegId(SoftCrossSegInput& input, DWORD dwSegId)
{
    CppSQLite3Buffer sqlBuff;
    sqlBuff.format("select name,leftName,leftLimitRane,leftSlope,leftDelta,rightName,rightLimitRane,rightSlope,rightDelta,unitDistance,design,heightEx \
                   from segInput where segId=%d", dwSegId);
    CppSQLite3Query q = m_db.execQuery(sqlBuff);
    input.m_dwSegId = dwSegId;
    if (!q.eof())
    {
        input.m_strName = q.getStringField(0);
        input.m_strPillarNameLeft = q.getStringField(1);
        input.m_strLimitRangeLeft = q.getStringField(2);
        input.m_strSlopeLeft = q.getStringField(3);
        input.m_strDeltaSurfaceLeft = q.getStringField(4);

        input.m_strPillarNameRight = q.getStringField(5);
        input.m_strLimitRangeRight = q.getStringField(6);
        input.m_strSlopeRight = q.getStringField(7);
        input.m_strDeltaSurfaceRight = q.getStringField(8);

        input.m_strUnitDistance = q.getStringField(9);
        input.m_strDesign = q.getStringField(10);
        input.m_strHeightEx = q.getStringField(11);
    }
    else
    {
        if (!Add(input))
        {
            return FALSE;
        }
    }
    return TRUE;
}