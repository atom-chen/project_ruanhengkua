#pragma once

#include <vector>
#include "DBInit.h"

using namespace std;


//��������������
class PartConnectParam
{
public:
	PartConnectParam(void)
	{
		m_dwId = -1;
	}

public:
	DWORD m_segId;				// �ֶ�id
	DWORD m_dwId;				// �������id,0:���,1:�ϲ��̶���,2:�²��̶���
	string m_name;				// �����������
	string m_partIds;			// ���id��
	string m_partNames;			// ������ִ�
};


//�������������ݿ����
class CPartConnectDBManager
{
public:
	CPartConnectDBManager(void);
	~CPartConnectDBManager(void);

public:
	// ��ʼ���ݿ�
	static BOOL InitDb(void);
	// �������
	BOOL Add(const PartConnectParam& partConnect);
	// ��������
	BOOL Update(const PartConnectParam& partConnect);
	// ɾ������
	BOOL Del(DWORD segId, DWORD dwId);
	// ��ȡ��������
	PartConnectParam GetPartConnectById(DWORD segId, DWORD dwId);
	// ����ȫ������
	vector<PartConnectParam> GetAllPartConnect(DWORD segId);
	// ��ȡ�������ݵ����id(���ظ�)
	vector<DWORD> GetPartIdVecById(DWORD segId, DWORD dwId);
	// ��ȡȫ�����ݵ����id(���ظ�)
	vector<DWORD> GetPartIdVec(DWORD segId);
	// �Ƿ����ĳ������
	BOOL FindPartConnect(DWORD segId, DWORD dwId);

private:
	// �����id�ַ������л�ȡ���ظ������id
	vector<DWORD> SeparatePartId(const vector<CString>& partIdVec);
	// �������в����Ƿ�������id
	BOOL FindPartId(const vector<DWORD>& partIdVec, DWORD partId);

private:
	CppSQLite3DB m_db;
};


