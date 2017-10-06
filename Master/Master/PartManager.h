#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

typedef struct _SOFTCROSS_SEG
{
    DWORD   m_dwSegId;//分段ID
    DWORD   m_dwSiteId;//所属的站点ID
    CString m_strName;//分段名称
}SoftCrossSeg;

class CSegManager
{
public:
    CSegManager(void);
    ~CSegManager(void);

public:
    static BOOL InitDb();
    BOOL Add(const SoftCrossSeg& seg);
    BOOL Update(const SoftCrossSeg& seg, DWORD dwId);
    BOOL Del(DWORD dwId);
    BOOL GetAllSegmentBySid(vector<SoftCrossSeg>& vecSegs, DWORD dwSiteId);

private:
    BOOL CheckSegmentName(const CString& strName);

private:
    CppSQLite3DB m_db;
};