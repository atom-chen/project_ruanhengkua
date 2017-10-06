#include "TableParser.h"
#include "../CommonFunction/CommonFunction.h"


//----------------------------------------------------------------------------

// 根据y坐标和偏移值判断是否为同行
bool sameRow(double y1, double y2, double yOffset)
{
	return abs(y1 - y2) <= yOffset;
}
// 根据x坐标和偏移值判断是否为同列
bool sameCol(double x1, double x2, double xOffset)
{
	return abs(x1 - x2) <= xOffset;
}

//----------------------------------------------------------------------------

Row::Row(size_t len)
	:mCellList(len)
{
}

Row::~Row()
{
}

bool Row::addCell(const Cell& ce, size_t index)
{
	// 索引值超出该行的最大列数
	if (index >= mCellList.size())
		return false;

	bool addFlag = false;
	if (mFirstAddCell.isEmpty())	// 第一次添加单元格
	{
		addFlag = true;
		mFirstAddCell = ce;
	}
	else
	{
		// 判断单元格是否和改行同行,是则准备插入
		addFlag = sameRow(ce.y, mFirstAddCell.y, mFirstAddCell.height);
	}
	if (addFlag)
	{
		Cell tempCe = mCellList[index];
		if (tempCe.isEmpty())
		{
			tempCe = ce;	// 保存单元格到索引处
		}
		else
		{
			if (tempCe.x < ce.x)
				tempCe.text += ce.text;
			else
				tempCe.text = ce.text + tempCe.text;
		}
		mCellList[index] = tempCe;
	}
	return addFlag;
}

Cell Row::getCell(size_t index)
{
	Cell ce;
	if (index >= 0 && index < mCellList.size())
	{
		ce = mCellList[index];
	}
	return ce;
}

const std::vector<Cell>& Row::getCellList(void)
{
	return mCellList;
}

double Row::getRowHeight(void)
{
	return mFirstAddCell.height;
}

bool Row::isCellDigit(size_t index)
{
	if (index >= mCellList.size())
		return false;
	// 判断索引处的单元格内容是否为数字格式
	return isDigit(mCellList[index].text);
}

//----------------------------------------------------------------------------

Table::Table(const std::vector<Header>& headerList)
	:mHeaderList(headerList)
{
	Header startHeader = headerList[0];	// 该明细表的第一个标题头
	Header endHeader = headerList[headerList.size()-1];	// 该明细表的最后一个标题头
	mStartX = startHeader.ce.x - startHeader.xOffset;
	mStartY = startHeader.ce.y - startHeader.ce.height;
	mEndX = endHeader.ce.x + endHeader.xOffset;
}

Table::~Table()
{
	for (size_t j = 0; j<mRowList.size(); ++j)
	{
		delete mRowList[j];
		mRowList[j] = NULL;
	}
	mRowList.clear();
}

bool Table::parseCell(const Cell& ce)
{
	for (size_t i = 0; i<mHeaderList.size(); ++i)
	{
		Header hea = mHeaderList[i];
		if (0 == hea.ce.text.compare(ce.text.c_str()))	// 单元格内容和该明细表的标题头内容一样(可能解析到其他表的标题头)
			continue;
		
		// 判断单元格是否和标题同列,是则判断插入是否成功
		if (sameCol(ce.x, hea.ce.x, hea.xOffset) && insertRowImpl(ce, i))
			return true;
	}
	return false;
}

const std::vector<Header>& Table::getHeaderList(void)
{
	return mHeaderList;
}

const std::vector<Row*>& Table::getRowList(void)
{
	return mRowList;
}

void Table::eraseRow(size_t index)
{
	if (index >= mRowList.size())
		return;
	// 删除索引处的行
	std::vector<Row*>::iterator iter = mRowList.begin() + index;
	delete *iter;
	*iter = NULL;
	mRowList.erase(iter);
}

bool Table::inXRange(const Cell& ce)
{
	return ce.x >= mStartX && ce.x <= mEndX;
}

double Table::getStartY(void)
{
	return mStartY;
}

int Table::getHeaderIndex(const std::string& title)
{
	for (size_t i = 0; i<mHeaderList.size(); ++i)
	{
		if (0 == mHeaderList[i].ce.text.compare(title.c_str()))
			return i;
	}
	return -1;
}

bool Table::insertRowImpl(const Cell& ce, size_t index)
{
	bool insertFlag = false;
	// 先在以后的行里面判断是否有可插入的地方
	for (size_t i = 0; i<mRowList.size(); ++i)
	{
		Row *oldRow = mRowList[i];
		if (oldRow->addCell(ce, index))
		{
			insertFlag = true;
			break;
		}
	}
	// 无可插入的地方
	if (false == insertFlag)
	{
		// 创建新的一行
		Row *newRow = new Row(mHeaderList.size());
		if (newRow->addCell(ce, index))
		{
			mRowList.push_back(newRow);
			insertFlag = true;
		}
		else
		{
			delete newRow;
			newRow = NULL;
		}
	}
	return insertFlag;
}

//----------------------------------------------------------------------------

TableParser::TableParser(const std::vector<std::string>& titleList, const std::string& key, const std::string& value, bool headerAtTop /*= true*/)
	:mTitleList(titleList)
	,mKey(key)
	,mValue(value)
	,mOriginHeaderList(titleList.size())
	,mHeaderAtTop(headerAtTop)
{
}

TableParser::~TableParser()
{
	for (size_t i = 0; i<mTableList.size(); ++i)
	{
		delete mTableList[i];
		mTableList[i] = NULL;
	}
	mTableList.clear();
}

bool TableParser::collectHeader(const Cell& ce)
{
	for (size_t i = 0; i<mTitleList.size(); ++i)
	{
		// 分析是否为标题头,是则插入初始的标题头列表
		if (0 == mTitleList[i].compare(ce.text.c_str()))
		{
			mOriginHeaderList[i].push_back(ce);
			return true;
		}
	}
	return false;
}

void TableParser::sortHeaders(void)
{
	if (mOriginHeaderList.empty())
		return;

	for (size_t i = 0; i<mOriginHeaderList[0].size(); ++i)
	{
		Cell startHeader = mOriginHeaderList[0][i];
		std::vector<Cell> headerList;
		headerList.push_back(startHeader);	// 保存第一个标题头,例如:以"序号"开始
		// 对初始标题头进行归类
		for (size_t j = 1; j<mOriginHeaderList.size(); ++j)
		{
			Cell tempHeader;
			double tempDistance = 100000000;
			// 根据标题头的坐标来归类
			for (size_t k = 0; k<mOriginHeaderList[j].size(); ++k)
			{
				Cell header = mOriginHeaderList[j][k];
				if (!sameRow(header.y, startHeader.y, startHeader.height))
					continue;

				size_t count = headerList.size();
				// 计算该标题头到上一个标题头的距离
				double distance = header.x - headerList[count-1].x;
				if (distance > 0 && distance <= tempDistance)	// 距离越短,则表示该标题头是属于第一标题头所在的明细表
				{
					tempHeader = header;
					tempDistance = distance;
				}
			}
			// 行成一样新的标题头
			if (!tempHeader.isEmpty())
			{
				headerList.push_back(tempHeader);
			}
		}
		// 保存归类后的标题头列表
		if (headerList.size() == mTitleList.size())
		{
			mHeaderList.push_back(headerList);
		}
	}
}

void TableParser::createTables(void)
{
	for (size_t i = 0; i<mHeaderList.size(); ++i)
	{
		std::vector<Header> headerList;
		for (size_t j = 0; j<mHeaderList[i].size(); ++j)
		{
			Cell ce = mHeaderList[i][j];
			Header hea;
			hea.ce = ce;
			if (0 == j)
			{
				hea.xOffset = ce.height;
			}
			else
			{
				Header preHeader = headerList[j-1];
				// 设置标题的x坐标偏移值(当前插入点的x坐标-前一插入点的x坐标-前一标题的偏移值)
				hea.xOffset = ce.x - preHeader.ce.x - preHeader.xOffset;
			}
			headerList.push_back(hea);
		}
		// 根据标题头创建明细表,并保存到列表
		mTableList.push_back(new Table(headerList));
	}
}

bool TableParser::collectCell(const Cell& ce)
{
	Table *tb = NULL;
	double distance = 0;
	// 遍历所有明细表
	for (size_t i = 0; i<mTableList.size(); ++i)
	{
		Table *tempTable = mTableList[i];
		// 判断单元格是否在明细表x坐标的范围内
		if (tempTable->inXRange(ce))
		{
			// 获取单元格到明细表开始插入的y坐标的距离
			double tempDistance = ce.y - tempTable->getStartY();
			if (mHeaderAtTop)	// 标题在顶部
			{
				if (tempDistance > 0)	// 由于标题在顶部,所以单元格的y坐标-明细表开始插入的y坐标>0
				{
					if (0 == distance)
					{
						tb = tempTable;
						distance = tempDistance;
					}
					else if (tempDistance < distance)	// 距离越小,表示该单元格属于该表
					{
						tb = tempTable;
						distance = tempDistance;
					}
				}
			} 
			else	// 标题在底部
			{
				if (tempDistance < 0)	// 由于标题在底部部,所以单元格的y坐标-明细表开始插入的y坐标<0
				{
					if (0 == distance)
					{
						tb = tempTable;
						distance = tempDistance;
					}
					else if (tempDistance > distance)	// 距离越大,表示该单元格属于该表
					{
						tb = tempTable;
						distance = tempDistance;
					}
				}
			}
		}
	}
	// 根据分析出来的单元格所属明细表来插入单元格
	return NULL == tb ? false : tb->parseCell(ce);
}

void TableParser::trimNotFitRow(void)
{
	// 遍历明细表列表
	for (size_t i = 0; i<mTableList.size(); ++i)
	{
		trimNotFitFromTable(mTableList[i]);
	}
}

const std::vector<Table*>& TableParser::getTableList(void)
{
	return mTableList;
}

void TableParser::trimNotFitFromTable(Table *tb)
{
	if (NULL == tb)
		return;

	size_t keyIndex = tb->getHeaderIndex(mKey);
	size_t valueIndex = tb->getHeaderIndex(mValue);
	size_t length = tb->getRowList().size();
	// 遍历明细表的行
	for (size_t i = 0; i<length;)
	{
		Row * r = tb->getRowList()[i];
		// 关键字所在的单元必须不为空,并且统计值所在单元的值必须为数字类型
		if (r->getCell(keyIndex).isEmpty() || !r->isCellDigit(valueIndex))
		{
			tb->eraseRow(i);
			--length;
		}
		else	// 判断明细表的下一行
		{
			++i;
		}
	}
}
//----------------------------------------------------------------------------

