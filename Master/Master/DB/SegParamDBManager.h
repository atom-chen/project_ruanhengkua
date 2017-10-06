#pragma once

//分段计算的初始参数
//股道数，零件选择，对应节点

#include "DBInit.h"
#include "../SegParamFormatter.h"
#include <vector>
using namespace std;

#define UPDATETYPE_ALL 0
#define UPDATETYPE_PART 1
#define UPDATETYPE_NODE 2

//分段数据
typedef struct _SOFTCROSS_SEGPARAM
{
    DWORD   m_dwId;//ID
    DWORD   m_dwSegId;//所属的分段ID
    DWORD   m_dwUnitRoad;//股道数
    DWORD   m_dwNodeNum;//节点总数
    DWORD   m_dwCrossBear;//横承
    BOOL    m_bDouble;//是否使用双横承
    DWORD   m_dwFixRopeUp;//上部固定绳
    DWORD   m_dwFixRopeBelow;//下部固定绳
    DWORD   m_dwPillarLeft;//左支柱
    DWORD   m_dwPillarRight;//右支柱
    DWORD   m_dwHeadStick;//杵头杆
    DWORD   m_dwEarRingStick;//耳环杆
    DWORD   m_dwSpring;//补偿器
    DWORD   m_dwEar;//连接器
    string  m_strNodeParam;//所有的节点信息组成的字符串

    _SOFTCROSS_SEGPARAM()
    {
        Reset();
    }
    void Reset()
    {
        m_dwUnitRoad = 5;
        m_dwNodeNum = 14;
        m_dwCrossBear = m_dwFixRopeUp = m_dwFixRopeBelow
            = m_dwPillarLeft = m_dwPillarRight 
            = m_dwHeadStick = m_dwEarRingStick 
            = m_dwSpring = m_dwEar = -1;
        m_bDouble = TRUE;
        m_strNodeParam.clear();
    }
    BOOL isAllSet()
    {
        if (-1 == m_dwCrossBear
            || -1 == m_dwFixRopeUp
            || -1 == m_dwFixRopeBelow
            //|| -1 == m_dwHeadStick
            //|| -1 == m_dwEarRingStick
            //|| -1 == m_dwSpring
            //|| -1 == m_dwEar
            || -1 == m_dwPillarLeft
            || -1 == m_dwPillarRight
            || m_dwNodeNum < m_dwUnitRoad)
        {
            return FALSE;
        }
        CSegParamFormatter fmt;
        fmt.Init(m_dwNodeNum);
        fmt.ConvertFromString(m_strNodeParam.c_str());

        NodeSelData nodeSelData;//节点设置信息-是否悬挂
        int nHangNodeCount = 0;//计数，用于判断悬挂节点的个数是否一致
        for (int i = 0; i < (int)m_dwNodeNum; i++)
        {
            fmt.GetNodeSel(nodeSelData, i);
            if (0 == i || m_dwNodeNum - 1 == i)
            {
                //头尾2节点为支柱的非悬挂节点(q1左侧，qn右侧)
                if (nodeSelData.m_dwIsHang)
                {
                    return FALSE;
                }
            }
            if (nodeSelData.m_dwIsHang)
            {
                nHangNodeCount++;
            }     
        }

        if(nHangNodeCount != m_dwUnitRoad)
        {
            return FALSE;
        }

        return TRUE;
    }
}SoftCrossSegParam;

//分段数据库管理
class CSegParamDBManager
{
public:
    CSegParamDBManager(void);
    ~CSegParamDBManager(void);

public:
    static BOOL InitDb();
    BOOL Add(const SoftCrossSegParam& param);
    BOOL Update(const SoftCrossSegParam& param, DWORD dwSegId, DWORD dwType = UPDATETYPE_ALL);
    BOOL Del(DWORD dwSegId);
    BOOL GetParamBySegId(SoftCrossSegParam& param, DWORD dwSegId);
    
private:
    CppSQLite3DB m_db;
};