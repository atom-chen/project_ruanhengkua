#include "StdAfx.h"
#include "PartTypeManager.h"
#include "../GlobalFun.h"

PartType g_arrPartType[] = 
{
    {1010, 4, _T("支柱")},
    {1011, 1, _T("承力索")},
    {1012, 1, _T("接触线")},
    {1013, 1, _T("横向承力索")},
    {1013, 1, _T("下部固定绳")},
    {1013, 1, _T("上部固定绳")},
    {1014, 3, _T("补偿器")},
    {1015, 1, _T("定位环")},
    {1016, 3, _T("杵头杆")},
    {1017, 1, _T("承力索线夹")},
    {1018, 1, _T("定位器")},
    {1019, 3, _T("耳环杆")},
    {1020, 3, _T("连接器")}
};

vector<PartType> CPartTypeManager::m_vecPartType;

CPartTypeManager::CPartTypeManager(void)
{
}


CPartTypeManager::~CPartTypeManager(void)
{
}

void CPartTypeManager::InitPartType()
{
    m_vecPartType.clear();

    for (int i = 0; i < sizeof(g_arrPartType) / sizeof(PartType); i++)
    {
        m_vecPartType.push_back(g_arrPartType[i]);
    }

    CPartTypeDBManager db;
    db.GetAllType(m_vecPartType);
}
