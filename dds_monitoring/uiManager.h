#pragma once
#ifndef INCLUDEONCE_H
#define INCLUDEONCE_H
#include <map>
#include <unordered_map>
#endif

/**************
* Purpose: �����������̽� ���� �������
**************/

#ifndef UI_H
#define UI_H


#include "dataManager.h"
#include "mainSystem.h"

//#include "uiManager.h"
#ifndef _INDDS
#include "MainFrm.h"
#include "ChildFrm.h"
#include "dds_monitoring.h"
#include "dds_monitoringDoc.h"
#include "dds_monitoringView.h"
#include "CPlotDoc.h"
#include "CPlotView.h"
#include "ClassView.h"
#endif
class UIManager
{
private:
	/* ���� �並 �˰� �־�� ��*/
	UIManager();
	UIManager(const UIManager& sfo);
	~UIManager();
	static UIManager* instance;
	//Cdds_monitoringView* GetMonitoringView();

public:

#ifndef _INDDS
	CDocument *pGraph1Document = NULL, *pGraph2Document = NULL, *pListDocument = NULL, *pRelvDocument = NULL;
	CView *pListView = NULL, *pRelvView = NULL, *pGraph1View = NULL, *pGraph2View = NULL;
	CMFCTabCtrl *wndTab = NULL;

#endif
	bool flagUpdateEntity = false;
	//����_�չο�_2018-02-26
	bool flagUpdateEvent = false;
	//����_�չο�_2018-02-26

	static UIManager* GetInstance();
	void updateEntityUI();
	void updateListenerEventUI();
	void refreshUI();
};

#endif