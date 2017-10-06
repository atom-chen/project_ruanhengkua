#pragma once
#include "afxwin.h"

#include "DB/SegParamDBManager.h"
// CSegPartSelDlg �Ի���

class CSegPartSelDlg : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(CSegPartSelDlg)

public:
	CSegPartSelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSegPartSelDlg();

// �Ի�������
	enum { IDD = IDD_SEGPARTSELDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_comboCrossBear;
    CComboBox m_comboFixRopeUp;
    CComboBox m_comboFixRopeBelow;
    virtual BOOL OnInitDialog();

private:
    DWORD GetPartIdFromCombo(const CComboBox& comboBox);
public:
    afx_msg void OnBnClickedCancel();
    DWORD m_dwUnitRoad;
    SoftCrossSegParam* m_pSegParam;
    virtual BOOL OnSetActive();
    virtual LRESULT OnWizardNext();
    DWORD m_dwNodeNum;
    CComboBox m_comboPillarLeft;
    CComboBox m_comboPillarRight;
    BOOL m_bDouble;//�Ƿ���˫���
    afx_msg void OnBnClickedPartmanager();
    void ResetCombo();
};
