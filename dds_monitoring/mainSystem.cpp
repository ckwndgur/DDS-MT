#include "stdafx.h"
#include "mainSystem.h"

MainSystem::MainSystem()
{
	bSelDomain = false; //수정 중혁 2018.03.26
}

MainSystem::MainSystem(const MainSystem& sfo)
{
}

MainSystem::~MainSystem()
{
}

MainSystem* MainSystem::instance = nullptr;



/**************
* Purpose:  싱글턴 인스턴스 받아오기
**************/
MainSystem* MainSystem::GetInstance() {
	if (instance == NULL) instance = new MainSystem();
	return instance;
}




/**************
* Purpose:  그래프를 위해서 데이터 생성 cplot용
**************/
double *MainSystem::FrequencyByHour() {//string eno, string sdate, string edate

									   /*
	string sdate = FilterInfo::GetInstance()->getSdate();// "2018-02-26";//날짜변경
	string edate = FilterInfo::GetInstance()->getEdate();//"2018-02-26";//날짜변경
	*/
	//수정_손민영_02_28
	double *tmp = new double[24];
	int j = 0;
	for (j = 0; j < 24; j++) {
		tmp[j] = 0;
	}
	/*
	if (eno == "")eno = ",";
	auto v = DataManager::GetInstance()->explode(eno, ',');

	

	tm tm1, tm2, tm3;
	time_t t1, t2, t3;

	istringstream str_stream(sdate);
	str_stream >> std::get_time(&tm1, "%Y-%m-%d");
	tm1.tm_sec = tm1.tm_hour = tm1.tm_min = 1;
	tm1.tm_isdst = -1;

	istringstream str_stream2(edate);
	str_stream2 >> std::get_time(&tm2, "%Y-%m-%d");
	tm2.tm_sec = tm2.tm_min = 59; tm2.tm_hour = 23;
	tm2.tm_isdst = -1;


	t1 = mktime(&tm1);
	t2 = mktime(&tm2);
	*/
	unordered_map<int, listener_event>::iterator i;
	for (i = this->currentDataSet.begin(); i != this->currentDataSet.end(); i++) {
		/*
		istringstream str_stream3((i->second.datetime.substr(0, 10)));
		str_stream3 >> std::get_time(&tm3, "%Y-%m-%d");
		tm3.tm_sec = tm3.tm_hour = tm3.tm_min = 2;
		tm3.tm_isdst = -1;


		t3 = mktime(&tm3);
		*/
		if (FilterInfo::GetInstance()->checkViewOption(i->second)){ //t1<= t3 && t2>= t3) {//&&
			/*
			bool run_flag = 0;
			if (eno == ",") { run_flag = 1; }
			else {
				for (j = 0; j < v.size(); j++) {
					if (i->second.id == atoi(v[j].c_str())) { run_flag = 1; }
				}
			}
			if (run_flag) {
			}*/
				int tmp_hour = atoi(i->second.datetime.substr(11, 2).c_str());
				if (tmp_hour >= 0 && tmp_hour < 25) {
					tmp[tmp_hour % 24]++;
				}
		}
	}

	return tmp;
}


/**************
* Purpose:  그래프를 위해서 데이터 생성 cplot용
**************/
double *MainSystem::LastTransmissionInterval(){//string eno) {
	//수정_손민영_2018-02-26
	double *tmp = new double[100];
	int j = 0;

	unordered_map<int, listener_event>::iterator i;

	for (i = this->currentDataSet.begin(); i != this->currentDataSet.end()&&j<100; i++) {
		//pView->m_lstListenerData.push_back(i->second);
		/*
		bool run_flag = 0;
		if (eno == ",") { run_flag = 1; }
		else {
			int k;
			for (k = 0; k < v.size(); k++) {
				if (i->second.id == atoi(v[k].c_str())) { run_flag = 1; }
			}
		}*/

		if (FilterInfo::GetInstance()->checkViewOption(i->second)) {//if (run_flag) {
			if (i->second.status == "data_available" && i->second.datetime2 != "") {
				tmp[j] = 0;
				time_t t1 = getEpochTime(i->second.datetime);
				time_t t2 = getEpochTime(i->second.datetime2);


				tmp[j] = difftime(t1, t2) * 1000000000;
				tmp[j] += stod(i->second.data[1]);
				tmp[j] -= stod(i->second.data[2]);
				tmp[j] /= 1000000000;
				j++;

			}
		}
	}
	for (; j < 100; j++) {
		tmp[j] = 0;
	}

	return tmp;
}



/**************
* Purpose:  시간데이터 string->time_t 변환
**************/
time_t MainSystem::getEpochTime(const string& dateTime)
{
	// this format: '2014-07-25 20:17:22Z' 
	static const string dateTimeFormat{ "%Y-%m-%d %H:%M:%S" };
	istringstream ss{ dateTime };
	tm dt;
	ss >> get_time(&dt, dateTimeFormat.c_str());
	return mktime(&dt);
}



/**************
* Purpose: 현재 보유한 엔티티와 데이터 정보초기화
**************/
void MainSystem::dataFree()
{
	currentDataSet.clear();
	currentEntitySet.clear();
}





/**************
* Purpose: 엔티티 관리 /  엔티티 번호 반납 (새로운 엔티티가 있을 경우 등록 및 갱신)
**************/
int MainSystem::getEntity(string guid, string domain_id, string qos, string ub_id, string topic_name, string topic_type, string ub_qos , string dp_qos , string topic_pos )
{
	unordered_map<int, entity>::iterator i;
	int find_id = 0;
	bool need_update_flag = false;

	for (i = currentEntitySet.begin(); i != currentEntitySet.end(); i++) {
		if (i->second.guid == guid) {
			find_id = i->first;
			if (domain_id != "" && i->second.domainId != domain_id) { need_update_flag = true; }
			if (qos != "" && i->second.qos != qos) { need_update_flag = true; }
			if (ub_id != "" && (i->second.publisher != ub_id || i->second.subscriber != ub_id)) { need_update_flag = true; }
			if (topic_name != "" && i->second.topicName != topic_name) { need_update_flag = true; }
			if (topic_type != "" && i->second.topicType != topic_type) { need_update_flag = true; }
		}
	}


	if (need_update_flag) {
		//엔티티 정보 업데이트 코드 추가 
		DataManager::GetInstance()->saveEntity(guid, domain_id, qos, ub_id, topic_name, topic_type,  ub_qos,  dp_qos,  topic_pos); //db저장
																									 //엔티티 정보 갱신
		currentEntitySet.erase(find_id); //엔티티 삭제
		if (guid.substr(30, 2) == "02" || guid.substr(30, 2) == "03" || guid.substr(30, 2) == "c2" || guid.substr(30, 2) == "c3") { //writer 타입
			entity new_entity = { find_id, guid, domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8), topic_name, topic_type, "", ub_id, "", guid.substr(24, 6), qos };
			currentEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));			
			UIManager::GetInstance()->updateEntityUI(); //UI 화면 새로고침
		}
		else if (guid.substr(30, 2) == "04" || guid.substr(30, 2) == "07" || guid.substr(30, 2) == "c4" || guid.substr(30, 2) == "c7") { //reader 타입
			entity new_entity = { find_id, guid, domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8), topic_name, topic_type, ub_id, "", guid.substr(24, 6), "", qos };
			currentEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));
			UIManager::GetInstance()->updateEntityUI(); //UI 화면 새로고침
		}
		else {
			//cout << "엔티티의 타입이 입력되지 않았습니다.\n";
		}
	}
	if (find_id == 0) {
		//엔티티 정보 삽입 코드 추가
		find_id = DataManager::GetInstance()->saveEntity(guid, domain_id, qos, ub_id, topic_name, topic_type, ub_qos, dp_qos, topic_pos); //db저장
																											   //cout << "find_id" << find_id << endl;
		if (guid.substr(30, 2) == "02" || guid.substr(30, 2) == "03" || guid.substr(30, 2) == "c2" || guid.substr(30, 2) == "c3") { //writer 타입
			entity new_entity = { find_id, guid, domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8), topic_name, topic_type, "", ub_id, "", guid.substr(24, 6), qos };
			currentEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));
			UIManager::GetInstance()->updateEntityUI(); //UI 화면 새로고침
		}
		else if (guid.substr(30, 2) == "04" || guid.substr(30, 2) == "07" || guid.substr(30, 2) == "c4" || guid.substr(30, 2) == "c7") { //reader 타입
			entity new_entity = { find_id, guid, domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8),topic_name, topic_type, ub_id, "", guid.substr(24, 6), "", qos };
			currentEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));
			UIManager::GetInstance()->updateEntityUI(); //UI 화면 새로고침
		}
		else {
			//cout << "엔티티의 타입이 입력되지 않았습니다.\n";
		}
	}
	// ui 쪽에 업데이트 된 엔티티 전달??
	return find_id;

}






/**************
* Purpose: 현재 저장하고 있는 배열에서 번호에 해당하는 entity를 찾아 반환
**************/
entity *MainSystem::findEntity(int i) {

	unordered_map<int, entity>::iterator i2;
	int find_id = 0;
	bool need_update_flag = false;

	for (i2 = currentEntitySet.begin(); i2 != currentEntitySet.end(); i2++) {
		if (i2->second.id == i) {
			return &(i2->second);
		}
	}
	return NULL;
}






/**************
* Purpose: 기본 데이터 가져오기
**************/
void MainSystem::getData()
{
	//수정_손민영_2018-02-26
	//dataFree();
	bool flag=DataManager::GetInstance()->getData(currentDataSet, currentEntitySet);
	if (flag) {
		UIManager::GetInstance()->updateEntityUI();
		UIManager::GetInstance()->updateListenerEventUI(); //UI 화면 새로고침
	}
}




/**************
* Purpose: 싱글턴 인스턴스 반환
**************/
string MainSystem::getQOS(entity e, string type) {
// type으로 가능한것 "topic", "dp", "pub", "sub"

	unordered_map<int, entity>::iterator j;

	for (j = qosEntitySet.begin(); j != qosEntitySet.end(); j++) {
		if (type == "topic") {
			if (j->second.topicName == e.topicName) {
				if(j->second.domainId==e.domainId&&j->second.host == e.host&&j->second.process == e.process&&j->second.domainParticipant == e.domainParticipant)
				return j->second.qos;
			}
		}else if(type == "dp") {
			if (j->second.topicName == "" &&  j->second.publisher == ""&& j->second.subscriber == "") {
				if (j->second.domainId == e.domainId&&j->second.host == e.host&&j->second.process == e.process&&j->second.domainParticipant == e.domainParticipant)
				return j->second.qos;
			}
		}
		else {
			if (j->second.publisher == e.publisher || j->second.subscriber ==e.subscriber) {
				if (j->second.domainId == e.domainId&&j->second.host == e.host&&j->second.process == e.process&&j->second.domainParticipant == e.domainParticipant)
				return j->second.qos;
			}
		}
	}


	return "";
}
