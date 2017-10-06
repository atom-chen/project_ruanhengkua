#pragma once


#ifndef _DXF_PARSER_H_
#define _DXF_PARSER_H_

#include <stdlib.h>
#include <stdio.h>
#include "dxflib/dl_dxf.h" 
#include "test_creationclass.h"
#include "../miniexcel/mexcel.h"

using namespace std;
using namespace miniexcel;


class DxfComputeRow
{
public:
	string key;
	size_t value;
	string unit;

	DxfComputeRow(void) : key(""), value(0), unit("") {}

	bool compute(const string& k, size_t v)
	{
		if (0 == key.compare(k.c_str()))
		{
			value += v;
			return true;
		}
		return false;
	}
};

void getInfoFromDxfTable(const vector<Table*>& tables, vector<DxfComputeRow>& rows, 
							const string& key, const string& value, const string& unit)
{
	for (size_t i = 0; i<tables.size(); ++i)
	{
		vector<Row*> rowList = tables[i]->getRowList();
		int keyIndex = tables[i]->getHeaderIndex(key.c_str());
		int valueIndex = tables[i]->getHeaderIndex(value.c_str());
		int unitIndex = tables[i]->getHeaderIndex(unit.c_str());
		for (size_t j = 0; j<rowList.size(); ++j)
		{
			vector<Cell> cellList = rowList[j]->getCellList();
			string keyText = -1 == keyIndex ? "" : cellList[keyIndex].text;
			size_t valueNumber = -1 == valueIndex ? 0 : atoi(cellList[valueIndex].text.c_str());
			string unitText = -1 == unitIndex ? "" : cellList[unitIndex].text;
			bool computeFlag = false;
			for (size_t k = 0; k<rows.size(); ++k)
			{
				if (rows[k].compute(keyText, valueNumber))
				{
					computeFlag = true;
					break;
				}
			}
			if (!computeFlag)
			{
				DxfComputeRow dcr;
				dcr.key = keyText;
				dcr.value = valueNumber;
				dcr.unit = unitText;
				rows.push_back(dcr);
			}
		}
	}
}

/*
函数：从dxf文件统计数值
参数：key - 关键字的列标题,根据此字段来获取统计值;unit - 单位的列标题;value - 要统计的列标题
*/
vector<DxfComputeRow> getInfoFromDxf(const vector<string>& files, const vector<string>& titleList,  
										const string& key = "名称", const string& value = "数量", const string& unit = "单位")
{
	vector<DxfComputeRow> rows;
	for (size_t i = 0; i<files.size(); ++i)
	{
		Test_CreationClass *creationClass = new Test_CreationClass(titleList, key, value);
		DL_Dxf *dxf = new DL_Dxf();
		if (dxf->in(files[i].c_str(), creationClass))
		{
			getInfoFromDxfTable(creationClass->createTable(), rows, key, value, unit);
		}
		delete creationClass;
		creationClass = NULL;
		delete dxf;
		dxf = NULL;
	}
	return rows;
}

void writeInfoToXls(const string& filename, const vector<DxfComputeRow>& rows, 
						const string& key = "名称", const string& value = "数量", const string& unit = "单位")
{
	FILE *f = fopen(filename.c_str(), "wb");

	CMiniExcel excel;
	excel(0, 0) = key.c_str();
	excel(0, 1) = value.c_str();
	excel(0, 2) = unit.c_str();
	for (size_t i = 0; i<rows.size(); ++i)
	{
		excel(i + 1, 0) = rows[i].key.c_str();
		excel(i + 1, 1) = rows[i].value;
		excel(i + 1, 2) = rows[i].unit.c_str();
	}
	excel.Write(f);

	fclose(f);
}


#endif // _DXF_PARSER_H_


