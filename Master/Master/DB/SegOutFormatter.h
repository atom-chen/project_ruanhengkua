#pragma once

//���ݸ�ʽ��
#include <string>
using namespace std;

//�ֶ������ʽ��
class CSegOutFormatter
{
public:
    CSegOutFormatter(int);
    CSegOutFormatter(void);
    ~CSegOutFormatter(void);

public:
    void Init(int nNum);
    void ConvertToString(CString& strOut);
    void ConvertFromString(const CString& strOut);
    void GetNodeSel(string& sel, int nIndex);
    void SetNodeSel(const string& sel, int nIndex);
    void SetNodeSel(double sel, int nIndex);

private:
    int m_nNum;//����ĸ���
    string* m_pSegOutData;//���������(��ʽ����һ���ַ���)
};

