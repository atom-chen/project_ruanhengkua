#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//站点
typedef struct _SOFTCROSS_SITE
{
    DWORD   m_dwSiteId;//站点ID
    CString  m_strName;//站点名称
}SoftCrossSite;

//站点数据库管理
class CSiteDBManager
{
public:
    CSiteDBManager(void);
    ~CSiteDBManager(void);

public:
    static BOOL InitDb();
    BOOL AddSite(const SoftCrossSite& site);
    BOOL UpdateSite(const SoftCrossSite& site, DWORD dwId);
    BOOL DelSite(DWORD dwId);
    BOOL GetAllSite(vector<SoftCrossSite>& vecSites);
    CString GetNameById(DWORD dwSiteId);

private:
    BOOL CheckSiteName(const CString& strName);

private:
    CppSQLite3DB m_db;
};

