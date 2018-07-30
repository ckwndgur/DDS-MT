
#pragma once

#include "ViewTree.h"
#include "PropertiesWnd.h"
#include <string.h>
#include "DataManager.h"
#include "RelationshipView.h"

#define IDC_TREE 2
//#define HOST 0
//#define PROCESS 1
//#define DOMAINPARTICIPANT 2
//#define PUB 3
//#define SUB 4
//#define DATAWRITER 5
//#define DATAREADER 6
//#define TOPIC 7 수정 중혁 2018.04.26
#define TOPIC_ROOT 8

using namespace std;
class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CClassView : public CDockablePane
{
public:
	CClassView();
	void ReadXML();
	virtual ~CClassView();

	void AdjustLayout();
	void OnChangeVisualStyle();

	HTREEITEM ritem = NULL;


protected:
	CComboBox m_wndObjectCombo;
	CViewTree m_wndClassView;
	CPropertiesWnd m_wndProp;
	CImageList m_ClassViewImages;
	UINT m_nCurrSort;
	int select_no;

	// Overrides
public:
	entity GetItemEntity(HTREEITEM hitem);
	void FillClassView();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	int m_nComboHeight;
	int CBindex;
	CTreeCtrl* mtree = (CTreeCtrl*)&m_wndClassView;
	HTREEITEM tree_host, tree_process, tree_dp, tree_topic, tree_pub, tree_sub, parentitem;
	list<entity> tv_entity;
	list<listener_event> tv_ListenerData;
	bool btreectrl; 

private:
	CString ConvertString(string str);
	HTREEITEM FindTreeData(CTreeCtrl* pTree, HTREEITEM parentItem, string ItemName, int kind = 2); //수정_하연_20180513
	void GetSelectItemData(int nkind, entity e);
	void Setdomainlst(); 
	entity SetSelecteditemEntity(HTREEITEM selItem);
	entity ClearEntity(entity e);
	CString sel_domainid;
	bool bselcombo;
	int PropertyIndex;
	CPropertiesWnd::Infor infor;
	entity selItem_entity;
	int sb_pos;
	HTREEITEM tree_system;

	position prePosition; //수정 중혁 2018.05.02
	int prenImage; //수정 중혁 2018.05.02

public:
	void SetTreeHost(entity d);
	void SetTreeProcess(entity d);
	void SetTreeDomainParticipant(entity d);
	void SetTreeTopics(entity d);
	void SetTreeClients(entity d);
	void ExpandAll(HTREEITEM rootItem);
	afx_msg void OnDoNotSave();
	afx_msg void OnDoNotSave2();

};



