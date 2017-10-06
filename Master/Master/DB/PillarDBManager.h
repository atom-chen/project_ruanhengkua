#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//֧������
typedef struct _PILLAR_PARAM
{
    DWORD   m_dwId;//���ID
    DWORD   m_dwType;//֧������
    string  m_strName;//�����
    string  m_strUp;//֧���ϵ�
    string  m_strBelow;//֧���µ�
    string  m_strLength;//֧���ܳ�
    string  m_strWidthOfUp;//֧���������

    _PILLAR_PARAM()
    {
        m_dwId = -1;
        m_dwType = -1;
    }
}PillarParam;

//֧�����ݿ����
class CPillarDBManager
{
public:
    CPillarDBManager(void);
    ~CPillarDBManager(void);

public:
    static BOOL InitDb();
    BOOL Add(const PillarParam& parts);
    BOOL Update(const PillarParam& parts, DWORD dwId);
    BOOL Del(DWORD dwId);
    BOOL GetAllPillars(vector<PillarParam>& vecParts);
    BOOL GetPillarInfoById(PillarParam& part, DWORD dwId);
    BOOL CheckName(const CString& strName);

private:
    CppSQLite3DB m_db;
};