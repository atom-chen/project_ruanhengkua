// SegNodeSelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Master.h"
#include "SegNodeSelDlg.h"
#include "afxdialogex.h"
#include "SegPartSelDlg.h "
#include "DB/NodeDBManager.h"
#include "DataValidator.h"
#include "DB/PartDBManager.h"
#include "NodeManagerDlg.h"
#include "SoftCrossCalculatePage.h"


// CSegNodeSelDlg 对话框

IMPLEMENT_DYNAMIC(CSegNodeSelDlg, CXTPPropertyPage)

CSegNodeSelDlg::CSegNodeSelDlg(CWnd* pParent /*=NULL*/)
	: CXTPPropertyPage(CSegNodeSelDlg::IDD)
{
}

CSegNodeSelDlg::~CSegNodeSelDlg()
{
}

void CSegNodeSelDlg::DoDataExchange(CDataExchange* pDX)
{
    CXTPPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LISTQ, m_listQ);
    DDX_Control(pDX, IDC_COMBONODE, m_comboNode);
    DDX_Control(pDX, IDC_COMBOHANG, m_comboHang);
    DDX_Control(pDX, IDC_COMBOCATENARY, m_comboCatenary);
    DDX_Control(pDX, IDC_COMBOTOUCHLINE, m_comboTouchLine);
    DDX_Control(pDX, IDC_EDTDISTANCE, m_edtDistance);
}


BEGIN_MESSAGE_MAP(CSegNodeSelDlg, CXTPPropertyPage)
    ON_BN_CLICKED(IDB_NODESET, &CSegNodeSelDlg::OnBnClickedNodeset)
END_MESSAGE_MAP()


// CSegNodeSelDlg 消息处理程序


BOOL CSegNodeSelDlg::OnInitDialog()
{
    CXTPPropertyPage::OnInitDialog();

    // TODO:  在此添加额外的初始化

    //列表设置
    m_listQ.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_listQ.InsertColumn(0, _T("序号"));
    m_listQ.InsertColumn(1, _T("自身节点"));
    m_listQ.InsertColumn(2, _T("是否悬挂节点"));
    m_listQ.InsertColumn(3, _T("至前一悬挂点跨距"));
    m_listQ.InsertColumn(4, _T("至后一悬挂点跨距"));
    m_listQ.InsertColumn(5, _T("承力索"));
    m_listQ.InsertColumn(6, _T("接触线"));
	m_listQ.InsertColumn(7, _T("到支柱距离"));

    m_ImageList.Create(1, 19, ILC_COLOR, 0, 1);
    m_listQ.SetImageList(&m_ImageList, LVSIL_SMALL);

    //列表头
    HWND hWndHeader = m_listQ.GetDlgItem(0)->GetSafeHwnd();
    m_header.SubclassWindow(hWndHeader);
    //m_header.EnableAutoSize(TRUE);
    m_header.ResizeColumnsToFit();

    //节点下拉列表
    InitNodeListAndMap();
    InitCatenaryListAndMap();
    InitTouchLineListAndMap();

    //列使用下拉列表编辑
    m_listQ.SetColumnEditor(1, &CSegNodeSelDlg::InitEditor, &CSegNodeSelDlg::EndEditor, &m_comboNode);//节点选择
    m_listQ.SetColumnEditor(2, &CSegNodeSelDlg::InitEditor, &CSegNodeSelDlg::EndEditor, &m_comboHang);//是否为非悬挂点
    m_listQ.SetColumnEditor(3, NULL, NULL, &m_edtDistance);
    m_listQ.SetColumnEditor(4, NULL, NULL, &m_edtDistance);
    m_listQ.SetColumnEditor(5, &CSegNodeSelDlg::InitEditor, &CSegNodeSelDlg::EndEditor, &m_comboCatenary);
    m_listQ.SetColumnEditor(6, &CSegNodeSelDlg::InitEditor, &CSegNodeSelDlg::EndEditor, &m_comboTouchLine);
	m_listQ.SetColumnEditor(7, NULL, NULL, &m_edtDistance);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CSegNodeSelDlg::InitNodeListAndMap()
{
    m_comboNode.ResetContent();

    vector<SoftCrossNode> vecNodes;
    CNodeDBManager db;
    db.GetAllNodes(vecNodes);
    vector<SoftCrossNode>::iterator itNode;
    int nIndex = 0;
    for (itNode = vecNodes.begin(); itNode != vecNodes.end(); itNode++)
    {
        m_mapNodeNameId.insert(map<CString, DWORD>::value_type(itNode->m_strName.c_str(), itNode->m_dwId));

        m_comboNode.AddString(itNode->m_strName.c_str());
        m_comboNode.SetItemData(nIndex, itNode->m_dwId);

        nIndex++;
    }
}

void CSegNodeSelDlg::InitCatenaryListAndMap()
{
    m_comboCatenary.ResetContent();

    CPartDBManager partDb;
    vector<PartParam>::iterator itPart;
    vector<PartParam> vecParts;
    int nIndex = 0;
    m_mapCatenaryNameId.insert(map<CString, DWORD>::value_type("", -1));
    m_comboCatenary.AddString("");
    m_comboCatenary.SetItemData(nIndex, -1);

    partDb.GetPartsByTypeId(vecParts, 1011);//承力索
    for (itPart = vecParts.begin(); itPart != vecParts.end(); itPart++)
    {
        m_mapCatenaryNameId.insert(map<CString, DWORD>::value_type(itPart->m_strName.c_str(), itPart->m_dwId));
        m_comboCatenary.AddString(itPart->m_strName.c_str());
        m_comboCatenary.SetItemData(nIndex, itPart->m_dwId);

        nIndex++;
    }
}

void CSegNodeSelDlg::InitTouchLineListAndMap()
{
    m_comboTouchLine.ResetContent();

    CPartDBManager partDb;
    vector<PartParam>::iterator itPart;
    vector<PartParam> vecParts;
    int nIndex = 0;
    m_mapTouchLineNameId.insert(map<CString, DWORD>::value_type("", -1));
    m_comboTouchLine.AddString("");
    m_comboTouchLine.SetItemData(nIndex, -1);

    partDb.GetPartsByTypeId(vecParts, 1012);//接触线
    for (itPart = vecParts.begin(); itPart != vecParts.end(); itPart++)
    {
        m_mapTouchLineNameId.insert(map<CString, DWORD>::value_type(itPart->m_strName.c_str(), itPart->m_dwId));
        m_comboTouchLine.AddString(itPart->m_strName.c_str());
        m_comboTouchLine.SetItemData(nIndex, itPart->m_dwId);

        nIndex++;
    }
}

void CSegNodeSelDlg::UpdateQ(int nIndex)
{
    NodeSelData sel;
    m_formatter.GetNodeSel(sel, nIndex);
    TCHAR* arrYesNo[2] = {_T("否"), _T("是")};
    if (sel.m_dwSelfId != -1)
    {
        SoftCrossNode node;
        CNodeDBManager dbNode;
        dbNode.GetNodeById(node, sel.m_dwSelfId);
        m_listQ.SetItemText(nIndex, 1, node.m_strName.c_str());
    }
    else
    {
        m_listQ.SetItemText(nIndex, 1, _T(""));
    }
    m_listQ.SetItemText(nIndex, 2, arrYesNo[sel.m_dwIsHang]);
    m_listQ.SetItemText(nIndex, 3, sel.m_strPreCrossLen.c_str());
    m_listQ.SetItemText(nIndex, 4, sel.m_strNextCrossLen.c_str());
    if (sel.m_dwCatenaryId != -1)
    {
        PartParam part;
        CPartDBManager dbPart;
        dbPart.GetPartInfoById(part, sel.m_dwCatenaryId);
        m_listQ.SetItemText(nIndex, 5, part.m_strName.c_str());
    }
    else
    {
        m_listQ.SetItemText(nIndex, 5, _T(""));
    }
    if (sel.m_dwTouchLineId != -1)
    {
        PartParam part;
        CPartDBManager dbPart;
        dbPart.GetPartInfoById(part, sel.m_dwTouchLineId);
        m_listQ.SetItemText(nIndex, 6, part.m_strName.c_str());
    }
    else
    {
        m_listQ.SetItemText(nIndex, 6, _T(""));
    }
	m_listQ.SetItemText(nIndex, 7, sel.m_strDistanceToPillar.c_str());
}

BOOL CSegNodeSelDlg::OnSetActive()
{
    // TODO: 在此添加专用代码和/或调用基类

    CPropertySheet* pParent = (CPropertySheet*)GetParent(); // 获得属性表的指针
    if (NULL != pParent)
    {
        pParent->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);//上一步，完成
    }
    if(NULL == m_pSegParam)
    {
        return FALSE;
    }
    m_formatter.Init(m_pSegParam->m_dwNodeNum);
    m_formatter.ConvertFromString(m_pSegParam->m_strNodeParam.c_str());
    m_listQ.DeleteAllItems();

    CString strQ;
    for (int i = 0; i < (int)m_pSegParam->m_dwNodeNum; i++)
    {
        strQ.Format(_T("%d"), i + 1);
        if (0 == i)
        {
            strQ += _T("(左)");
        }
        else if (m_pSegParam->m_dwNodeNum - 1 == i)
        {
            strQ += _T("(右)");
        }
        m_listQ.InsertItem(i, strQ);

        UpdateQ(i);
    }

    return CXTPPropertyPage::OnSetActive();
}


LRESULT CSegNodeSelDlg::OnWizardBack()
{
    // TODO: 在此添加专用代码和/或调用基类
    DoSave();

    return CXTPPropertyPage::OnWizardBack();
}

BOOL CSegNodeSelDlg::OnWizardFinish()
{
    // TODO: 在此添加专用代码和/或调用基类
    DoSave();
	SoftCrossCalculatePage::firstShowMessageBox = true;
    return CXTPPropertyPage::OnWizardFinish();
}

BOOL CSegNodeSelDlg::DoSave()
{
    if(NULL == m_pSegParam)
    {
        return FALSE;
    }
    for (int i = 0; i < (int)m_pSegParam->m_dwNodeNum; i++)
    {
        NodeSelData sel;
        CString strText;
        strText = m_listQ.GetItemText(i, 1);
        sel.m_dwSelfId = GetIdByName(m_mapNodeNameId, strText);
        strText = m_listQ.GetItemText(i, 2);
        sel.m_dwIsHang = (strText == _T("是")) ? 1 : 0;
        sel.m_strPreCrossLen = m_listQ.GetItemText(i, 3);
        sel.m_strNextCrossLen = m_listQ.GetItemText(i, 4);
        strText = m_listQ.GetItemText(i, 5);
        sel.m_dwCatenaryId = GetIdByName(m_mapCatenaryNameId, strText);
        strText = m_listQ.GetItemText(i, 6);
        sel.m_dwTouchLineId = GetIdByName(m_mapTouchLineNameId, strText);
		sel.m_strDistanceToPillar = m_listQ.GetItemText(i, 7);

        m_formatter.SetNodeSel(sel, i);
    }

    CString strRes;
    m_formatter.ConvertToString(strRes);

    SoftCrossSegParam param;
    param.m_strNodeParam = strRes;
    CSegParamDBManager db;
    db.Update(param, m_pSegParam->m_dwSegId, UPDATETYPE_NODE);
    return TRUE;
}

DWORD CSegNodeSelDlg::GetIdByName(map<CString, DWORD>& mapNameId, const CString& strName)
{
    map<CString, DWORD>::iterator it;
    it = mapNameId.find(strName);
    if (it != mapNameId.end())
    {
        return it->second;
    }
    return -1;
}

BOOL CSegNodeSelDlg::InitEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText,
    DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
    ASSERT(*pWnd);
    switch(nColumn)
    {
    case 1:
    case 2:
    case 5:
    case 6:
        {
            CComboBox *pCmb = dynamic_cast<CComboBox *>(*pWnd);
            pCmb->SelectString(0, strSubItemText);
        }
        break;
    case 3:
    case 4:
	case 7:
        {
            CEdit* pEdt = dynamic_cast<CEdit *>(*pWnd);
            pEdt->SetWindowText(strSubItemText);
        }
        break;
    }
    return TRUE;
}

BOOL CSegNodeSelDlg::EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText,
    DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
    ASSERT(pWnd);
    switch(nColumn)
    {
    case 1:
    case 2:
    case 5:
    case 6:
        {
            CComboBox *pCmb = dynamic_cast<CComboBox *>(*pWnd);
            int index = pCmb->GetCurSel();
            if(index >= 0) pCmb->GetLBText(index, strSubItemText);
        }
        break;
    case 3:
    case 4:
	case 7:
        {
            if (!strSubItemText.IsEmpty()
                && !CDataValidator::IsUnsignFloat(strSubItemText))
            {
                AfxMessageBox(_T("数据格式错误，请重新输入!"));
                return FALSE;
            }
        }
        break;
    }
    return TRUE;
}

void CSegNodeSelDlg::OnBnClickedNodeset()
{
    // TODO: 在此添加控件通知处理程序代码
    CNodeManagerDlg dlg;
    dlg.DoModal();
}
