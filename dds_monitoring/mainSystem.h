#pragma once

#ifndef INCLUDEONCE_H
#define INCLUDEONCE_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#endif
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

#include <unordered_map>

/**************
* Purpose: 메인시스템 헤더파일
**************/

#ifndef MS_H
#define MS_H


#include "dataManager.h"
#include "filterInfo.h"
//#include "mainSystem.h"

#include "uiManager.h"

class MainSystem
{
private:

	MainSystem();
	MainSystem(const MainSystem& sfo);
	~MainSystem();
	static MainSystem* instance;


public:
	static MainSystem* GetInstance();
	int viwflag;
	bool bSelDomain; //수정 중혁 2018.03.19
#ifndef _INDDS
	CString sel_domainid; //수정 중혁 2018.03.19
#endif
	unordered_map<int, listener_event> currentDataSet;
	unordered_map<int, entity> currentEntitySet;
	unordered_map<int, entity> qosEntitySet;



	//수정_손민영_2018-02-26
	double *FrequencyByHour( ); //시간당 누적 횟수 구하기
	double *LastTransmissionInterval(); //최근송수신간격구하기
	time_t getEpochTime(const string& dateTime);
	int getEntity(string guid, string domain_id = "", string qos = "", string ub_id = "", string topic_name = "", string topic_type = "", string ub_qos = "", string dp_qos = "", string topic_pos = "");

	void dataFree();
	void getData();
	entity *findEntity(int i);


	string getQOS(entity e, string type); //  type으로 가능한것 "topic", "dp", "pub", "sub"
};

#endif