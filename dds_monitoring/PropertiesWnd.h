
#pragma once

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{


// Construction
public:
	CPropertiesWnd();

	void AdjustLayout();

// Attributes
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);

// Implementation
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

public:
	struct Infor {
		CString name;
		CString guid;
		CString qos;
		CString dp;
		CString entity_kind;
		CString host_name;
		CString process_name;
		CString participant_kind;
		CString participant_name;
		CString topic_name;
	};

public:
	void SetPropertyWnd();
	CString str;
	CMFCPropertyGridCtrl m_wndPropList;
	int index; 
	void clearInfor(Infor i); //수정_하연_2018-04-03

private:
	CMFCPropertyGridProperty* pGroup1;
	CMFCPropertyGridProperty* pGroup2;
	CMFCPropertyGridProperty* pGroup3;

	int GetFindCharCount(CString parm_string, char parm_find_char);
	void MakeQosGroup(CString qos); 
	CString* SubString(CString str, char ch);
	CString value;



public:
	CString entity_state, entity_status, entity_time1, entity_time2;
	void MakeValueGroup(CString val);
	CString ValueFromView(CString val);
	BOOL bSetValueProp = false;
	Infor prop_infor; 
};

