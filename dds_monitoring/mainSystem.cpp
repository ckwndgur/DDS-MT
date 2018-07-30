#include "stdafx.h"
#include "mainSystem.h"

MainSystem::MainSystem()
{
	bSelDomain = false; //���� ���� 2018.03.26
}

MainSystem::MainSystem(const MainSystem& sfo)
{
}

MainSystem::~MainSystem()
{
}

MainSystem* MainSystem::instance = nullptr;



/**************
* Purpose:  �̱��� �ν��Ͻ� �޾ƿ���
**************/
MainSystem* MainSystem::GetInstance() {
	if (instance == NULL) instance = new MainSystem();
	return instance;
}




/**************
* Purpose:  �׷����� ���ؼ� ������ ���� cplot��
**************/
double *MainSystem::FrequencyByHour() {//string eno, string sdate, string edate

									   /*
	string sdate = FilterInfo::GetInstance()->getSdate();// "2018-02-26";//��¥����
	string edate = FilterInfo::GetInstance()->getEdate();//"2018-02-26";//��¥����
	*/
	//����_�չο�_02_28
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
* Purpose:  �׷����� ���ؼ� ������ ���� cplot��
**************/
double *MainSystem::LastTransmissionInterval(){//string eno) {
	//����_�չο�_2018-02-26
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
* Purpose:  �ð������� string->time_t ��ȯ
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
* Purpose: ���� ������ ��ƼƼ�� ������ �����ʱ�ȭ
**************/
void MainSystem::dataFree()
{
	currentDataSet.clear();
	currentEntitySet.clear();
}





/**************
* Purpose: ��ƼƼ ���� /  ��ƼƼ ��ȣ �ݳ� (���ο� ��ƼƼ�� ���� ��� ��� �� ����)
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
		//��ƼƼ ���� ������Ʈ �ڵ� �߰� 
		DataManager::GetInstance()->saveEntity(guid, domain_id, qos, ub_id, topic_name, topic_type,  ub_qos,  dp_qos,  topic_pos); //db����
																									 //��ƼƼ ���� ����
		currentEntitySet.erase(find_id); //��ƼƼ ����
		if (guid.substr(30, 2) == "02" || guid.substr(30, 2) == "03" || guid.substr(30, 2) == "c2" || guid.substr(30, 2) == "c3") { //writer Ÿ��
			entity new_entity = { find_id, guid, domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8), topic_name, topic_type, "", ub_id, "", guid.substr(24, 6), qos };
			currentEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));			
			UIManager::GetInstance()->updateEntityUI(); //UI ȭ�� ���ΰ�ħ
		}
		else if (guid.substr(30, 2) == "04" || guid.substr(30, 2) == "07" || guid.substr(30, 2) == "c4" || guid.substr(30, 2) == "c7") { //reader Ÿ��
			entity new_entity = { find_id, guid, domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8), topic_name, topic_type, ub_id, "", guid.substr(24, 6), "", qos };
			currentEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));
			UIManager::GetInstance()->updateEntityUI(); //UI ȭ�� ���ΰ�ħ
		}
		else {
			//cout << "��ƼƼ�� Ÿ���� �Էµ��� �ʾҽ��ϴ�.\n";
		}
	}
	if (find_id == 0) {
		//��ƼƼ ���� ���� �ڵ� �߰�
		find_id = DataManager::GetInstance()->saveEntity(guid, domain_id, qos, ub_id, topic_name, topic_type, ub_qos, dp_qos, topic_pos); //db����
																											   //cout << "find_id" << find_id << endl;
		if (guid.substr(30, 2) == "02" || guid.substr(30, 2) == "03" || guid.substr(30, 2) == "c2" || guid.substr(30, 2) == "c3") { //writer Ÿ��
			entity new_entity = { find_id, guid, domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8), topic_name, topic_type, "", ub_id, "", guid.substr(24, 6), qos };
			currentEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));
			UIManager::GetInstance()->updateEntityUI(); //UI ȭ�� ���ΰ�ħ
		}
		else if (guid.substr(30, 2) == "04" || guid.substr(30, 2) == "07" || guid.substr(30, 2) == "c4" || guid.substr(30, 2) == "c7") { //reader Ÿ��
			entity new_entity = { find_id, guid, domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8),topic_name, topic_type, ub_id, "", guid.substr(24, 6), "", qos };
			currentEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));
			UIManager::GetInstance()->updateEntityUI(); //UI ȭ�� ���ΰ�ħ
		}
		else {
			//cout << "��ƼƼ�� Ÿ���� �Էµ��� �ʾҽ��ϴ�.\n";
		}
	}
	// ui �ʿ� ������Ʈ �� ��ƼƼ ����??
	return find_id;

}






/**************
* Purpose: ���� �����ϰ� �ִ� �迭���� ��ȣ�� �ش��ϴ� entity�� ã�� ��ȯ
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
* Purpose: �⺻ ������ ��������
**************/
void MainSystem::getData()
{
	//����_�չο�_2018-02-26
	//dataFree();
	bool flag=DataManager::GetInstance()->getData(currentDataSet, currentEntitySet);
	if (flag) {
		UIManager::GetInstance()->updateEntityUI();
		UIManager::GetInstance()->updateListenerEventUI(); //UI ȭ�� ���ΰ�ħ
	}
}




/**************
* Purpose: �̱��� �ν��Ͻ� ��ȯ
**************/
string MainSystem::getQOS(entity e, string type) {
// type���� �����Ѱ� "topic", "dp", "pub", "sub"

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
