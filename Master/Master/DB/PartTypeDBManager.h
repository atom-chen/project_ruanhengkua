#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//�����������
typedef struct _PART_TYPE
{
    DWORD   m_dwId;//ID
    DWORD   m_dwFlag; //���ͣ��Ƿ����������ߴ�
    CString m_strName;//����
}PartType;

//����������ݿ����
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