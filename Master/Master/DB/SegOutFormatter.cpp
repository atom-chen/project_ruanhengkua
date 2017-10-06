#include "StdAfx.h"
#include "SegOutFormatter.h"
#include "..\GlobalFun.h"

CSegOutFormatter::CSegOutFormatter(int nNum)
    :m_pSegOutData(NULL), m_nNum(nNum)
{
    Init(m_nNum);
}

CSegOutFormatter::CSegOutFormatter()
    :m_pSegOutData(NULL), m_nNum(0)
{

}

CSegOutFormatter::~CSegOutFormatter()
{
    if (NULL != m_pSegOutData)
    {
        delete[] m_pSegOutData;
    }    
}

void CSegOutFormatter::Init(int nNum)
{
    m_nNum = nNum;
    if (NULL != m_pSegOutData)
    {
        delete[] m_pSegOutData;
        m_pSegOutData = NULL;
    }
    if (0 != nNum)
    {
        m_pSegOutData = new string[nNum];
    }
}

void CSegOutFormatter::ConvertToString(CString& strOut)
{
    if(NULL == m_pSegOutData)
    {
        return;
    }
    for (int i = 0; i < m_nNum; i++)
    {
        CString strTemp;
        if (0 == i)
        {
            strTemp.Format(_T("%s"), m_pSegOutData[i].c_str());
        }
        else
        {
            strTemp.Format(_T("%s|%s"), strOut, m_pSegOutData[i].c_str());
        }
        strOut = strTemp;
    }
}

void CSegOutFormatter::ConvertFromString(const CString& strOut)
{
    if(NULL == m_pSegOutData)
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
        
        m_pSegOutData[nCount] = strTemp;           

        nCount++;   
        nStart = nPos + 1;
        nPos = strOut.Find(_T('|'), nStart);
    }
}

void CSegOutFormatter::SetNodeSel(const string& sel, int nIndex)
{
    if (NULL != m_pSegOutData)
    {
        m_pSegOutData[nIndex] = sel;
    }
}

void CSegOutFormatter::SetNodeSel(double sel, int nIndex)
{
    if (NULL != m_pSegOutData)
    {
        m_pSegOutData[nIndex] = CGlobalFun::DoubleToString(sel);
    }
}

void CSegOutFormatter::GetNodeSel(string& sel, int nIndex)
{
    if (NULL != m_pSegOutData)
    {
        sel = m_pSegOutData[nIndex];
    }
}