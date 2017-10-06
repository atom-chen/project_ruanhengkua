#pragma once

//分段计算的初始参数
//股道数，零件选择，对应节点

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

#define MAX_UNITNUM 20//最大的股道数

//分段输入数据
typedef struct _SOFTCROSS_SEGINPUT
{
    DWORD   m_dwId;//ID
    DWORD   m_dwSegId;//所属的分段ID
    string  m_strName;//支柱区间名称

    string  m_strPillarNameLeft;//左支柱
    string  m_strLimitRangeLeft;//限界
    string  m_strSlopeLeft;//斜率
    string  m_strDeltaSurfaceLeft;//基础面与柱面高差

    string  m_strPillarNameRight;//右支柱
    string  m_strLimitRangeRight;//限界
    string  m_strSlopeRight;//斜率
    string  m_strDeltaSurfaceRight;//基础面与柱面高差

    string  m_strUnitDistance;//股道间距
    string  m_strDesign;//拉出值->设计
    string  m_strHeightEx;//拉出值->超高
    
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

    //dwHangNum=>悬挂点数
    BOOL isAllSet(DWORD dwHangNum)
    {
        //任何一处为空即表示未全部输入
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

//分段输入数据库管理
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