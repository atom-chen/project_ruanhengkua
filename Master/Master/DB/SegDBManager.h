#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//�ֶ�����
typedef struct _SOFTCROSS_SEG
{
    DWORD   m_dwSegId;//�ֶ�ID
    DWORD   m_dwSiteId;//������վ��ID
    CString m_strName;//�ֶ�����
}SoftCrossSeg;

//�ֶ����ݿ����
class CSegDBManager
{
public:
    CSegDBManager(void);
    ~CSegDBManager(void);

public:
    static BOOL InitDb();
    BOOL Add(const SoftCrossSeg& seg);
    BOOL Update(const SoftCrossSeg& seg, DWORD dwId);
    BOOL Del(DWORD dwId);
    BOOL GetAllSegmentBySid(vector<SoftCrossSeg>& vecSegs, DWORD dwSiteId);
    DWORD GetInsertId();
    BOOL GetSegBySegId(SoftCrossSeg& seg, DWORD dwSegId);

private:
    BOOL CheckSegmentName(DWORD dwSiteId, const CString& strName);

private:
    CppSQLite3DB m_db;
};