#pragma once

#include "DBInit.h"
#include <vector>
using namespace std;

//节点数据
typedef struct _SOFTCROSS_NODE
{
    DWORD   m_dwId;     //节点ID
    string  m_strName;  //节点名称
    string  m_strWeight; //节点重量
    string  m_strPartId; //零件列表

    _SOFTCROSS_NODE()
    {
        m_dwId = -1;
    }
}SoftCrossNode;

//节点数据库管理
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