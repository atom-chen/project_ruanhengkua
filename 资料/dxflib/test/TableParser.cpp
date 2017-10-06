#include "TableParser.h"


//----------------------------------------------------------------------------
// �ж��ַ��Ƿ�Ϊ���ָ�ʽ
bool isDigit(char c)
{
	return '0' == c || '1' == c || '2' == c || '3' == c || '4' == c || '5' == c || '6' == c || '7' == c || '8' == c || '9' == c;
}
// �ж��ַ����Ƿ�Ϊ���ָ�ʽ
bool isDigit(const std::string& str)
{
	if (str.empty())
		return false;

	bool mask = false;
	int decimal = 0;
	for (size_t i = 0; i<str.length(); ++i)
	{
		// �����һ���ַ�Ϊ+��-,���ж�Ϊ������,�����Ƿ��ű�־,���������ж�
		if (0 == i && ('+' == str[i] || '-' == str[i]))
		{
			mask = true;
			continue;
		}
		if ('.' == str[i])
		{
			++decimal;
			if (decimal > 1)	//���С�����������һ��,���ʾ��������
				return false;
		}
		if (!isDigit(str[i]))
			return false;
	}
	if (mask && 1 == str.length())
		return false;

	return true;
}
// ����y�����ƫ��ֵ�ж��Ƿ�Ϊͬ��
bool sameRow(double y1, double y2, double yOffset)
{
	return abs(y1 - y2) <= yOffset;
}
// ����x�����ƫ��ֵ�ж��Ƿ�Ϊͬ��
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
	// ����ֵ�������е��������
	if (index >= mCellList.size())
		return false;

	bool addFlag = false;
	if (mFirstAddCell.isEmpty())	// ��һ����ӵ�Ԫ��
	{
		addFlag = true;
		mFirstAddCell = ce;
	}
	else
	{
		// �жϵ�Ԫ���Ƿ�͸���ͬ��,����׼������
		addFlag = sameRow(ce.y, mFirstAddCell.y, mFirstAddCell.height);
	}
	if (addFlag)
	{
		Cell tempCe = mCellList[index];
		if (tempCe.isEmpty())
		{
			tempCe = ce;	// ���浥Ԫ��������
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
	// �ж��������ĵ�Ԫ�������Ƿ�Ϊ���ָ�ʽ
	return isDigit(mCellList[index].text);
}

//----------------------------------------------------------------------------

Table::Table(const std::vector<Header>& headerList)
	:mHeaderList(headerList)
{
	Header startHeader = headerList[0];	// ����ϸ��ĵ�һ������ͷ
	Header endHeader = headerList[headerList.size()-1];	// ����ϸ������һ������ͷ
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
		if (0 == hea.ce.text.compare(ce.text.c_str()))	// ��Ԫ�����ݺ͸���ϸ��ı���ͷ����һ��(���ܽ�����������ı���ͷ)
			continue;
		
		// �жϵ�Ԫ���Ƿ�ͱ���ͬ��,�����жϲ����Ƿ�ɹ�
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
	// ɾ������������
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
	// �����Ժ���������ж��Ƿ��пɲ���ĵط�
	for (size_t i = 0; i<mRowList.size(); ++i)
	{
		Row *oldRow = mRowList[i];
		if (oldRow->addCell(ce, index))
		{
			insertFlag = true;
			break;
		}
	}
	// �޿ɲ���ĵط�
	if (false == insertFlag)
	{
		// �����µ�һ��
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

TableParser::TableParser(const std::vector<std::string>& titleList, const std::vector<size_t>& digitIndexList, bool headerAtTop /*= true*/)
	:mTitleList(titleList)
	,mDigitIndexList(digitIndexList)
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
		// �����Ƿ�Ϊ����ͷ,��������ʼ�ı���ͷ�б�
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
		headerList.push_back(startHeader);	// �����һ������ͷ,����:��"���"��ʼ
		// �Գ�ʼ����ͷ���й���
		for (size_t j = 1; j<mOriginHeaderList.size(); ++j)
		{
			Cell tempHeader;
			double tempDistance = 100000000;
			// ���ݱ���ͷ������������
			for (size_t k = 0; k<mOriginHeaderList[j].size(); ++k)
			{
				Cell header = mOriginHeaderList[j][k];
				if (!sameRow(header.y, startHeader.y, startHeader.height))
					continue;

				size_t count = headerList.size();
				// ����ñ���ͷ����һ������ͷ�ľ���
				double distance = header.x - headerList[count-1].x;
				if (distance > 0 && distance <= tempDistance)	// ����Խ��,���ʾ�ñ���ͷ�����ڵ�һ����ͷ���ڵ���ϸ��
				{
					tempHeader = header;
					tempDistance = distance;
				}
			}
			// �г�һ���µı���ͷ
			if (!tempHeader.isEmpty())
			{
				headerList.push_back(tempHeader);
			}
		}
		// ��������ı���ͷ�б�
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
				// ���ñ����x����ƫ��ֵ(��ǰ������x����-ǰһ������x����-ǰһ�����ƫ��ֵ)
				hea.xOffset = ce.x - preHeader.ce.x - preHeader.xOffset;
			}
			headerList.push_back(hea);
		}
		// ���ݱ���ͷ������ϸ��,�����浽�б�
		mTableList.push_back(new Table(headerList));
	}
}

bool TableParser::collectCell(const Cell& ce)
{
	Table *tb = NULL;
	double distance = 0;
	// ����������ϸ��
	for (size_t i = 0; i<mTableList.size(); ++i)
	{
		Table *tempTable = mTableList[i];
		// �жϵ�Ԫ���Ƿ�����ϸ��x����ķ�Χ��
		if (tempTable->inXRange(ce))
		{
			// ��ȡ��Ԫ����ϸ��ʼ�����y����ľ���
			double tempDistance = ce.y - tempTable->getStartY();
			if (mHeaderAtTop)	// �����ڶ���
			{
				if (tempDistance > 0)	// ���ڱ����ڶ���,���Ե�Ԫ���y����-��ϸ��ʼ�����y����>0
				{
					if (0 == distance)
					{
						tb = tempTable;
						distance = tempDistance;
					}
					else if (tempDistance < distance)	// ����ԽС,��ʾ�õ�Ԫ�����ڸñ�
					{
						tb = tempTable;
						distance = tempDistance;
					}
				}
			} 
			else	// �����ڵײ�
			{
				if (tempDistance < 0)	// ���ڱ����ڵײ���,���Ե�Ԫ���y����-��ϸ��ʼ�����y����<0
				{
					if (0 == distance)
					{
						tb = tempTable;
						distance = tempDistance;
					}
					else if (tempDistance > distance)	// ����Խ��,��ʾ�õ�Ԫ�����ڸñ�
					{
						tb = tempTable;
						distance = tempDistance;
					}
				}
			}
		}
	}
	// ���ݷ��������ĵ�Ԫ��������ϸ�������뵥Ԫ��
	return NULL == tb ? false : tb->parseCell(ce);
}

void TableParser::trimNotFitRow(void)
{
	// ������ϸ���б�
	for (size_t i = 0; i<mTableList.size(); ++i)
	{
		trimNotDigitFromTable(mTableList[i]);
	}
}

const std::vector<Table*>& TableParser::getTableList(void)
{
	return mTableList;
}

void TableParser::trimNotDigitFromTable(Table *tb)
{
	if (NULL == tb)
		return;

	size_t length = tb->getRowList().size();
	// ������ϸ�����
	for (size_t i = 0; i<length;)
	{
		Row * r = tb->getRowList()[i];
		bool findFlag = false;
		// ������ϸ��ĳ�е����е�Ԫ��
		for (size_t j = 0; j<r->getCellList().size(); ++j)
		{
			Cell ce = r->getCellList()[j];
			// �жϵ�Ԫ�������е�����ֵ�����ݱ���Ϊ���ָ�ʽ��������ֵ�Ƿ����
			for (size_t k = 0; k<mDigitIndexList.size(); ++k)
			{
				// ����ֵ���,�ҵ�Ԫ�����ݲ�Ϊ���ָ�ʽ
				if (j == mDigitIndexList[k] && !r->isCellDigit(j))
				{
					findFlag = true;
					break;
				}
			}
			if (findFlag)
				break;
		}
		// �ҵ���������ϸ�����,ɾ��,��ϸ������-1
		if (findFlag)
		{
			tb->eraseRow(i);
			--length;
		}
		else	// �ж���ϸ�����һ��
		{
			++i;
		}
	}
}
//----------------------------------------------------------------------------

