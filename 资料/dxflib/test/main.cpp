/*
 * @file main.cpp
 */

/*****************************************************************************
**  $Id: main.cpp 2362 2005-04-04 14:56:33Z andrew $
**
**  This is part of the dxflib library
**  Copyright (C) 2000-2001 Andrew Mustun
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

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "dl_dxf.h" 
#include "test_creationclass.h"

void testReading(char* file);
void testWriting();


/*
 * @brief Main function for DXFLib test program.
 *
 * @param argc Number of delimited items on command line,
 *		including program name.
 * @param argv Pointer to array of command line items
 *
 * @retval 0 if missing input file argument or
 *		file couldn't be opened
 * @retval 1 if file opened
 */
int main(int argc, char** argv) {

	testReading("test.dxf");
	std::string temp;
	std::cin>>temp;

    return 0;
}

void testReading(char* file) {
    // Load DXF file into memory:
    std::cout << "Reading file " << file << " ...\n";
	// 标题内容
	std::vector<std::string> titleList;
	titleList.push_back("序号");	// 0
	titleList.push_back("代号");
	titleList.push_back("名称");
	titleList.push_back("材料");
	titleList.push_back("单位");
	titleList.push_back("数量");	// 5
	titleList.push_back("单重");
	titleList.push_back("共重");
	titleList.push_back("附注");
	// 设定哪个列的内容必须为数字格式,注意:插入的是标题内容的索引值
	std::vector<size_t> digitIndexList;
	digitIndexList.push_back(0);
	digitIndexList.push_back(5);
	// 
    Test_CreationClass* creationClass = new Test_CreationClass(titleList, digitIndexList/*"data.txt"*/);
    DL_Dxf* dxf = new DL_Dxf();
    if (!dxf->in(file, creationClass)) { // if file open failed
        std::cerr << file << " could not be opened.\n";
        return;
    }
	std::cout << "Creating tables ...\n";
	creationClass->createTable();
	std::cout << "Computing dxf parts ...\n";
	creationClass->computeDxfPart();
	std::cout << "Saving tables to file ...\n";
	creationClass->saveFile("table.txt");

    delete dxf;
    delete creationClass;
	std::cout<<"End of reading file " << file << " ...";
}
