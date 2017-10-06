#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//支柱数据
typedef struct _PILLAR_PARAM
{
    DWORD   m_dwId;//零件ID
    DWORD   m_dwType;//支柱类型
    string  m_strName;//零件名
    string  m_strUp;//支柱上底
    string  m_strBelow;//支柱下底
    string  m_strLength;//支柱总长
    string  m_strWidthOfUp;//支柱顶部宽度

    _PILLAR_PARAM()
    {
        m_dwId = -1;
        m_dwType = -1;
    }
}PillarParam;

//支柱数据库管理
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