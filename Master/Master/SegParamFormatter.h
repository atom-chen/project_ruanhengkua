#pragma once

//���ݸ�ʽ��
//�ڵ�ѡ��Q1...QN: �������ҽڵ�_(a5-�������ҽڵ�����λ�������ڵ�ˮƽ����)_�Ҳ�����ҽڵ�_(a6-�Ҳ�����ҽڵ�����λ�������ڵ�ˮƽ����)|...|...
#include <string>
using namespace std;

//�ڵ�ѡ������
typedef struct _NODESEL_DATA 
{
    DWORD   m_dwSelfId;//���
    DWORD   m_dwIsHang;//�Ƿ����ҽڵ�
    DWORD   m_dwCatenaryId;//������ID
    DWORD   m_dwTouchLineId;//�Ӵ���ID
    string  m_strPreCrossLen;//��ǰһ���ҵ���
    string  m_strNextCrossLen;//����һ���ҵ���
	string	m_strDistanceToPillar;//��֧������

    _NODESEL_DATA()
    {
        m_dwSelfId = m_dwCatenaryId = m_dwTouchLineId = -1;
        m_dwIsHang = FALSE;
    }
}NodeSelData;

//�ֶβ�����ʽ��
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
    int m_nNum;//�ڵ���
    NodeSelData* m_pNodeSelData;//�ֶ��µ����нڵ�
};

