#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <map>
#include <list>
using namespace std;

#define PART_WEIGHT 0x1 //包含重量
#define PART_LENGTH 0x2 //包含尺寸
#define PART_PILLAR 0x4 //包含支柱
// CPartTypeDlg 对话框

class CPartTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartTypeDlg)

public:
	CPartTypeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPartTypeDlg();

// 对话框数据
	enum { IDD = IDD_PARTTYPEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    DWORD   m_dwId;
    CString m_strName;
    afx_msg void OnBnClickedOk();
    virtual BOOL OnInitDialog();
    BOOL m_bWeight;
    BOOL m_bLength;
};
