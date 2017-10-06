#include "StdAfx.h"
#include "GlobalFun.h"

CGlobalFun::CGlobalFun(void)
{
}


CGlobalFun::~CGlobalFun(void)
{
}

//¶à×Ö½Ú±àÂë×ª»»
char* CGlobalFun::Convert(const char* strIn, int sourceCodepage, int targetCodepage)
{
    int len = lstrlen(strIn);
    int unicodeLen = MultiByteToWideChar(sourceCodepage, 0, strIn, -1, NULL, 0);
    wchar_t* pUnicode;
    pUnicode = new wchar_t[unicodeLen + 1];
    if(NULL == pUnicode)
    {
        return NULL;
    }
    memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(sourceCodepage, 0, strIn, -1, (LPWSTR)pUnicode, unicodeLen);
    char* pTargetData = NULL;
    int targetLen = WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, 0, NULL, NULL);
    pTargetData = new char[targetLen + 1];
    if (NULL == pTargetData)
    {
        delete[] pUnicode;
        return NULL;
    }
    memset(pTargetData, 0, targetLen + 1);
    WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, targetLen, NULL, NULL);
    delete[] pUnicode;

    return pTargetData;
}

string CGlobalFun::DoubleToString(double dbData)
{
    stringstream ss;
    ss << dbData;
    return ss.str();
}

double CGlobalFun::StringToDouble(string& str) 
{ 
    double d; 
    std::stringstream ss;

    ss.precision(0xf);

    ss <<str; 
    ss >> std::fixed >> d; 
    return d; 
}

void CGlobalFun::GetStringVectorByToken(const TCHAR * strSrc, vector<CString>& vecOutput,TCHAR token)
{
    CString strTemp(strSrc);
    int pos = 0;
    while(true)
    {
        CString temp = strTemp.Tokenize(&token,pos);
        if (pos != -1 && temp != L"")
        {
            vecOutput.push_back(temp);
        }
        else
        {
            break;
        }
    }
}

void CGlobalFun::GetStringVectorByToken(const TCHAR * strSrc, set<CString>& vecOutput,TCHAR token)
{
    CString strTemp(strSrc);
    int pos = 0;
    while(true)
    {
        CString temp = strTemp.Tokenize(&token,pos);
        if (pos != -1 && temp != L"")
        {
            vecOutput.insert(temp);
        }
        else
        {
            break;
        }
    }
}

void CGlobalFun::FormatStringByToken(CString& strOut, const vector<CString>& vecData, TCHAR token)
{
    strOut = _T("");

    int nSize = (int)vecData.size();
    for (int i = 0; i < nSize; i++)
    {
        CString strTemp;
        if (0 == i)
        {
            strTemp.Format(_T("%s"), vecData[i]);
        }
        else
        {
            strTemp.Format(_T("%s%c%s"), strOut, token, vecData[i]);
        }
        strOut = strTemp;
    }
}