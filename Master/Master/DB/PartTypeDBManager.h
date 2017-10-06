#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//零件类型数据
typedef struct _PART_TYPE
{
    DWORD   m_dwId;//ID
    DWORD   m_dwFlag; //类型，是否含有重量，尺寸
    CString m_strName;//名称
}PartType;

//零件类型数据库管理
class CPartTypeDBManager
{
public:
    CPartTypeDBManager(void);
    ~CPartTypeDBManager(void);

public:
    static BOOL InitDb();
    BOOL Add(const PartType& seg);
    BOOL Update(const PartType& seg, DWORD dwId);
    BOOL Del(DWORD dwId);
    BOOL GetAllType(vector<PartType>& vecSegs);
    BOOL GetTypeById(PartType& seg, DWORD dwId);

    BOOL CheckName(const CString& strName);

private:
    CppSQLite3DB m_db;
};