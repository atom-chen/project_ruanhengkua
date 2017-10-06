#pragma once

#include "../sqlite3/CppSQLite3.h"

class CDBInit
{
public:
    CDBInit(void);
    ~CDBInit(void);

public:
    static void InitDbPath();
    static void Free();

public:
    static char* g_pDbPath;
};

