#pragma once
#ifndef INCLUDEONCE_H
#define INCLUDEONCE_H
#include <map>
#include <unordered_map>
#endif

/**************
* Purpose: 유저인터페이스 관리 헤더파일
**************/

#ifndef UI_H
#define UI_H


#include "dataManager.h"
#include "mainSystem.h"

//#include "uiManager.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "dds_monitoring.h"
#include "dds_monitoringDoc.h"
#include "dds_monitoringView.h"
#include "ClassView.h"

class UIManager
{
private:
	/* 각종 뷰를 알고 있어야 함*/
	UIManager();
	UIManager(const UIManager& sfo);
	~UIManager();
	static UIManager* instance;
	bool flagUpdateEntity = false;
	bool flagUpdateEvent = false;
	//Cdds_monitoringView* GetMonitoringView();

public:
	

	static UIManager* GetInstance();
	void updateEntityUI();
	void updateListenerEventUI();
	void refreshUI();
};

#endif