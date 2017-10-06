#ifndef _TABLE_PARSER_
#define _TABLE_PARSER_


#include <string>
#include <vector>


/*
��Ԫ��
*/
class Cell
{
public:
	double x, y, z, height;	// ����͸߶�
	std::string text;	// �ı�����

	Cell() : x(0), y(0), z(0), height(0), text("") {}
	bool isEmpty(void) { return 0 == x && 0 == y && 0 == z && 0 == text.compare(""); }	// ��Ԫ�������Ƿ�Ϊ��

public:
	// ��̬����,����һ����Ԫ��
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
����ͷ
*/
class Header
{
public:
	Cell ce;	// ����ͷ�������ĵ�Ԫ��
	double xOffset;	// ����ͷ��x����ƫ��ֵ

	Header() : xOffset(ce.height) {}
};

/*
��
*/
class Row
{
public:
	Row(size_t len);	// len��ʾ�����м���
	~Row();

	bool addCell(const Cell& ce, size_t index);	// ��������ӵ�Ԫ��
	const std::vector<Cell>& getCellList(void);	// ��ȡ�е����е�Ԫ��
	double getRowHeight(void);	// ��ȡ�и�
	bool isCellDigit(size_t index);	// �õ�Ԫ��������Ƿ�Ϊ���ָ�ʽ

private:
	std::vector<Cell> mCellList;	// ���еĵ�Ԫ���б�
	Cell mFirstAddCell;	// ��һ����ӵ����еĵ�Ԫ��(������ֵ��һ��Ϊ0)
};

/*
��ϸ��
*/
class Table
{
public:
	Table(const std::vector<Header>& headerList);
	~Table();

	bool parseCell(const Cell& ce);	// ������Ԫ��
	const std::vector<Header>& getHeaderList(void);	// ��ȡ����ͷ�б�
	const std::vector<Row*>& getRowList(void);	// ��ȡ���б�
	void eraseRow(size_t index);	// ɾ��ָ����������
	bool inXRange(const Cell& ce);	// ��Ԫ���Ƿ��ڱ��x���귶Χ��
	double getStartY(void);	// ��ȡ��Ŀ�ʼy����
	int getHeaderIndex(const std::string& title);	// ��ȡ���������

private:
	bool insertRowImpl(const Cell& ce, size_t index);	// �ڲ��ӿ�

public:
	std::vector<Header> mHeaderList;	// ����ϸ��ı���ͷ
	std::vector<Row*> mRowList;	// ����ϸ�����������,����Ϊ��λ
	double mStartX;	// ����ϸ��ʼ�����x����
	double mEndX;	// ����ϸ����������x����
	double mStartY;	// ����ϸ��ʼ�����y����
};

/*
�������
*/
class TableParser
{
public:
	TableParser(const std::vector<std::string>& titleList, const std::vector<size_t>& digitIndexList, bool headerAtTop = true);
	~TableParser();
	bool collectHeader(const Cell& ce);	// �ռ�����ͷ
	void sortHeaders(void);	// �Ա���ͷ���з���
	void createTables(void);	// ���ݱ���ͷ������
	bool collectCell(const Cell& ce);	// �ռ���Ԫ��,�����䵽��������
	void trimNotFitRow(void);	// ɾ�������ڱ����
	const std::vector<Table*>& getTableList(void);	// ��ȡ���б�	

private:
	void trimNotDigitFromTable(Table *tb);	// ɾ������ĳ�����ݲ��������ָ�ʽ����

private:
	std::vector<std::string> mTitleList;	// ��Ҫ��������ϸ��ı���ͷ����
	std::vector<size_t> mDigitIndexList;	// ��Ҫ�������Ƿ�Ϊ���ָ�ʽ�ı���ͷ�����е�����
	std::vector<std::vector<Cell>> mOriginHeaderList;	// ��ʼ�����ڴ�ı���ͷ�б�,δ����
	std::vector<std::vector<Cell>> mHeaderList;	// �����ı���ͷ�б�
	std::vector<Table*> mTableList;	// ��ϸ���б�
	bool mHeaderAtTop;	// �����Ƿ��ڱ�Ķ���
};


#endif

