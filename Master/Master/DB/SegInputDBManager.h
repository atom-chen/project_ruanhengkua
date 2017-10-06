#pragma once

//�ֶμ���ĳ�ʼ����
//�ɵ��������ѡ�񣬶�Ӧ�ڵ�

#include "DBInit.h"
#include "SegInFormatter.h"
#include <vector>
using namespace std;

#define INPUT_ALL 0
#define INPUT_LEFTNAME 1
#define INPUT_LEFTLIMITRANGE 2
#define INPUT_LEFTSLOPE 3
#define INPUT_LEFTDELTA 4
#define INPUT_RIGHTNAME 5
#define INPUT_RIGHTLIMITRANGE 6
#define INPUT_RIGHTSLOPE 7
#define INPUT_RIGHTDELTA 8
#define INPUT_UNITDISTANCE 9
#define INPUT_DESIGN 10
#define INPUT_HEIGHTEX 11
#define INPUT_NAME 12

#define MAX_UNITNUM 20//���Ĺɵ���

//�ֶ���������
typedef struct _SOFTCROSS_SEGINPUT
{
    DWORD   m_dwId;//ID
    DWORD   m_dwSegId;//�����ķֶ�ID
    string  m_strName;//֧����������

    string  m_strPillarNameLeft;//��֧��
    string  m_strLimitRangeLeft;//�޽�
    string  m_strSlopeLeft;//б��
    string  m_strDeltaSurfaceLeft;//������������߲�

    string  m_strPillarNameRight;//��֧��
    string  m_strLimitRangeRight;//�޽�
    string  m_strSlopeRight;//б��
    string  m_strDeltaSurfaceRight;//������������߲�

    string  m_strUnitDistance;//�ɵ����
    string  m_strDesign;//����ֵ->���
    string  m_strHeightEx;//����ֵ->����
    
    _SOFTCROSS_SEGINPUT()
    {
        Reset();
    }

    void Reset()
    {
        m_dwId = m_dwSegId = -1;
        m_strName.clear();
        m_strPillarNameLeft.clear();
        m_strLimitRangeLeft.clear();
        m_strSlopeLeft.clear();
        m_strDeltaSurfaceLeft.clear();
        m_strPillarNameRight.clear();
        m_strLimitRangeRight.clear();
        m_strSlopeRight.clear();
        m_strDeltaSurfaceRight.clear();
        m_strUnitDistance.clear();
        m_strDesign.clear();
        m_strHeightEx.clear();
    }

    //dwHangNum=>���ҵ���
    BOOL isAllSet(DWORD dwHangNum)
    {
        //�κ�һ��Ϊ�ռ���ʾδȫ������
        if (m_strPillarNameLeft.empty()
            || m_strLimitRangeLeft.empty()
            || m_strLimitRangeLeft.empty()
            || m_strSlopeLeft.empty()
            || m_strPillarNameRight.empty()
            || m_strLimitRangeRight.empty()
            || m_strSlopeRight.empty()
            || m_strDeltaSurfaceRight.empty())
        {
            return FALSE;
        }
        CSegInFormatter fmt;
        CString arrUnitDistance[MAX_UNITNUM + 1];
        fmt.Init(dwHangNum + 1);
        fmt.GetValue(CString(m_strUnitDistance.c_str()), arrUnitDistance);
        for (DWORD i = 0; i < dwHangNum + 1; i++)
        {
            if (arrUnitDistance[i].IsEmpty())
            {
                return FALSE;
            }
        }

        CString arrDesign[MAX_UNITNUM + 1];
        CString arrHeightEx[MAX_UNITNUM + 1];
        fmt.Init(dwHangNum);
        fmt.GetValue(CString(m_strDesign.c_str()), arrDesign);
        fmt.GetValue(CString(m_strHeightEx.c_str()), arrHeightEx);
        for (DWORD i = 0; i < dwHangNum; i++)
        {
            if (arrDesign[i].IsEmpty() || arrHeightEx[i].IsEmpty())
            {
                return FALSE;
            }
        }
        return TRUE;
    }
}SoftCrossSegInput;

//�ֶ��������ݿ����
class CSegInputDBManager
{
public:
    CSegInputDBManager(void);
    ~CSegInputDBManager(void);

public:
    static BOOL InitDb();
    BOOL Add(const SoftCrossSegInput& input);
    BOOL Update(const SoftCrossSegInput& input, DWORD dwSegId, DWORD dwType = INPUT_ALL);
    BOOL Del(DWORD dwSegId);
    BOOL GetInputBySegId(SoftCrossSegInput& input, DWORD dwSegId);
    
private:
    CppSQLite3DB m_db;
};