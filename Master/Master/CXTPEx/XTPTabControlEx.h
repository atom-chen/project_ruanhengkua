#pragma once

class CWorkArea;

class CXTPTabControlEx : public CXTPTabControl
{
public:
	CXTPTabControlEx(CWorkArea* workArea = NULL);
	~CXTPTabControlEx(void);

protected:
	void OnItemClick(CXTPTabManagerItem* pItem);

private:
	CWorkArea *m_workArea;
};

