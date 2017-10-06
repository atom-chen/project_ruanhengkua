#include "StdAfx.h"
#include "DBInit.h"
#include "../GlobalFun.h"

char* CDBInit::g_pDbPath = NULL;

CDBInit::CDBInit(void)
{
}


CDBInit::~CDBInit(void)
{
}

void CDBInit::InitDbPath()
{
    if (NULL == g_pDbPath)
    {
        TCHAR szPath[MAX_PATH] = {0};
        GetModuleFileName(NULL, szPath, MAX_PATH);
        CStringA strPath = szPath;
        strPath = strPath.Left(strPath.ReverseFind('\\'));
        strPath += "\\softcross.db";
        g_pDbPath = CGlobalFun::Convert(strPath, CP_ACP, CP_UTF8);//sqlite3的路径须转成UTF8格式
    }
}

void CDBInit::Free()
{
    if (NULL != g_pDbPath)
    {
        delete[] g_pDbPath;
        g_pDbPath = NULL;
    }
}