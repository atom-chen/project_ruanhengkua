#include "StdAfx.h"
#include "SegInFormatter.h"

CSegInFormatter::CSegInFormatter(int nNum)
    :m_pData(NULL), m_nNum(nNum)
{
    Init(m_nNum);
}

CSegInFormatter::CSegInFormatter()
    :m_pData(NULL), m_nNum(0)
{

}

CSegInFormatter::~CSegInFormatter()
{
    if (NULL != m_pData)
    {
        delete[] m_pData;
    }    
}

void CSegInFormatter::Init(int nNum)
{
    m_nNum = nNum;
    if (NULL != m_pData)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
    if (0 != nNum)
    {
        m_pData = new string[nNum];
    }
}

void CSegInFormatter::ConvertToString(CString& strOut)
{
    if(NULL == m_pData)
    {
        return;
    }
    for (int i = 0; i < m_nNum; i++)
    {
        CString strTemp;
        if (0 == i)
        {
            strTemp.Format(_T("%s"), m_pData[i].c_str());
        }
        else
        {
            strTemp.Format(_T("%s|%s"), strOut, m_pData[i].c_str());
        }
        strOut = strTemp;
    }
}

void CSegInFormatter::ConvertFromString(CString& strOut)
{
    if(NULL == m_pData)
    {
        return;
    }
    int nCount = 0;
    int nStart = 0;
    int nPos = strOut.Find(_T('|'));
    while (TRUE)
    {
        if (nCount >= m_nNum)
        {
            break;
        }
        CString strTemp;
        if (-1 == nPos)
        {
            strTemp = strOut.Right(strOut.GetLength() - nStart);
        }
        else
        {
            strTemp = strOut.Mid(nStart, nPos - nStart);
        }

        m_pData[nCount] = strTemp;           

        nCount++;   
        nStart = nPos + 1;
        nPos = strOut.Find(_T('|'), nStart);
    }
}

void CSegInFormatter::SetValue(const CString& sel, int nIndex)
{
    if (NULL != m_pData)
    {
        m_pData[nIndex] = sel;
    }
}

void CSegInFormatter::GetValue(CString& sel, int nIndex)
{
    if (NULL != m_pData)
    {
        sel = m_pData[nIndex].c_str();
    }
}

void CSegInFormatter::GetValue(CString& strOut, CString& sel, int nIndex)
{
    ConvertFromString(strOut);
    GetValue(sel, nIndex);
}

void CSegInFormatter::GetValue(CString& strOut, CString* pValue)
{
    if (NULL == pValue)
    {
        return;
    }
    ConvertFromString(strOut);
    for (int i = 0; i < m_nNum; i++)
    {
        GetValue(pValue[i], i);        
    }
}

void CSegInFormatter::SetValue(CString& strOut, const CString& sel, int nIndex)
{
    ConvertFromString(strOut);
    SetValue(sel, nIndex);
    ConvertToString(strOut);
}