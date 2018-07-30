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
* Purpose: 싱글턴 인스턴스 받아오기
**************/
UIManager* UIManager::GetInstance() {
	if (instance == NULL) instance = new UIManager();
	return instance;
}

/**************
* Purpose: 데이터에 변화가 있다면 현재 보여지고 있는창을 새로고침 하기
**************/
void UIManager::refreshUI() {
	//수정_손민영_2018-02-26

	MainSystem * m = MainSystem::GetInstance();

	
	Cdds_monitoringView *pView = (Cdds_monitoringView *)pListView;
	CCPlotView *pView2 = (CCPlotView *)pGraph1View;
	CCPlotView *pView3 = (CCPlotView *)pGraph2View;
	RelationshipView *pView4 = (RelationshipView *)pRelvView;

		if (flagUpdateEvent) {
			flagUpdateEvent = false;

			if (pView != NULL) {
				pView->bUpdateEntity = true;
				pView->Invalidate();
			}
			if (pView2 != NULL) {
				pView2->setData(pView2->GetDocument(), pView2->graphtype);
				pView2->Invalidate();
			}
			if (pView3 != NULL) {
				pView3->setData(pView3->GetDocument(), pView3->graphtype);
				pView3->Invalidate();
			}
			if (pView4 != NULL) {
			}

		}


	if (flagUpdateEntity) {
		flagUpdateEntity = false;
		
		Cdds_monitoringApp *p = (Cdds_monitoringApp*)AfxGetApp();
		CMainFrame* MainWnd = (CMainFrame*)p->GetMainWnd();
		CClassView *pClass2 = (CClassView*)MainWnd->GetClassViewPT();

		if (pClass2 != nullptr) {
			unordered_map<int, listener_event>::iterator i;
			if (pClass2->tv_ListenerData.size() > 0) pClass2->tv_ListenerData.clear();
			for (i = m->currentDataSet.begin(); i != m->currentDataSet.end(); i++) {
				pClass2->tv_ListenerData.push_back(i->second);
			}

			pClass2->Invalidate(false); //수정_이하연_2018-04-03
		}
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
* Purpose: 엔티티 데이터에 새로 고쳐진 부분이 발생하였다고 설정
**************/
void UIManager::updateEntityUI()
{
	flagUpdateEntity = true;

}
/**************
* Purpose: 리스너 데이터에 새로 고쳐진 부분이 발생하였다고 설정
**************/
void UIManager::updateListenerEventUI()
{
	flagUpdateEvent = true;
}



