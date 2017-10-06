#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <map>
#include <list>
using namespace std;

#define PART_WEIGHT 0x1 //��������
#define PART_LENGTH 0x2 //�����ߴ�
#define PART_PILLAR 0x4 //����֧��
// CPartTypeDlg �Ի���

class CPartTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartTypeDlg)

public:
	CPartTypeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPartTypeDlg();

// �Ի�������
	enum { IDD = IDD_PARTTYPEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    DWORD   m_dwId;
    CString m_strName;
    afx_msg void OnBnClickedOk();
    virtual BOOL OnInitDialog();
    BOOL m_bWeight;
    BOOL m_bLength;
};
