#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//�ڵ�����
typedef struct _SOFTCROSS_NODE
{
    DWORD   m_dwId;     //�ڵ�ID
    string  m_strName;  //�ڵ�����
    string  m_strWeight; //�ڵ�����
    string  m_strPartId; //����б�

    _SOFTCROSS_NODE()
    {
        m_dwId = -1;
    }
}SoftCrossNode;

//�ڵ����ݿ����
class CNodeDBManager
{
public:
    CNodeDBManager(void);
    ~CNodeDBManager(void);

public:
    static BOOL InitDb();
    BOOL Add(const SoftCrossNode& node);
    BOOL Update(const SoftCrossNode& node, DWORD dwId);
    BOOL Del(DWORD dwId);
    BOOL GetAllNodes(vector<SoftCrossNode>& vecNodes);
    BOOL GetNodeById(SoftCrossNode& vecNodes, DWORD dwId);

private:
    BOOL CheckName(const string& strName);

private:
    CppSQLite3DB m_db;
};