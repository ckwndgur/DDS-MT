#include "stdafx.h"
#include "uiManager.h"



UIManager::UIManager()
{
}

UIManager::UIManager(const UIManager& sfo)
{
}

UIManager::~UIManager()
{
}

UIManager* UIManager::instance = nullptr;



/**************
* Purpose: 생성자
**************/
UIManager* UIManager::GetInstance() {
	if (instance == NULL) instance = new UIManager();
	return instance;
}

/**************
* Purpose: 엔티티 데이터에 새로 고쳐진 부분이 발생하였기 떄문에 뷰어의 새로 고침이 필요함
**************/
void UIManager::updateEntityUI()
{
	flagUpdateEntity = true;

}
void UIManager::refreshUI() {

	if (flagUpdateEvent) {
		flagUpdateEvent = false;
		MainSystem * m = MainSystem::GetInstance();

		CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
		CChildFrame *pChild = (CChildFrame *)pFrame->GetActiveFrame();
		Cdds_monitoringView *pView = (Cdds_monitoringView *)pChild->GetActiveView();


		unordered_map<int, listener_event>::iterator i;

		for (i = m->currentDataSet.begin(); i != m->currentDataSet.end(); i++) {
			pView->m_lstListenerData.push_back(i->second);
		}

		pView->bUpdateEntity = true;
		pView->Invalidate();
	}
	if (flagUpdateEntity) {
		flagUpdateEntity = false;
		MainSystem * m = MainSystem::GetInstance();

		Cdds_monitoringApp *p = (Cdds_monitoringApp*)AfxGetApp();
		CMainFrame* MainWnd = (CMainFrame*)p->GetMainWnd();
		CClassView *pClass2 = (CClassView*)MainWnd->GetClassViewPT();


		unordered_map<int, entity>::iterator i;
		/*for (i = m->currentEntitySet.begin(); i != m->currentEntitySet.end(); i++) {
		if (i->second.dataReader == "") cout << i->first << " is datawriter." << endl;
		else  cout << i->first << " is dataReader." << endl;
		}*/
		for (i = m->currentEntitySet.begin(); i != m->currentEntitySet.end(); i++) {
			//pView->m_lstEntitiesData.push_back(i->second);
			pClass2->tv_entity.push_back(i->second);
		}
		//pView->bUpdateEntity = true;


		pClass2->Invalidate();
	}


}
//Cdds_monitoringView* UIManager::GetMonitoringView()
//{
//	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
//	CChildFrame *pChild = (CChildFrame *)pFrame->GetActiveFrame();
//	Cdds_monitoringView *pView = (Cdds_monitoringView *)pChild->GetActiveView();
//
//
//	return pView;
//}

/**************
* Purpose: 리스너 데이터에 새로 고쳐진 부분이 발생하였기 떄문에 뷰어의 새로 고침이 필요함
**************/
void UIManager::updateListenerEventUI()
{
	flagUpdateEvent = true;
}



