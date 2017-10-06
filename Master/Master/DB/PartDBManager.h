#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//�������
typedef struct _PART_PARAM
{
    DWORD   m_dwId;//���ID
    DWORD   m_dwTypeId;//��������ID
    DWORD   m_dwFlag;//��־λ�������Ƿ���Ч
    string  m_strName;//�����
    string  m_strWeight;//����
    string  m_strLength;//�ߴ�

    _PART_PARAM()
    {
        m_dwId = m_dwTypeId = -1;
    }
}PartParam;

//������ݿ����
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