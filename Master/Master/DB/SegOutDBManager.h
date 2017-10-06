#pragma once

//�ֶμ���ĳ�ʼ����
//�ɵ��������ѡ�񣬶�Ӧ�ڵ�

#include "DBInit.h"
#include <vector>
using namespace std;

#define SEGOUT_ALL 0
#define SEGOUT_PULL 1
#define SEGOUT_RES 2

//�ֶ��������
typedef struct _SOFTCROSS_SEGOUT
{
    DWORD   m_dwId;//ID
    DWORD   m_dwSegId;//�����ķֶ�ID
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
    string  m_strPull;//ʵ������ֵ
    DWORD   m_dwLowestIndex;//Q����͵�

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

//�ֶ�������ݿ����
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