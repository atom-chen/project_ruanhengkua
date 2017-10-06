#include "StdAfx.h"
#include "SegParamFormatter.h"

CSegParamFormatter::CSegParamFormatter(int nNum)
    :m_pNodeSelData(NULL), m_nNum(nNum)
{
    Init(m_nNum);
}

CSegParamFormatter::CSegParamFormatter()
    :m_pNodeSelData(NULL), m_nNum(0)
{

}

CSegParamFormatter::~CSegParamFormatter()
{
    if (NULL != m_pNodeSelData)
    {
        delete[] m_pNodeSelData;
    }    
}

void CSegParamFormatter::Init(int nNum)
{
    m_nNum = nNum;
    if (NULL != m_pNodeSelData)
    {
        delete[] m_pNodeSelData;
        m_pNodeSelData = NULL;
    }
    if (0 != nNum)
    {
        m_pNodeSelData = new NodeSelData[nNum];
    }
}

void CSegParamFormatter::ConvertToString(CString& strOut)
{
    if(NULL == m_pNodeSelData)
    {
        return;
    }
    for (int i = 0; i < m_nNum; i++)
    {
        CString strTemp;
        if (0 == i)
        {
            strTemp.Format(_T("%d_%d_%s_%s_%d_%d_%s"),
                m_pNodeSelData[i].m_dwSelfId, m_pNodeSelData[i].m_dwIsHang,
                m_pNodeSelData[i].m_strPreCrossLen.c_str(), m_pNodeSelData[i].m_strNextCrossLen.c_str(),
                m_pNodeSelData[i].m_dwCatenaryId, m_pNodeSelData[i].m_dwTouchLineId, m_pNodeSelData[i].m_strDistanceToPillar.c_str());
        }
        else
        {
            strTemp.Format(_T("%s|%d_%d_%s_%s_%d_%d_%s"), strOut,
                m_pNodeSelData[i].m_dwSelfId, m_pNodeSelData[i].m_dwIsHang,
                m_pNodeSelData[i].m_strPreCrossLen.c_str(), m_pNodeSelData[i].m_strNextCrossLen.c_str(),
                m_pNodeSelData[i].m_dwCatenaryId, m_pNodeSelData[i].m_dwTouchLineId, m_pNodeSelData[i].m_strDistanceToPillar.c_str());
        }
        strOut = strTemp;
    }
}

void CSegParamFormatter::ConvertFromString(const CString& strOut)
{
    if(NULL == m_pNodeSelData)
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
        
        int nSubCount = 0;
        int nSubStart = 0;
        int nSubPos = strTemp.Find(_T('_'));
        while(TRUE)
        {
            if (nSubCount >= 7)
            {
                break;
            }
            CString strField;
            if (-1 == nSubPos)
            {
                strField = strTemp.Right(strTemp.GetLength() - nSubStart);
            }
            else
            {
                strField = strTemp.Mid(nSubStart, nSubPos - nSubStart);
            }
            switch(nSubCount)
            {
            case 0:
                m_pNodeSelData[nCount].m_dwSelfId = atoi(strField);
                break;
            case 1:
                m_pNodeSelData[nCount].m_dwIsHang = atoi(strField);
                break;
            case 2:
                m_pNodeSelData[nCount].m_strPreCrossLen = strField;
                break;
            case 3:
                m_pNodeSelData[nCount].m_strNextCrossLen = strField;
                break;
            case 4:
                m_pNodeSelData[nCount].m_dwCatenaryId = atoi(strField);
                break;
            case 5:
                m_pNodeSelData[nCount].m_dwTouchLineId = atoi(strField);
                break;
			case 6:
				m_pNodeSelData[nCount].m_strDistanceToPillar = strField;
				break;
            default:
                break;
            }
            nSubCount++;
            nSubStart = nSubPos + 1;
            nSubPos = strTemp.Find(_T('_'), nSubStart);
        }            

        nCount++;   
        nStart = nPos + 1;
        nPos = strOut.Find(_T('|'), nStart);
    }
}

void CSegParamFormatter::SetNodeSel(const NodeSelData& sel, int nIndex)
{
    if (NULL != m_pNodeSelData)
    {
        m_pNodeSelData[nIndex].m_dwSelfId = sel.m_dwSelfId;
        m_pNodeSelData[nIndex].m_dwIsHang = sel.m_dwIsHang;
        m_pNodeSelData[nIndex].m_strPreCrossLen = sel.m_strPreCrossLen;
        m_pNodeSelData[nIndex].m_strNextCrossLen = sel.m_strNextCrossLen;
        m_pNodeSelData[nIndex].m_dwCatenaryId = sel.m_dwCatenaryId;
        m_pNodeSelData[nIndex].m_dwTouchLineId = sel.m_dwTouchLineId;
		m_pNodeSelData[nIndex].m_strDistanceToPillar = sel.m_strDistanceToPillar;
    }
}

void CSegParamFormatter::GetNodeSel(NodeSelData& sel, int nIndex)
{
    if (NULL != m_pNodeSelData)
    {
        sel.m_dwSelfId = m_pNodeSelData[nIndex].m_dwSelfId;
        sel.m_dwIsHang = m_pNodeSelData[nIndex].m_dwIsHang;
        sel.m_strPreCrossLen = m_pNodeSelData[nIndex].m_strNextCrossLen;
        sel.m_strNextCrossLen = m_pNodeSelData[nIndex].m_strNextCrossLen;
        sel.m_dwCatenaryId = m_pNodeSelData[nIndex].m_dwCatenaryId;
        sel.m_dwTouchLineId = m_pNodeSelData[nIndex].m_dwTouchLineId;
		sel.m_strDistanceToPillar = m_pNodeSelData[nIndex].m_strDistanceToPillar;
    }
}