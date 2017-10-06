#pragma once

//�ֶμ���ĳ�ʼ����
//�ɵ��������ѡ�񣬶�Ӧ�ڵ�

#include "DBInit.h"
#include "../SegParamFormatter.h"
#include <vector>
using namespace std;

#define UPDATETYPE_ALL 0
#define UPDATETYPE_PART 1
#define UPDATETYPE_NODE 2

//�ֶ�����
typedef struct _SOFTCROSS_SEGPARAM
{
    DWORD   m_dwId;//ID
    DWORD   m_dwSegId;//�����ķֶ�ID
    DWORD   m_dwUnitRoad;//�ɵ���
    DWORD   m_dwNodeNum;//�ڵ�����
    DWORD   m_dwCrossBear;//���
    BOOL    m_bDouble;//�Ƿ�ʹ��˫���
    DWORD   m_dwFixRopeUp;//�ϲ��̶���
    DWORD   m_dwFixRopeBelow;//�²��̶���
    DWORD   m_dwPillarLeft;//��֧��
    DWORD   m_dwPillarRight;//��֧��
    DWORD   m_dwHeadStick;//��ͷ��
    DWORD   m_dwEarRingStick;//������
    DWORD   m_dwSpring;//������
    DWORD   m_dwEar;//������
    string  m_strNodeParam;//���еĽڵ���Ϣ��ɵ��ַ���

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

        NodeSelData nodeSelData;//�ڵ�������Ϣ-�Ƿ�����
        int nHangNodeCount = 0;//�����������ж����ҽڵ�ĸ����Ƿ�һ��
        for (int i = 0; i < (int)m_dwNodeNum; i++)
        {
            fmt.GetNodeSel(nodeSelData, i);
            if (0 == i || m_dwNodeNum - 1 == i)
            {
                //ͷβ2�ڵ�Ϊ֧���ķ����ҽڵ�(q1��࣬qn�Ҳ�)
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

//�ֶ����ݿ����
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