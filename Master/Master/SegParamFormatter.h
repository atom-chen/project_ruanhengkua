#pragma once

//数据格式化
//节点选择Q1...QN: 左侧非悬挂节点_(a5-左侧非悬挂节点重心位置至本节点水平距离)_右侧非悬挂节点_(a6-右侧非悬挂节点重心位置至本节点水平距离)|...|...
#include <string>
using namespace std;

//节点选择数据
typedef struct _NODESEL_DATA 
{
    DWORD   m_dwSelfId;//序号
    DWORD   m_dwIsHang;//是否悬挂节点
    DWORD   m_dwCatenaryId;//承力索ID
    DWORD   m_dwTouchLineId;//接触线ID
    string  m_strPreCrossLen;//至前一悬挂点跨距
    string  m_strNextCrossLen;//至后一悬挂点跨距
	string	m_strDistanceToPillar;//至支柱距离

    _NODESEL_DATA()
    {
        m_dwSelfId = m_dwCatenaryId = m_dwTouchLineId = -1;
        m_dwIsHang = FALSE;
    }
}NodeSelData;

//分段参数格式化
class CSegParamFormatter
{
public:
    CSegParamFormatter(int);
    CSegParamFormatter(void);
    ~CSegParamFormatter(void);

public:
    void Init(int nNum);
    void ConvertToString(CString& strOut);
    void ConvertFromString(const CString& strOut);
    void GetNodeSel(NodeSelData& sel, int nIndex);
    void SetNodeSel(const NodeSelData& sel, int nIndex);

private:
    int m_nNum;//节点数
    NodeSelData* m_pNodeSelData;//分段下的所有节点
};

