/*
 * @file test_creationclass.cpp
 */

/*****************************************************************************
**  $Id: test_creationclass.cpp 163 2003-07-01 15:51:48Z andrew $
**
**  This is part of the dxflib library
**  Copyright (C) 2001 Andrew Mustun
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU Library General Public License as
**  published by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Library General Public License for more details.
**
**  You should have received a copy of the GNU Library General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

#include "test_creationclass.h"

#include <iostream>
#include <stdio.h>


/**
 * Default constructor.
 */
Test_CreationClass::Test_CreationClass(const std::vector<std::string>& titleList, const std::vector<size_t>& digitIndexList/*, const char* filename*/) /*: mFile(filename)*/ {
	// 创建明细表解析器,注意:false表示标题在表的底部,反之为true
	mTableParser = new TableParser(titleList, digitIndexList, false);
}

Test_CreationClass::~Test_CreationClass() {
	delete mTableParser;
	mTableParser = NULL;
}

void Test_CreationClass::addMText(const DL_MTextData& data) {
	// 标题分析
	mTableParser->collectHeader(Cell::createCell(data.ipx, data.ipy, data.ipz, data.height, data.text));
	// 保存MText到内存
	mMTextDataVector.push_back(data);
	// 把MText写到文件
	doString("MText", data.ipx, data.ipy, data.ipz, data.height, data.text.c_str());
	// 在控制台打印MText信息
	//printf("MText   (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f) (%d, %d, %d, %f) (%s, %s, %f)\n", 
	//	data.ipx, data.ipy, data.ipz, data.width, data.height, 
	//	data.attachmentPoint, data.drawingDirection, data.lineSpacingStyle, data.lineSpacingFactor, 
	//	data.text.c_str(), data.style.c_str(), data.angle);
}

void Test_CreationClass::addText(const DL_TextData& data) {
	// 标题分析
	mTableParser->collectHeader(Cell::createCell(data.ipx, data.ipy, data.ipz, data.height, data.text));
	// 保存Text到内存
	mTextDataVector.push_back(data);
	// 把Text写到文件
	doString("Text", data.ipx, data.ipy, data.ipz, data.height, data.text.c_str());
	// 在控制台打印Text信息
	//printf("Text   (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f, %6.3f) (%f, %f, %d, %d, %d) (%s, %s, %f)\n", 
	//	data.ipx, data.ipy, data.ipz, data.apx, data.apy, data.apz, 
	//	data.height, data.xScaleFactor, data.textGenerationFlags, data.hJustification, data.vJustification, 
	//	data.text.c_str(), data.style.c_str(), data.angle);
}

void Test_CreationClass::createTable(void)
{
	// 对标题进行归类
	mTableParser->sortHeaders();
	// 根据标题创建明细表
	mTableParser->createTables();
	// 填充表格数据
	writeTable();
}

void Test_CreationClass::writeTable(void)
{
	// 遍历内存中的Text数据
	for (size_t i = 0; i<mTextDataVector.size(); ++i)
	{
		DL_TextData textData = mTextDataVector[i];
		mTableParser->collectCell(Cell::createCell(textData.ipx, textData.ipy, textData.ipz, textData.height, textData.text));
	}
	// 遍历内存中的MText数据
	for (size_t j = 0; j<mMTextDataVector.size(); ++j)
	{
		DL_MTextData mtextData = mMTextDataVector[j];
		mTableParser->collectCell(Cell::createCell(mtextData.ipx, mtextData.ipy, mtextData.ipz, mtextData.height, mtextData.text));
	}
	// 去除与明细表内容无关的数据
	mTableParser->trimNotFitRow();
}

void Test_CreationClass::computeDxfPart(const std::string& key /*= "名称"*/)
{
	const std::vector<Table*> tableList = mTableParser->getTableList();
	for (size_t i = 0; i<tableList.size(); ++i)
	{
		std::vector<Row*> rowList = tableList[i]->getRowList();
		size_t keyIndex = tableList[i]->getHeaderIndex(key.c_str());
		size_t countIndex = tableList[i]->getHeaderIndex("数量");
		for (size_t j = 0; j<rowList.size(); ++j)
		{
			std::vector<Cell> cellList = rowList[j]->getCellList();
			std::string keyText = cellList[keyIndex].text;
			size_t count = atoi(cellList[countIndex].text.c_str());
			bool increaseFlag = false;
			for (size_t k = 0; k<mDxfPartVector.size(); ++k)
			{
				if (mDxfPartVector[k].increase(keyText, count))
				{
					increaseFlag = true;
					break;
				}
			}
			if (!increaseFlag)
			{
				DxfPart dp;
				dp.name = keyText;
				dp.count = count;
				mDxfPartVector.push_back(dp);
			}
		}
	}
}

void Test_CreationClass::saveFile(const std::string& filename)
{
	// 写到文本
	std::ofstream file(filename.c_str());
	// 写各明细表
	const std::vector<Table*> tableList = mTableParser->getTableList();
	for (size_t i = 0; i<tableList.size(); ++i)
	{
		file << "========================================= table_" << i << " =========================================\n";
		// 写标题头
		std::vector<Header> headList = tableList[i]->getHeaderList();
		for (size_t j = 0; j<headList.size(); ++j)
		{
			//file << headList[j].ce.text << "(x:" << headList[j].ce.x << ", y:" << headList[j].ce.y << ")           ";
			file << headList[j].ce.text << "           ";
		}
		file << "\n";
		// 写数据
		std::vector<Row*> rowList = tableList[i]->getRowList();
		for (size_t k = 0; k<rowList.size(); ++k)
		{
			std::vector<Cell> cellList = rowList[k]->getCellList();
			for (size_t l = 0; l<cellList.size(); ++l)
			{
				//file << cellList[l].text << "(x:" << cellList[l].x << ", y:" << cellList[l].y << ")          ";
				file << cellList[l].text << "          ";
			}
			file << "\n";
		}
	}
	file << "========================================= compute result =========================================\n";
	// 写统计结果
	for (size_t m = 0; m<mDxfPartVector.size(); ++m)
	{
		DxfPart dp = mDxfPartVector[m];
		file << dp.name << "          " << dp.count << "\n";
	}
}

void Test_CreationClass::doString(const char* type, double x, double y, double z, double height, const char* text)
{
	if (NULL == text || "" == text)
		return;

	//mFile << type << " -> (x=" << x << ", y=" << y << ", z=" << z << ") height=" << height << ", text=" << text << "\n";
}


// EOF
