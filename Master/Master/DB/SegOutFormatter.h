#pragma once

//数据格式化
#include <string>
using namespace std;

//分段输出格式化
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
    int m_nNum;//输出的个数
    string* m_pSegOutData;//输出的数据(格式化的一串字符串)
};

