#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "dds_monitoring.h"

#include "dds_monitoringView.h"
#include "ChildFrm.h"
#include <cstdlib>
//#include "afxtempl.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CClassView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		//pImages = CMFCToolBar::GetImages();

		//CAfxDrawState ds;
		//pImages->PrepareDrawImage(ds);

		//CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		//pImages->EndDrawImage(ds);

	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClassView::CClassView()
{
	//m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

void CClassView::ReadXML()
{

}

CClassView::~CClassView()
{
}



BEGIN_MESSAGE_MAP(CClassView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CClassView::OnSelChanged)
	ON_WM_PAINT()
	ON_COMMAND(ID_DO_NOT_SAVE, &CClassView::OnDoNotSave)
	ON_COMMAND(ID_DO_NOT_SAVE2, &CClassView::OnDoNotSave2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

void CClassView::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	AfxMessageBox(_T("t"));
	*pResult = 0;
}

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{



	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;


	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_CHECKBOXES;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect(&rectCombo);

	m_nComboHeight = rectCombo.Height();


	OnChangeVisualStyle();
	bselcombo = false;

	btreectrl = false;


	return 0;
}

void CClassView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

/**************
* Purpose: TreeView 내에 아이템이 존재하는지 검색
**************/
HTREEITEM CClassView::FindTreeData(CTreeCtrl* pTree, HTREEITEM parentItem, string ItemName, int kind) //수정_하연_20180513
{
	HTREEITEM hFindItem, hChildItem;
	CString CompareItemName(ItemName.c_str());
	CString GetItemStr = pTree->GetItemText(parentItem);

	if (GetItemStr.CompareNoCase(CompareItemName) == 0) {
		return parentItem;
	}

	if (kind == HOST)
		hChildItem = pTree->GetNextItem(parentItem, TVGN_NEXT);
	else
		hChildItem = pTree->GetChildItem(parentItem);

	while (hChildItem) {
		hFindItem = FindTreeData(pTree, hChildItem, ItemName);

		if (hFindItem) return hFindItem;
		hChildItem = pTree->GetNextItem(hChildItem, TVGN_NEXT);
	}

	return NULL;
}


/**************
* Purpose: TreeView에 Host 아이템 생성
**************/
void CClassView::SetTreeHost(entity d) //수정_하연_20180513
{
	//Host가 추가되어있지 않을 때,
	if (FindTreeData(mtree, mtree->GetRootItem(), d.host, HOST) == NULL) {
		tree_host = m_wndClassView.InsertItem(ConvertString(d.host), HOST, HOST); //수정_하연_20180513
	}
	//Hsot가 존재할 때
	else {
		tree_host = FindTreeData(mtree, mtree->GetRootItem(), d.host, HOST);
	}
	SetTreeProcess(d);

	m_wndClassView.SetItemState(tree_host, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
}

/**************
* Purpose: TreeView에 Process 아이템 생성
**************/
void CClassView::SetTreeProcess(entity d)
{
	DWORD test = 0;

	//Process가 추가되어있지 않을 때,
	if (FindTreeData(mtree, tree_host, d.process) == NULL) {
		tree_process = m_wndClassView.InsertItem(ConvertString(d.process), PROCESS, PROCESS, tree_host);
	}
	//Process가 존재할 때
	else {
		tree_process = FindTreeData(mtree, tree_host, d.process);

	}

	SetTreeDomainParticipant(d);


	bool view_test = 0;
	bool save_test = 0;
	if (FilterInfo::GetInstance()->currentFilterData.size() > 0) {
		unordered_map<int, filterData>::iterator i;
		for (i = FilterInfo::GetInstance()->currentFilterData.begin(); i != FilterInfo::GetInstance()->currentFilterData.end(); i++) {
			if (i->second.e.domainParticipant == "") {
				if (i->second.e.process == d.process) {
					if (i->second.forView) {
						view_test = 1;
					}
					else {
						save_test = 1;
						// 흐리게 하기 코드 추가
						m_wndClassView.SetItemState(tree_process, TVGN_FIRSTVISIBLE, TVGN_FIRSTVISIBLE);
					}
				}
			}
		}
	}

	if (save_test) {
		m_wndClassView.SetItemState(tree_process, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
	}
	else {
		m_wndClassView.SetItemState(tree_process, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);
		if (view_test) {
			m_wndClassView.SetCheck(tree_process, false);
		}
		else {
			m_wndClassView.SetCheck(tree_process, true);
		}

	}
}

/**************
* Purpose: TreeView에 DomainParticipant 아이템 생성
**************/
void CClassView::SetTreeDomainParticipant(entity d)
{
	if (FindTreeData(mtree, tree_process, d.domainParticipant) == NULL) {
		tree_dp = m_wndClassView.InsertItem(ConvertString(d.domainParticipant), DOMAINPARTICIPANT, DOMAINPARTICIPANT, tree_process);
	}
	else {
		tree_dp = FindTreeData(mtree, tree_process, d.domainParticipant);

	}

	SetTreeTopics(d);
	SetTreeClients(d);

	bool view_test = 0;
	bool save_test = 0;
	if (FilterInfo::GetInstance()->currentFilterData.size() > 0) {
		unordered_map<int, filterData>::iterator i;
		for (i = FilterInfo::GetInstance()->currentFilterData.begin(); i != FilterInfo::GetInstance()->currentFilterData.end(); i++) {
			if (i->second.e.process == d.process) {
				if (i->second.e.domainParticipant == d.domainParticipant) {
					if (i->second.e.publisher == "") {
						if (i->second.forView) {
							view_test = 1;
						}
						else {
							save_test = 1;
							// 흐리게 하기 코드 추가
							m_wndClassView.SetItemState(tree_dp, TVGN_FIRSTVISIBLE, TVGN_FIRSTVISIBLE);

						}
					}
				}
				else if (i->second.e.domainParticipant == "") {
					if (i->second.forView) {
						view_test = 1;
					}
					else {
						save_test = 1;
					}
				}
			}
		}
	}

	if (save_test) {
		m_wndClassView.SetItemState(tree_dp, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
	}
	else {
		m_wndClassView.SetItemState(tree_dp, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);
		if (view_test) {
			m_wndClassView.SetCheck(tree_dp, false);
		}
		else {
			m_wndClassView.SetCheck(tree_dp, true);
		}

	}
}

/**************
* Purpose: TreeView에 Topic 아이템 생성
**************/
void CClassView::SetTreeTopics(entity d)
{
	string topic = "Topics";
	HTREEITEM tree_topic_item = NULL; //수정_하연_20180513

	if (d.topicName.compare("") != 0) {
		if (FindTreeData(mtree, tree_dp, topic) == NULL) {
			tree_topic = m_wndClassView.InsertItem(ConvertString(topic), TOPIC_ROOT, TOPIC_ROOT, tree_dp);

			if (FindTreeData(mtree, tree_topic, d.topicName) == NULL) {
				tree_topic_item = m_wndClassView.InsertItem(ConvertString(d.topicName), TOPIC, TOPIC, tree_topic);
			}
		}
		else {
			tree_topic = FindTreeData(mtree, tree_dp, topic);
			if (FindTreeData(mtree, tree_topic, d.topicName) == NULL) {
				tree_topic_item = m_wndClassView.InsertItem(ConvertString(d.topicName), TOPIC, TOPIC, tree_topic);
			}
		}
	}

	bool view_test = 0;
	bool save_test = 0;
	if (FilterInfo::GetInstance()->currentFilterData.size() > 0) {
		unordered_map<int, filterData>::iterator i;
		for (i = FilterInfo::GetInstance()->currentFilterData.begin(); i != FilterInfo::GetInstance()->currentFilterData.end(); i++) {
			if (i->second.e.topicName == d.topicName) {
				if (i->second.forView) {
					view_test = 1;
				}
				else {
					save_test = 1;
					// 흐리게 하기 코드 추가
					m_wndClassView.SetItemState(tree_topic, TVGN_FIRSTVISIBLE, TVGN_FIRSTVISIBLE);
				}

			}
		}
	}
	//HTREEITEM topic_child = m_wndClassView.GetChildItem(tree_topic);
	if (save_test) {//수정_하연_20180513
		m_wndClassView.SetItemState(tree_topic_item, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
	}
	else {
		m_wndClassView.SetItemState(tree_topic_item, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);
		if (view_test) {
			m_wndClassView.SetCheck(tree_topic_item, false);
		}
		else {
			m_wndClassView.SetCheck(tree_topic_item, true);
		}

	}
	m_wndClassView.SetItemState(tree_topic, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
}


/**************
* Purpose: TreeView에 Pub, Sub, DataWriter, DataReader 아이템 생성
**************/
void CClassView::SetTreeClients(entity d)
{
	string id;
	HTREEITEM tmp_tree_dw = NULL, tmp_tree_dr = NULL, tmp_tree_sub = NULL, tmp_tree_pub = NULL;

	//Subscriber
	if (d.publisher == "") {//수정_하연_20180513
		id = d.subscriber;
		if (FindTreeData(mtree, tree_dp, d.subscriber) == NULL) {

			tree_sub = m_wndClassView.InsertItem(ConvertString(id), SUB, SUB, tree_dp);
			tmp_tree_sub = tree_sub;
			tmp_tree_dr = m_wndClassView.InsertItem(ConvertString(d.dataReader), DATAREADER, DATAREADER, tree_sub);
		}
		else {
			tree_sub = FindTreeData(mtree, tree_dp, d.subscriber);
			if (FindTreeData(mtree, tree_sub, d.dataWriter) == NULL) {
				tmp_tree_sub = tree_sub;
				tmp_tree_dr = m_wndClassView.InsertItem(ConvertString(d.dataReader), DATAREADER, DATAREADER, tree_sub);
			}
		}
	}
	//Publisher
	else if (d.subscriber == "") {//수정_하연_20180513
		id = d.publisher;

		if (FindTreeData(mtree, tree_dp, d.publisher) == NULL) {
			tree_pub = m_wndClassView.InsertItem(ConvertString(id), PUB, PUB, tree_dp);
			tmp_tree_pub = tree_pub;
			tmp_tree_dw = m_wndClassView.InsertItem(ConvertString(d.dataWriter), DATAWRITER, DATAWRITER, tree_pub);
		}
		else {
			tree_pub = FindTreeData(mtree, tree_dp, d.publisher);
			if (FindTreeData(mtree, tree_pub, d.dataReader) == NULL) {
				tmp_tree_pub = tree_pub;
				tmp_tree_dw = m_wndClassView.InsertItem(ConvertString(d.dataWriter), DATAWRITER, DATAWRITER, tree_pub);
			}
		}

	}

	bool view_test_dr = 0, view_test_sub = 0, view_test_dw = 0, view_test_pub = 0;
	bool save_test_dr = 0, save_test_sub = 0, save_test_dw = 0, save_test_pub = 0;

	if (FilterInfo::GetInstance()->currentFilterData.size() > 0) {
		unordered_map<int, filterData>::iterator i;
		for (i = FilterInfo::GetInstance()->currentFilterData.begin(); i != FilterInfo::GetInstance()->currentFilterData.end(); i++) {
			if (i->second.e.process == d.process) {
				if (i->second.e.domainParticipant == d.domainParticipant) {
					if (i->second.e.publisher == d.publisher) {
						if (i->second.e.dataWriter == d.dataWriter) {
							if (i->second.forView) {
								view_test_dw = 1;
							}
							else {
								save_test_dw = 1;
								// 흐리게 하기 코드 추가
								m_wndClassView.SetItemState(tmp_tree_dw, TVGN_FIRSTVISIBLE, TVGN_FIRSTVISIBLE);
							}
						}
						else if (i->second.e.dataWriter == "") {
							if (i->second.forView) {
								view_test_pub = 1;
							}
							else {
								save_test_pub = 1;
								// 흐리게 하기 코드 추가
								m_wndClassView.SetItemState(tmp_tree_pub, TVGN_FIRSTVISIBLE, TVGN_FIRSTVISIBLE);
							}
						}
					}
					else if (i->second.e.publisher == d.subscriber) {
						if (i->second.e.dataWriter == d.dataReader) {
							if (i->second.forView) {
								view_test_dr = 1;
							}
							else {
								save_test_dr = 1;
								m_wndClassView.SetItemState(tmp_tree_dr, TVGN_FIRSTVISIBLE, TVGN_FIRSTVISIBLE);

							}
						}
						else if (i->second.e.dataWriter == "") {
							if (i->second.forView) {
								view_test_sub = 1;
							}
							else {
								save_test_sub = 1;
								m_wndClassView.SetItemState(tmp_tree_sub, TVGN_FIRSTVISIBLE, TVGN_FIRSTVISIBLE);
							}
						}
					}
					else if (i->second.e.publisher == "") {
						if (i->second.forView) {
							view_test_sub = 1;
							view_test_pub = 1;
						}
						else {
							save_test_sub = 1;
							save_test_pub = 1;
						}
					}
				}
				else if (i->second.e.domainParticipant == "") {
					if (i->second.forView) {
						view_test_sub = 1;
						view_test_pub = 1;
					}
					else {
						save_test_sub = 1;
						save_test_pub = 1;
					}
				}
			}
		}
	}


	if (save_test_sub) {
		m_wndClassView.SetItemState(tmp_tree_sub, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		m_wndClassView.SetItemState(tmp_tree_dr, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
	}
	else {
		m_wndClassView.SetItemState(tmp_tree_sub, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);

		if (save_test_dr) {
			m_wndClassView.SetItemState(tmp_tree_dr, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		}
		else {
			m_wndClassView.SetItemState(tmp_tree_dr, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);
		}

		if (view_test_sub) {
			m_wndClassView.SetCheck(tmp_tree_sub, false);
			m_wndClassView.SetCheck(tmp_tree_dr, false);
		}
		else {
			m_wndClassView.SetCheck(tmp_tree_sub, true);

			if (view_test_dr) {
				m_wndClassView.SetCheck(tmp_tree_dr, false);
			}
			else {
				m_wndClassView.SetCheck(tmp_tree_dr, true);
			}

		}
	}

	if (save_test_pub) {
		m_wndClassView.SetItemState(tmp_tree_pub, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		m_wndClassView.SetItemState(tmp_tree_dw, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
	}
	else {
		m_wndClassView.SetItemState(tmp_tree_pub, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);

		if (save_test_dw) {
			m_wndClassView.SetItemState(tmp_tree_dw, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		}
		else {
			m_wndClassView.SetItemState(tmp_tree_dw, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);
		}

		if (view_test_pub) {
			m_wndClassView.SetCheck(tmp_tree_pub, false);
			m_wndClassView.SetCheck(tmp_tree_dw, false);
		}
		else {
			m_wndClassView.SetCheck(tmp_tree_pub, true);

			if (view_test_dw) {
				m_wndClassView.SetCheck(tmp_tree_dw, false);
			}
			else {
				m_wndClassView.SetCheck(tmp_tree_dw, true);
			}

		}


	}
}


/**************
* Purpose: String to CString
**************/
CString CClassView::ConvertString(string str)
{
	CString C_str(str.c_str());
	return C_str;
	LPCTSTR convert_str = C_str.GetBuffer(0);
	C_str.ReleaseBuffer();

	return convert_str;
}

/**************
* Purpose: TreeView의 모든 아이템 확장 및 축소
**************/
void CClassView::ExpandAll(HTREEITEM rootItem) {
	int state;

	
	
	if (m_wndClassView.ItemHasChildren(rootItem))
	{
		if ((m_wndClassView.GetItemState(rootItem, TVGN_FIRSTVISIBLE) == TVGN_FIRSTVISIBLE) || !m_wndClassView.GetCheck(rootItem)) //수정_하연_2018-04-04
			m_wndClassView.Expand(rootItem, TVE_COLLAPSE);

		else
			m_wndClassView.Expand(rootItem, TVE_EXPAND);
		

		//자식 노드 확인
		rootItem = m_wndClassView.GetChildItem(rootItem);
		if (rootItem)
		{
			do
			{
				// 재귀로 하위 확인  
				ExpandAll(rootItem);
			} while ((rootItem = m_wndClassView.GetNextSiblingItem(rootItem)) != NULL);
		}
	}

}



/**************
* Purpose: 선택한 Tree 아이템의 정보(Data) 추출
**************/
void CClassView::GetSelectItemData(int nkind, entity e)
{
	unordered_map<int, entity>::iterator i;
	//size_t lineno = tv_ListenerData.size() - 1;


	for (i = MainSystem::GetInstance()->currentEntitySet.begin(); i != MainSystem::GetInstance()->currentEntitySet.end(); ++i) {
		entity entity_data = i->second;


		//for (size_t lineno = tv_ListenerData.size() - 1; lineno < tv_ListenerData.size(); lineno--) {
		//listener_event mMessageData = MainSystem::GetInstance()->currentDataSet.find(tv_ListenerData.size() - lineno)->second;
		switch (nkind) {
		case DATAREADER:
			if ((entity_data.host == e.host) && (entity_data.process == e.process) && (entity_data.domainParticipant == e.domainParticipant)
				&& (entity_data.subscriber == e.subscriber) && (entity_data.dataReader == e.dataReader)) {
				PropertyIndex = 1; //DR
				select_no = entity_data.id;
				infor.entity_kind = "DataReader";
				infor.participant_kind = "Subscriber";
				infor.participant_name = entity_data.subscriber.c_str();;
				infor.name = entity_data.dataReader.c_str();
				infor.qos = entity_data.qos.c_str();
				infor.topic_name = entity_data.topicName.c_str(); //수정_하연_20180513
			}
			break;
		case DATAWRITER:
			if ((entity_data.host == e.host) && (entity_data.process == e.process) && (entity_data.domainParticipant == e.domainParticipant)
				&& (entity_data.publisher == e.publisher) && (entity_data.dataWriter == e.dataWriter)) {

				PropertyIndex = 1; //DW
				select_no = entity_data.id;
				infor.entity_kind = "DataWriter";
				infor.participant_kind = "Publisher";
				infor.participant_name = entity_data.publisher.c_str();;
				infor.name = entity_data.dataWriter.c_str();
				infor.qos = entity_data.qos.c_str();
				infor.topic_name = entity_data.topicName.c_str(); //수정_하연_20180513
			}
			break;
		case TOPIC:
			if ((entity_data.host == e.host) && (entity_data.process == e.process) && (entity_data.domainParticipant == e.domainParticipant) && (entity_data.topicName == e.topicName)) {

				PropertyIndex = 2; //Topic(세부)
				infor.entity_kind = "Topic";
				infor.name = entity_data.topicName.c_str();
				infor.qos = MainSystem::GetInstance()->getQOS(e, "topic").c_str();
				infor.topic_name = entity_data.topicName.c_str(); //수정_하연_20180513
			}
			break;
		case PUB:
			if ((entity_data.host == e.host) && (entity_data.process == e.process) && (entity_data.domainParticipant == e.domainParticipant)) {

				PropertyIndex = 2;
				infor.entity_kind = "Publisher";
				infor.name = entity_data.publisher.c_str();
				infor.qos = MainSystem::GetInstance()->getQOS(e, "pub").c_str();
				infor.topic_name = entity_data.topicName.c_str(); //수정_하연_20180513
			}
			break;
		case SUB:
			if ((entity_data.host == e.host) && (entity_data.process == e.process) && (entity_data.domainParticipant == e.domainParticipant)) {

				PropertyIndex = 2;
				infor.entity_kind = "Subscriber";
				infor.name = entity_data.subscriber.c_str();
				infor.qos = MainSystem::GetInstance()->getQOS(e, "sub").c_str();
				infor.topic_name = entity_data.topicName.c_str(); //수정_하연_20180513
			}
			break;
		case TOPIC_ROOT:
			PropertyIndex = 0;
			infor.entity_kind = "Topic";
			infor.name = "";
			break;
		case DOMAINPARTICIPANT:
			if ((entity_data.host == e.host) && (entity_data.process == e.process) && (entity_data.domainParticipant == e.domainParticipant)) {
				PropertyIndex = 2;
				infor.entity_kind = "DomainParticipant";
				infor.name = entity_data.domainParticipant.c_str();
				infor.qos = MainSystem::GetInstance()->getQOS(e, "dp").c_str();
			}
			break;
		case PROCESS:
			if ((entity_data.host == e.host) && (entity_data.process == e.process)) {
				PropertyIndex = 3;
				infor.entity_kind = "Process";
				infor.name = entity_data.process.c_str();
			}
			break;
		case HOST:
			if ((entity_data.host == e.host)) {
				PropertyIndex = 3;
				infor.entity_kind = "Host";
				infor.name = entity_data.host.c_str();
			}
			break;
		default:
			PropertyIndex = 0;
			break;

		}

		//}

		infor.guid = entity_data.guid.c_str();
		infor.host_name = entity_data.host.c_str();
		infor.process_name = entity_data.process.c_str();
		infor.dp = entity_data.domainParticipant.c_str();
		//infor.topic_name = entity_data.topicName.c_str();

	}
}


void CClassView::FillClassView()
{
	if (btreectrl) {
		m_wndClassView.DeleteAllItems();
		unordered_map<int, entity>::iterator i;

		for (i = MainSystem::GetInstance()->currentEntitySet.begin(); i != MainSystem::GetInstance()->currentEntitySet.end(); ++i) {
			if (sel_domainid != "DomainList") {
				//m_wndClassView.SetItemText(tree_host, sel_domainid);
				if (i->second.domainId.c_str() == sel_domainid && i->second.guid != "")//수정_하연_20180513
				{
					SetTreeHost(i->second);
				}
			}
		}
		while (tree_host != NULL) { //수정_하연_20180513
			ExpandAll(tree_host);
			tree_host = m_wndClassView.GetNextItem(tree_host, TVGN_NEXT);
		}
	}
	btreectrl = false;

}



void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CEditTreeCtrlEx* pWndTree = (CEditTreeCtrlEx*)&m_wndClassView;
	ASSERT_VALID(pWndTree);


	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		ritem = hTreeItem;

		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;

	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

}

void CClassView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndClassView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + m_nComboHeight/* + cyTlb */ + 1, rectClient.Width() - 2, rectClient.Height()
		- m_nComboHeight/* - cyTlb*/ - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}



/**************
* Purpose: 선택한 Tree 아이템이 속한 엔티티 추출
**************/
entity CClassView::SetSelecteditemEntity(HTREEITEM selItem)
{
	int nImage, nSelImage;

	CString parent_name;
	CString domainid;
	CString child_name = mtree->GetItemText(selItem); //get the selected item text

													  // Convert a TCHAR string to a LPCSTR
	CT2CA C_str_name(child_name);
	// Construct a std::string using the LPCSTR input
	string s_child_name(C_str_name);

	int index = m_wndObjectCombo.GetCurSel();
	m_wndObjectCombo.GetLBText(index, domainid);

	CT2CA C_str_domainid(domainid);
	// Construct a std::string using the LPCSTR input
	string s_domainid(C_str_domainid);

	selItem_entity.domainId = s_domainid;
	mtree->GetItemImage(selItem, nImage, nSelImage);

	switch (nImage) {
	case HOST:
		selItem_entity.host = s_child_name;
		break;
	case PROCESS:
		selItem_entity.process = s_child_name;
		break;
	case DOMAINPARTICIPANT:
		selItem_entity.domainParticipant = s_child_name;
		break;
	case PUB:
		selItem_entity.publisher = s_child_name;
		break;
	case SUB:
		selItem_entity.subscriber = s_child_name;
		break;
	case DATAWRITER:
		selItem_entity.dataWriter = s_child_name;
		break;
	case DATAREADER:
		selItem_entity.dataReader = s_child_name;
		break;
	case TOPIC:
		selItem_entity.topicName = s_child_name;
		break;
	case TOPIC_ROOT:
		break;
	}

	parentitem = mtree->GetNextItem(selItem, TVGN_PARENT);

	while (parentitem != NULL) {
		parent_name = mtree->GetItemText(parentitem);
		SetSelecteditemEntity(parentitem);
	}

	return selItem_entity;
}


/**************
* Purpose: entity clear
**************/
entity CClassView::ClearEntity(entity e) {
	e.host = "";
	e.process = "";
	e.domainParticipant = "";
	e.topicName = "";
	e.publisher = "";
	e.subscriber = "";
	e.dataReader = "";
	e.dataWriter = "";

	return e;
}

/**************
* Purpose: 도메인 리스트를 콤보박스로 생성
**************/
void CClassView::Setdomainlst()
{
	unordered_map<int, entity>::iterator i;
	list<CString> domainlst;
	CString domainid;

	m_wndObjectCombo.ResetContent();
	m_wndObjectCombo.AddString(_T("DomainList"));

	for (i = MainSystem::GetInstance()->currentEntitySet.begin(); i != MainSystem::GetInstance()->currentEntitySet.end(); ++i) {

		domainid = (i->second.domainId).c_str();
		domainlst.push_back(domainid);
	}

	domainlst.sort();
	domainlst.unique();

	for (list<CString>::iterator k = domainlst.begin(); k != domainlst.end(); ++k) {
		m_wndObjectCombo.AddString(k->GetString());
	}
}


/**************
* Purpose: 선택한 Tree 아이템의 엔티티 정보 추출
**************/
entity CClassView::GetItemEntity(HTREEITEM hitem) {


	HTREEITEM tmp = hitem;

	int b = 0;
	while (mtree->GetParentItem(tmp) != NULL)
	{
		tmp = mtree->GetParentItem(tmp);
		b++;
	}
	tmp = hitem;
	int d = b;
	entity e = { -1,"","","","","","","", "", "", "", "","" };

	bool break_flag = false;
	while (d > 0) {
		if (!break_flag) {
			if (d == 1) {
				e.process = (string)(CStringA)(mtree->GetItemText(tmp));
			}
			else if (d == 2) {
				e.domainParticipant = (string)(CStringA)(mtree->GetItemText(tmp));
			}
			else if (d == 3) {
				e.publisher = (string)(CStringA)(mtree->GetItemText(tmp));
			}
			else if (d == 4) {
				HTREEITEM level3 = mtree->GetParentItem(tmp);
				string tmp_text = (string)(CStringA)(mtree->GetItemText(level3));
				if (tmp_text == "Topics") {
					e.topicName = (string)(CStringA)(mtree->GetItemText(tmp));
					break_flag = true;
				}
				else {
					e.dataWriter = (string)(CStringA)(mtree->GetItemText(tmp));
				}
			}
		}
		tmp = mtree->GetParentItem(tmp);
		d--;
	}
	return e;
}

BOOL CClassView::PreTranslateMessage(MSG* pMsg)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pFrame->GetActiveFrame();
	CPropertiesWnd *pProWnd = (CPropertiesWnd*)pFrame->GetPropertyViewPT();
	Cdds_monitoringView *pView = (Cdds_monitoringView *)pChild->GetActiveView();

	HTREEITEM selected_item = NULL;
	CString str_selected_item;
	int nImage, nSelectedImage;

	DWORD cur_pos = ::GetMessagePos();
	CPoint point(LOWORD(cur_pos), HIWORD(cur_pos));
	mtree->ScreenToClient(&point);

	UINT nFlags = 0;
	HTREEITEM hitem = mtree->HitTest(point, &nFlags);


	entity e;

	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN: //마우스 왼쪽버튼 클릭

		
		if (hitem != NULL && (nFlags & TVHT_ONITEMSTATEICON) != 0) {

			entity e = GetItemEntity(hitem);
			if (mtree->GetCheck(hitem)) { //체크된 상태
				FilterInfo::GetInstance()->setViewOption(e);
				//ChildItem이 존재하면 Collapse.
				m_wndClassView.Expand(hitem, TVE_COLLAPSE); //수정_하연_2018-04-04
			}
			else {
				FilterInfo::GetInstance()->setViewOptionDelete(e);
				//ChildItem이 존재하면 Expand
				m_wndClassView.Expand(hitem, TVE_EXPAND); //수정_하연_2018-04-04
			}
			if (e.topicName != "") {
				UIManager::GetInstance()->updateEntityUI();
			}
			UIManager::GetInstance()->updateListenerEventUI();
			UIManager::GetInstance()->refreshUI();

		}
		
		MainSystem::GetInstance()->getData();
		UIManager::GetInstance()->refreshUI();


		TV_HITTESTINFO hit_info;
		::GetCursorPos(&hit_info.pt);
		::ScreenToClient(m_wndClassView.m_hWnd, &hit_info.pt);

		selected_item = m_wndClassView.HitTest(&hit_info);

		str_selected_item = m_wndClassView.GetItemText(selected_item);
		select_no = -1;

		if (selected_item != NULL) {
			m_wndClassView.GetItemImage(selected_item, nImage, nSelectedImage);
			GetSelectItemData(nImage, SetSelecteditemEntity(selected_item));
		}

		if (pView != nullptr) { //수정_하연_2018-04-26
			CMFCTabCtrl *wndTab = UIManager::GetInstance()->wndTab;
		
			if (wndTab != NULL) {
				for (int i = 0; i < wndTab->GetTabsNum(); i++) {
					CString title;
					wndTab->GetTabLabel(i, title);
					if (title.Find(_T("List_")) >= 0) {
						pView->setFocus(nImage, SetSelecteditemEntity(selected_item));
					}
					else if (title.Find(_T("Relationship_")) >= 0) //수정 중혁 2018.05.02
					{
						RelationshipView *R = RelationshipView::GetInstance();
						if (nImage < 8)
						{
							position curentP = R->Find_Position(nImage, SetSelecteditemEntity(selected_item));

							if (!prePosition.bclick)
							{
								curentP.bclick = true;
								R->Edit_Position(nImage, curentP);
								prePosition = curentP;
								prenImage = nImage;
							}
							else
							{
								if (!(prePosition.x == curentP.x && prePosition.y == curentP.y))
								{
									prePosition.bclick = false;
									R->Edit_Position(prenImage, prePosition);

									curentP.bclick = true;
									R->Edit_Position(nImage, curentP);
									prePosition = curentP;
									prenImage = nImage;

								}
							}
							pView->Invalidate();
						}
					}
				}
			}
		}

		//Treeview -> Propertyview
		pProWnd->index = PropertyIndex;
		pProWnd->m_wndPropList.RemoveAll();
		pProWnd->clearInfor(infor); 
		pProWnd->prop_infor = infor; 
		pProWnd->Invalidate();

		selItem_entity = ClearEntity(selItem_entity);
		break;

	case WM_LBUTTONDBLCLK:
		UIManager::GetInstance()->updateEntityUI();
		btreectrl = true;
		break;

	case WM_RBUTTONDOWN:
		sb_pos = m_wndClassView.GetScrollPos(SB_VERT);
		break;

	}
	return CDockablePane::PreTranslateMessage(pMsg);
}

BOOL CClassView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam)) {
	case CBN_SELCHANGE:
		bselcombo = true;

		CBindex = m_wndObjectCombo.GetCurSel();
		m_wndObjectCombo.GetLBText(CBindex, sel_domainid);


		if (sel_domainid != "DomainList") {
			btreectrl = true;
			MainSystem::GetInstance()->sel_domainid = sel_domainid;
			MainSystem::GetInstance()->bSelDomain = true;
		}
		else {
			btreectrl = false;
		}


		UIManager::GetInstance()->updateListenerEventUI();
		UIManager::GetInstance()->refreshUI();
		m_wndClassView.DeleteAllItems();
		Invalidate();
		break;

	case CBN_CLOSEUP:
		break;
	}
	return 0;
}




void CClassView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void CClassView::OnClassDefinition()
{
	// TODO: Add your command handler code here
}

void CClassView::OnClassProperties()
{
	// TODO: Add your command handler code here
}

void CClassView::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}

void CClassView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;

	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));

	//m_wndClassView.DeleteAllItems();
	Setdomainlst();
	FillClassView();

	m_wndClassView.SetScrollPos(SB_VERT, sb_pos, 1);

	//btreectrl = false;  //수정_하연_2018-04-02
	if (bselcombo) m_wndObjectCombo.SetCurSel(CBindex);
	else m_wndObjectCombo.SetCurSel(m_wndObjectCombo.GetCount() - 1);

}

void CClassView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CClassView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

}


void CClassView::OnDoNotSave()
{
	// TODO: Add your command handler code here



}


void CClassView::OnDoNotSave2()
{
	// TODO: Add your command handler code here
}