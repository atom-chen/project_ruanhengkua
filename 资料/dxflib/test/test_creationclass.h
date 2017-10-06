/**
 * @file test_creationclass.h
 */

/*****************************************************************************
**  $Id: test_creationclass.h 219 2004-01-07 23:08:40Z andrew $
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

#ifndef TEST_CREATIONCLASS_H
#define TEST_CREATIONCLASS_H

#include "dl_creationadapter.h"
#include "TableParser.h"
#include <fstream>
#include <vector>


/**
 * This class takes care of the entities read from the file.
 * Usually such a class would probably store the entities.
 * this one just prints some information about them to stdout.
 *
 * @author Andrew Mustun
 */

class DxfPart
{
public:
	std::string name;
	size_t count;

	DxfPart(void) : name(""), count(0) {}

	bool increase(const std::string& na, size_t num)
	{
		if (0 == name.compare(na.c_str()))
		{
			count += num;
			return true;
		}
		return false;
	}
};


class Test_CreationClass : public DL_CreationAdapter {
public:
    Test_CreationClass(const std::vector<std::string>& titleList, const std::vector<size_t>& digitIndexList/*, const char* filename*/);
	~Test_CreationClass();

	virtual void addMText(const DL_MTextData& data);
	virtual void addText(const DL_TextData& data);

	// 创建明细表
	void createTable(void);
	// 计算零件数量
	void computeDxfPart(const std::string& key = "名称");
	// 保存明细表的内容到文本
	void saveFile(const std::string& filename);
	
private:
	// 往明细表插入合适的数据
	void writeTable(void);
	// 把数据写到文本
	void doString(const char* type, double x, double y, double z, double height, const char* text);

private:
	//std::ofstream mFile;
	std::vector<DL_TextData> mTextDataVector;
	std::vector<DL_MTextData> mMTextDataVector;
	std::vector<DxfPart> mDxfPartVector;
	TableParser *mTableParser;
};

#endif
