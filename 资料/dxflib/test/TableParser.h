#ifndef _TABLE_PARSER_
#define _TABLE_PARSER_


#include <string>
#include <vector>


/*
单元格
*/
class Cell
{
public:
	double x, y, z, height;	// 坐标和高度
	std::string text;	// 文本内容

	Cell() : x(0), y(0), z(0), height(0), text("") {}
	bool isEmpty(void) { return 0 == x && 0 == y && 0 == z && 0 == text.compare(""); }	// 单元格内容是否为空

public:
	// 静态函数,创建一个单元格
	static Cell createCell(double x, double y, double z, double height, const std::string& text)
	{
		Cell ce;
		ce.x = x;
		ce.y = y;
		ce.height = height;
		ce.text = text;
		return ce;
	}
};

/*
标题头
*/
class Header
{
public:
	Cell ce;	// 标题头所包含的单元格
	double xOffset;	// 标题头的x坐标偏移值

	Header() : xOffset(ce.height) {}
};

/*
行
*/
class Row
{
public:
	Row(size_t len);	// len表示此行有几列
	~Row();

	bool addCell(const Cell& ce, size_t index);	// 向行里添加单元格
	const std::vector<Cell>& getCellList(void);	// 获取行的所有单元格
	double getRowHeight(void);	// 获取行高
	bool isCellDigit(size_t index);	// 该单元格的内容是否为数字格式

private:
	std::vector<Cell> mCellList;	// 改行的单元格列表
	Cell mFirstAddCell;	// 第一个添加到改行的单元格(其索引值不一定为0)
};

/*
明细表
*/
class Table
{
public:
	Table(const std::vector<Header>& headerList);
	~Table();

	bool parseCell(const Cell& ce);	// 解析单元格
	const std::vector<Header>& getHeaderList(void);	// 获取标题头列表
	const std::vector<Row*>& getRowList(void);	// 获取行列表
	void eraseRow(size_t index);	// 删除指定索引的行
	bool inXRange(const Cell& ce);	// 单元格是否在表的x坐标范围内
	double getStartY(void);	// 获取表的开始y坐标
	int getHeaderIndex(const std::string& title);	// 获取标题的索引

private:
	bool insertRowImpl(const Cell& ce, size_t index);	// 内部接口

public:
	std::vector<Header> mHeaderList;	// 此明细表的标题头
	std::vector<Row*> mRowList;	// 此明细表的数据内容,以行为单位
	double mStartX;	// 此明细表开始插入的x坐标
	double mEndX;	// 此明细表结束插入的x坐标
	double mStartY;	// 此明细表开始插入的y坐标
};

/*
表解析器
*/
class TableParser
{
public:
	TableParser(const std::vector<std::string>& titleList, const std::vector<size_t>& digitIndexList, bool headerAtTop = true);
	~TableParser();
	bool collectHeader(const Cell& ce);	// 收集标题头
	void sortHeaders(void);	// 对标题头进行分类
	void createTables(void);	// 根据标题头创建表
	bool collectCell(const Cell& ce);	// 收集单元格,并分配到各个表里
	void trimNotFitRow(void);	// 删除不属于表的行
	const std::vector<Table*>& getTableList(void);	// 获取表列表	

private:
	void trimNotDigitFromTable(Table *tb);	// 删除表里某列内容不属于数字格式的行

private:
	std::vector<std::string> mTitleList;	// 所要解析的明细表的标题头内容
	std::vector<size_t> mDigitIndexList;	// 所要分析的是否为数字格式的标题头所在列的索引
	std::vector<std::vector<Cell>> mOriginHeaderList;	// 初始读入内存的标题头列表,未归类
	std::vector<std::vector<Cell>> mHeaderList;	// 归类后的标题头列表
	std::vector<Table*> mTableList;	// 明细表列表
	bool mHeaderAtTop;	// 标题是否在表的顶部
};


#endif

