#pragma once

#include "../tinyxml/tinyxml.h"
#include "PartTypeDBManager.h"
#include <vector>
using namespace std;

#define TYPE_XML_NAME "PartsType.xml"

#define PARTTYPE_WEIGHT 0
#define PARTTYPE_LENGTH 1

//零件类型管理
class CPartTypeManager
{
public:
    CPartTypeManager(void);
    ~CPartTypeManager(void);

public:
    static void InitPartType();
    static vector<PartType> m_vecPartType;
};

