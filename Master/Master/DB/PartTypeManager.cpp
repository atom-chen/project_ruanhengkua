#include "StdAfx.h"
#include "PartTypeManager.h"
#include "../GlobalFun.h"

PartType g_arrPartType[] = 
{
    {1010, 4, _T("֧��")},
    {1011, 1, _T("������")},
    {1012, 1, _T("�Ӵ���")},
    {1013, 1, _T("���������")},
    {1013, 1, _T("�²��̶���")},
    {1013, 1, _T("�ϲ��̶���")},
    {1014, 3, _T("������")},
    {1015, 1, _T("��λ��")},
    {1016, 3, _T("��ͷ��")},
    {1017, 1, _T("�������߼�")},
    {1018, 1, _T("��λ��")},
    {1019, 3, _T("������")},
    {1020, 3, _T("������")}
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
