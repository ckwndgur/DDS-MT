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
* Purpose: ���νý��� �������
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
	bool bSelDomain; //���� ���� 2018.03.19
#ifndef _INDDS
	CString sel_domainid; //���� ���� 2018.03.19
#endif
	unordered_map<int, listener_event> currentDataSet;
	unordered_map<int, entity> currentEntitySet;
	unordered_map<int, entity> qosEntitySet;



	//����_�չο�_2018-02-26
	double *FrequencyByHour( ); //�ð��� ���� Ƚ�� ���ϱ�
	double *LastTransmissionInterval(); //�ֱټۼ��Ű��ݱ��ϱ�
	time_t getEpochTime(const string& dateTime);
	int getEntity(string guid, string domain_id = "", string qos = "", string ub_id = "", string topic_name = "", string topic_type = "", string ub_qos = "", string dp_qos = "", string topic_pos = "");

	void dataFree();
	void getData();
	entity *findEntity(int i);


	string getQOS(entity e, string type); //  type���� �����Ѱ� "topic", "dp", "pub", "sub"
};

#endif