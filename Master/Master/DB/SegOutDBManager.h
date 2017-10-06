#pragma once

//分段计算的初始参数
//股道数，零件选择，对应节点

#include "DBInit.h"
#include <vector>
using namespace std;

#define SEGOUT_ALL 0
#define SEGOUT_PULL 1
#define SEGOUT_RES 2

//分段输出数据
typedef struct _SOFTCROSS_SEGOUT
{
    DWORD   m_dwId;//ID
    DWORD   m_dwSegId;//所属的分段ID
    string  m_f1;
    string  m_f2;
    string  m_F1;
    string  m_F2;
    string  m_L1;
    string  m_L2;
    string  m_M1;
    string  m_M2;
    string  m_T;
    string  m_Y;
    string  m_strA;
    string  m_strQ;
    string  m_strK;
    string  m_strB;
    string  m_strPull;//实际拉出值
    DWORD   m_dwLowestIndex;//Q的最低点

    _SOFTCROSS_SEGOUT()
    {
        Reset();
    }
    void Reset()
    {
        m_dwId = m_dwSegId = m_dwLowestIndex = -1;
        m_f1.clear();
        m_f2.clear();
        m_F1.clear();
        m_F2.clear();
        m_L1.clear();
        m_L2.clear();
        m_M1.clear();
        m_M2.clear();
        m_T.clear();
        m_Y.clear();
        m_strA.clear();
        m_strQ.clear();
        m_strK.clear();
        m_strB.clear();
        m_strPull.clear();
    }
}SoftCrossSegOut;

//分段输出数据库管理
class CSegOutDBManager
{
public:
    CSegOutDBManager(void);
    ~CSegOutDBManager(void);

public:
    static BOOL InitDb();
    BOOL Add(const SoftCrossSegOut& output);
    BOOL Update(const SoftCrossSegOut& output, DWORD dwSegId, DWORD dwType = SEGOUT_ALL);
    BOOL Del(DWORD dwSegId);
    BOOL GetInfoBySegId(SoftCrossSegOut& output, DWORD dwSegId);
    
private:
    CppSQLite3DB m_db;
};