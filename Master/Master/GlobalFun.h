#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
using namespace std;

class CGlobalFun
{
public:
    CGlobalFun(void);
    ~CGlobalFun(void);

public:
    static char* Convert(const char* strIn, int sourceCodepage, int targetCodepage);
    static string DoubleToString(double dbData);
    static double StringToDouble(string& str); 
    static void GetStringVectorByToken(const TCHAR * strSrc, vector<CString> &vecOutput, TCHAR token);
    static void GetStringVectorByToken(const TCHAR * strSrc, set<CString> &vecOutput, TCHAR token);
    static void FormatStringByToken(CString& strOut, const vector<CString>& vecData, TCHAR token);
};

