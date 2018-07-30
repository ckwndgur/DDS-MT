
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "dds_monitoring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

BOOL CPropertiesWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CString str;
	
	if (HIWORD(wParam) == CBN_SELCHANGE) { //A combo box selection changed
		//AfxMessageBox(_T("combo click"));
		str = "click";
		/*InitPropList();*/
		m_wndPropList.Invalidate();
	}
	return 0;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top /*+ m_nComboHeight + cyTlb*/, rectClient.Width(), rectClient.Height()/* -(m_nComboHeight+cyTlb)*/, SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	//const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE |  WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList() //수정_하연_2018-04-03
{
	SetPropListFont();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}



void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	//m_wndObjectCombo.SetFont(&m_fntPropList);
}

//수정_하연_2018-04-03
void CPropertiesWnd::SetPropertyWnd()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CClassView *pClassView = (CClassView*)pFrame->GetPropertyViewPT();
	CString title;

	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	m_wndPropList.RemoveAll();
	switch (index) {
	case 1: //DR, DW
		pGroup1 = new CMFCPropertyGridProperty(_T("ENTITY"));
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("KIND OF ENTITY"), prop_infor.entity_kind, _T("")));
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Name"), prop_infor.name, _T("")));
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("GUID"), prop_infor.guid, _T("")));

		pGroup3 = new CMFCPropertyGridProperty(_T("ENTITY INFORMATION"));
		pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Host"), prop_infor.host_name, _T("")));
		pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Process"), prop_infor.process_name, _T("")));
		pGroup3->AddSubItem(new CMFCPropertyGridProperty(prop_infor.participant_kind, prop_infor.participant_name, _T("")));
		pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Topic"), prop_infor.topic_name, _T("")));

		m_wndPropList.AddProperty(pGroup1);
		m_wndPropList.AddProperty(pGroup3);
		MakeQosGroup(prop_infor.qos);
		break;
	case 2: //Topic, PUB, SUB, DP
		pGroup1 = new CMFCPropertyGridProperty(_T("ENTITY"));
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("KIND OF ENTITY"), prop_infor.entity_kind, _T("")));
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("NAME"), prop_infor.name, _T("")));
		m_wndPropList.AddProperty(pGroup1);
		MakeQosGroup(prop_infor.qos);
		break;
	case 3: //HOST, PROCESS
		pGroup1 = new CMFCPropertyGridProperty(_T("ENTITY"));
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("KIND OF ENTITY"), prop_infor.entity_kind, _T("")));
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("NAME"), prop_infor.name, _T("")));
		m_wndPropList.AddProperty(pGroup1);
		break;
	case 0:
		m_wndPropList.RemoveAll();
		break;
	}
	
	clearInfor(prop_infor);
	

}

void CPropertiesWnd::clearInfor(Infor i)
{
	i.name = "";
	i.guid = "";
	i.qos = "";
	i.dp = "";
	i.host_name = "";
	i.process_name = "";
	i.participant_kind = "";
	i.participant_name = "";
	i.topic_name = "";
}

/**************
* Purpose: 구분자의 개수 파악 //수정_이하연_2018-03-27
**************/
int CPropertiesWnd::GetFindCharCount(CString parm_string, char parm_find_char)
{
	int length = parm_string.GetLength(), find_count = 0;

	for (int i = 0; i < length; i++)
	{
		if (parm_string[i] == parm_find_char)
		{
			find_count++;
		}
	}

	return find_count;
}

/**************
* Purpose: 속성창에 QoS 리스트 생성 //수정_이하연_2018-03-27
**************/
void CPropertiesWnd::MakeQosGroup(CString qos)
{
	CString* qos_list = NULL;
	CString* qos_detail = NULL;
	CString* qos_value = NULL;

	int sepCount = GetFindCharCount(qos, '}');
	pGroup2 = new CMFCPropertyGridProperty(_T("QoS"));

	qos_list = SubString(qos, '}');

	for (int num = 0; num < sepCount; num++) {
		
		qos_detail = SubString(qos_list[num], '{'); //qos_name{v1:0;v2:1;
		for (int a = 0; a < GetFindCharCount(qos_detail[1], ';'); a++) { //v1:0; v2:1
			qos_value = SubString(qos_detail[1], ';');
			if(a < 1)
				pGroup2->AddSubItem(new CMFCPropertyGridProperty(qos_detail[0], qos_value[a], _T("Specifies the text that will be displayed in the window's title bar")));
			else 
				pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T(""), qos_value[a], _T("Specifies the text that will be displayed in the window's title bar")));
		}
	}
	m_wndPropList.AddProperty(pGroup2);

	delete[] qos_list;
	delete[] qos_detail;
	delete[] qos_value;
}

/**************
* Purpose: 구분자를 기준으로 String split //수정_이하연_2018-03-27
**************/
CString* CPropertiesWnd::SubString(CString str, char ch) {
	CString strTok;
	CString* substr = NULL;
	int cnt = 0;

	delete[] substr;
	int sepCount = GetFindCharCount(str, ch);

	substr = new CString[sepCount + 1];

	while (AfxExtractSubString(strTok, str, cnt, ch)) {
		substr[cnt++] = strTok;
	}

	return substr;
}

void CPropertiesWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDockablePane::OnPaint() for painting messages

	if (bSetValueProp) {
		MakeValueGroup(value);
		bSetValueProp = false;
	}
	else
		SetPropertyWnd();
}

/**************
* Purpose: 속성창에 엔티티 정보(Status, state, timestamp 등) 리스트 생성
**************/
void CPropertiesWnd::MakeValueGroup(CString val)
{
	CString* val_list = NULL;
	CString* val_detail = NULL;
	int sepCount = GetFindCharCount(val, ';');
	
	m_wndPropList.RemoveAll();

	pGroup1 = new CMFCPropertyGridProperty(_T("INFROMATION"));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("state"), entity_state, _T("")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("status"), entity_status, _T("")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("DR's TimeStamp"), entity_time1, _T(""))); 
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("DW's TimeStamp"), entity_time2, _T(""))); 
	m_wndPropList.AddProperty(pGroup1);

	pGroup2 = new CMFCPropertyGridProperty(_T("VALUE"));
	val_list = SubString(val, ';');

	for (int num = 0; num < sepCount; num++) {
		val_detail = SubString(val_list[num], ':');
		pGroup2->AddSubItem(new CMFCPropertyGridProperty(val_detail[0], val_detail[1], _T("")));

	}
	m_wndPropList.AddProperty(pGroup2);

	delete[] val_list;
	delete[] val_detail;
}

CString CPropertiesWnd::ValueFromView(CString val) {
	value = val;
	return value;
}
