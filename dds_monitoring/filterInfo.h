#pragma once
#ifndef INCLUDEONCE_H
#define INCLUDEONCE_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#endif
using namespace std;
/**************
* Purpose: 필터정보 헤더파일
**************/

#ifndef FI_H
#define FI_H


#include "dataManager.h"
//#include "filterInfo.h"
#include "mainSystem.h"

#include "uiManager.h"


struct filterData {
	int id;
	entity e;
	listener_event le;
	bool forSave;
	bool forView;
};

class FilterInfo
{
	//수정_손민영_02_28
private:


	string sDate;
	string eDate;
	time_t t_sDate;
	time_t t_eDate;



	FilterInfo();
	FilterInfo(const FilterInfo& sfo);
	~FilterInfo();
	static FilterInfo* instance;

public:
	int count_view_e;
	int count_view_le;
	int count_save;
	unordered_map<int, filterData> currentFilterData;
	static FilterInfo* GetInstance();
	void setDefault();

	void setViewOptionDelete(listener_event le);
	void setViewOption(listener_event le);
	bool checkViewOption(listener_event le);

	void setViewOptionDelete(entity e);
	void setViewOption(entity e);

	void changeDate(string sDate_new, string eDate_new);
	string getSdate();
	string getEdate();
	string getDate();



	filterData *checkEntity(entity e, bool flag);
	filterData *checkEvent(listener_event le);
	void setSaveOptionDelete(entity e);
	void setSaveOption(entity e);
	bool checkSaveOption(int id);
};


#endif