#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//零件数据
typedef struct _PART_PARAM
{
    DWORD   m_dwId;//零件ID
    DWORD   m_dwTypeId;//所属类型ID
    DWORD   m_dwFlag;//标志位，属性是否有效
    string  m_strName;//零件名
    string  m_strWeight;//重量
    string  m_strLength;//尺寸

    _PART_PARAM()
    {
        m_dwId = m_dwTypeId = -1;
    }
}PartParam;

//零件数据库管理
class CPartDBManager
{
public:
    CPartDBManager(void);
    ~CPartDBManager(void);

public:
    static BOOL InitDb();
    BOOL Add(const PartParam& parts);
    BOOL Update(const PartParam& parts, DWORD dwId);
    BOOL Del(DWORD dwId);
    BOOL GetPartsByTypeId(vector<PartParam>& vecParts, DWORD dwTypeId);
    BOOL GetPartInfoById(PartParam& part, DWORD dwPartId);
	BOOL GetParts(vector<PartParam>& vecParts);
    CString GetNameById(DWORD dwId);

private:
    //BOOL CheckSegmentName(const CString& strName);

private:
    CppSQLite3DB m_db;
};