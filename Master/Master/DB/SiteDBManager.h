#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//վ��
typedef struct _SOFTCROSS_SITE
{
    DWORD   m_dwSiteId;//վ��ID
    CString  m_strName;//վ������
}SoftCrossSite;

//վ�����ݿ����
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

