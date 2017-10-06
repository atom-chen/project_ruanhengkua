#include "StdAfx.h"
#include "XTPTabControlEx.h"
#include "../WorkArea.h"


CXTPTabControlEx::CXTPTabControlEx(CWorkArea* workArea /*= NULL*/)
	:m_workArea(workArea)
{
}


CXTPTabControlEx::~CXTPTabControlEx(void)
{
}

void CXTPTabControlEx::OnItemClick(CXTPTabManagerItem* pItem)
{
	SetSelectedItem(pItem);
	if (m_workArea)
	{
		m_workArea->setSelTabIndex(pItem->GetIndex());
	}
}