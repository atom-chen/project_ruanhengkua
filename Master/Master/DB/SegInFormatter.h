#pragma once

#include <string>
using namespace std;

class CSegInFormatter
{
public:
    CSegInFormatter(int);
    CSegInFormatter(void);
    ~CSegInFormatter(void);

public:
    void Init(int nNum);
    void ConvertToString(CString& strOut);
    void ConvertFromString(CString& strOut);
    void GetValue(CString& sel, int nIndex);
    void SetValue(const CString& sel, int nIndex);
    void GetValue(CString& strOut, CString& sel, int nIndex);
    void GetValue(CString& strOut, CString* pValue);
    void SetValue(CString& strOut, const CString& sel, int nIndex);

private:
    int m_nNum;//个数
    string* m_pData;//输入数据
};

