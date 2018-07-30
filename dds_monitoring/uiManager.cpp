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
* Purpose: �̱��� �ν��Ͻ� �޾ƿ���
**************/
UIManager* UIManager::GetInstance() {
	if (instance == NULL) instance = new UIManager();
	return instance;
}

/**************
* Purpose: �����Ϳ� ��ȭ�� �ִٸ� ���� �������� �ִ�â�� ���ΰ�ħ �ϱ�
**************/
void UIManager::refreshUI() {
	//����_�չο�_2018-02-26

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

			pClass2->Invalidate(false); //����_���Ͽ�_2018-04-03
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
* Purpose: ��ƼƼ �����Ϳ� ���� ������ �κ��� �߻��Ͽ��ٰ� ����
**************/
void UIManager::updateEntityUI()
{
	flagUpdateEntity = true;

}
/**************
* Purpose: ������ �����Ϳ� ���� ������ �κ��� �߻��Ͽ��ٰ� ����
**************/
void UIManager::updateListenerEventUI()
{
	flagUpdateEvent = true;
}



