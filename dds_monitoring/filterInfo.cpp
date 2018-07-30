#include "stdafx.h"
#include "filterInfo.h"

FilterInfo::FilterInfo()
{
}

FilterInfo::FilterInfo(const FilterInfo& sfo)
{
}

FilterInfo::~FilterInfo()
{
}

FilterInfo* FilterInfo::instance = nullptr;



/**************
* Purpose:  �̱��� �ν��Ͻ� �޾ƿ���
**************/
FilterInfo* FilterInfo::GetInstance() {
	if (instance == NULL) instance = new FilterInfo();
	return instance;
}


/**************
* Purpose: ���� �ð��� string���� ��ȯ
**************/
const std::string currentDateTime(int type = 0) {
	time_t     now = time(0); //���� �ð��� time_t Ÿ������ ����
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	if (type == 1) {
		if (tstruct.tm_mon == 0) {
			tstruct.tm_year -= 1;
			tstruct.tm_mon = 11;
		}
		else {
			if (tstruct.tm_mday > 28) {
				if (tstruct.tm_mon == 2) {
					tstruct.tm_mday = 28;
				}
				else if (tstruct.tm_mon != 7 && tstruct.tm_mday == 31) {
					tstruct.tm_mday = 30;
				}
			}
			tstruct.tm_mon -= 1;
		}
	}

	strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct); // YYYY-MM-DD.HH:mm:ss ������ ��Ʈ��


	return buf;
}


/**************
* Purpose: ����� ��¥���� �޾ƿ���
**************/
string FilterInfo::getDate()
{
	string  tmp = this->sDate + "~" + this->eDate;
	return tmp;
}


/**************
* Purpose: ���۳�¥ �޾ƿ���
**************/
string FilterInfo::getSdate()
{
	return this->sDate;
}

/**************
* Purpose:���� ��¥ �޾ƿ���
**************/
string FilterInfo::getEdate()
{
	return this->eDate;
}



/**************
* Purpose: ���� ��¥ ����
**************/
void FilterInfo::changeDate(string sDate_new, string eDate_new)
{
	//����_�չο�_02_28
	sDate = sDate_new;
	eDate = eDate_new;

	tm tm1, tm2;

	istringstream str_stream(sDate_new);
	str_stream >> std::get_time(&tm1, "%Y-%m-%d");
	tm1.tm_sec = tm1.tm_hour = tm1.tm_min = 1;
	tm1.tm_isdst = -1;

	istringstream str_stream2(eDate_new);
	str_stream2 >> std::get_time(&tm2, "%Y-%m-%d");
	tm2.tm_sec = tm2.tm_min = 59; tm2.tm_hour = 23;
	tm2.tm_isdst = -1;


	t_sDate = mktime(&tm1);
	t_eDate = mktime(&tm2);
}



/**************
* Purpose: �⺻ ���� ����
**************/
void FilterInfo::setDefault()
{
	sDate= currentDateTime(1); //�Ѵ���
	eDate = currentDateTime();
	changeDate(sDate, eDate);
	if (currentFilterData.size() == 0) {
		count_save = 0;
		count_view_e = 0;
		count_view_le = 0;
	}
	DataManager::GetInstance()->getFilterOption();
}



/**************
* Purpose: �̺�Ʈ�� ���� ���� ����
**************/
void FilterInfo::setViewOption(listener_event le)
{
	filterData *f = checkEvent(le);
	if (f == NULL)
	DataManager::GetInstance()->setFilterOption(&le, NULL, -1, false/*view��*/, false /*�߰�*/); 
}


/**************
* Purpose: ��ƼƼ�� ���� ���� ����
**************/
void FilterInfo::setViewOption(entity e)
{
	filterData *f = checkEntity(e, 0); //flag 1�̸� �����ã��, 0�̸� view�� ã��
	if (f == NULL) 
	DataManager::GetInstance()->setFilterOption(NULL, &e, -1, false/*view��*/, false /*�߰�*/);
}


/**************
* Purpose: �̺�Ʈ�� �������� ���� 
**************/
void FilterInfo::setViewOptionDelete(listener_event le)
{
	unordered_map<int, filterData>::iterator i;
	filterData *f = checkEvent(le);
	
	if (f != NULL) {
		DataManager::GetInstance()->setFilterOption(NULL, NULL, f->id, false/*view��*/, true /*����*/);
		currentFilterData.erase(f->id);
		count_view_le--;
	}
}


/**************
* Purpose: ��ƼƼ�� �������� ����
**************/
void FilterInfo::setViewOptionDelete(entity e)
{
	unordered_map<int, filterData>::iterator i;
	filterData *f = checkEntity(e, 0); //flag 1�̸� �����ã��, 0�̸� view�� ã��

	if (f != NULL) {
		int tt = f->id;
		currentFilterData.erase(tt);
		DataManager::GetInstance()->setFilterOption(NULL, NULL, tt, false/*view��*/, true /*����*/);
		count_view_e--;
	}

}

/**************
* Purpose: ���� ���� Ȯ�� �Լ� ��ȯ�� true �� ui �� �迭 ����
**************/
bool FilterInfo::checkViewOption(listener_event le)
{
	//����_�չο�_02_28
	bool saveflag = 0;
	bool entity_flag = 1;
	bool dateflag = 1;
	if (this->checkSaveOption(le.id)&&this->checkSaveOption(le.wid)) {
		saveflag = 1;
	}

	entity *e = MainSystem::GetInstance()->findEntity(le.id);

	if ((MainSystem::GetInstance()->sel_domainid).Compare((CString)(e->domainId.c_str())) == 0) {
		filterData *f = checkEntity(*e, false); //flag 1�̸� �����ã��, 0�̸� view�� ã��
		if (f != NULL) {
			entity_flag = 0;
		}
	}
	else {
		entity_flag = 0;
	}
	if (le.wid) {
		e = MainSystem::GetInstance()->findEntity(le.wid);
			filterData *f = NULL;
			if (e != NULL)	f = checkEntity(*e, false); //flag 1�̸� �����ã��, 0�̸� view�� ã��
			if (f != NULL) {
				entity_flag = 0;
			}
	}
	/*
	f = NULL;
	f = checkEvent(le);
	if (f != NULL) filterflag = 1;
	*/

	entity_flag = entity_flag;
		tm tm3;
		time_t t3;


		istringstream str_stream(le.datetime.substr(0, 10));
		str_stream >> std::get_time(&tm3, "%Y-%m-%d");
		tm3.tm_sec = tm3.tm_hour = tm3.tm_min = 2;
		tm3.tm_isdst = -1;


		t3 = mktime(&tm3);

		if (t_sDate> t3) {

			dateflag = 0;
		}
		if (t_eDate<t3) {

			dateflag = 0;
		}


	return dateflag&&entity_flag&&saveflag; //true�� �����ֱ�
}


/**************
* Purpose: ��ƼƼ �˻�(���ο�)
**************/
filterData *FilterInfo::checkEntity(entity e, bool flag) { //flag 1�̸� �����ã��, 0�̸� view�� ã��

	filterData *f = NULL;
	unordered_map<int, filterData>::iterator i;
	for (i = currentFilterData.begin(); i != currentFilterData.end(); i++) {
		if (i->second.forSave&&flag || i->second.forView && !flag) {
			if (i->second.e.process != "" && e.process == i->second.e.process) {
				if (i->second.e.domainParticipant != "") {
					if (e.domainParticipant == i->second.e.domainParticipant) {
								if (i->second.e.dataWriter != "") {
									if (e.dataWriter == i->second.e.dataWriter) {
										f = &i->second;
									}else if (e.dataReader == i->second.e.dataWriter) {
										f = &i->second;
									}
								}
								else {
									f = &i->second;
								}
					}
				}
				else {
					f = &i->second;
				}
			}
			//���Ȱ˻�
			if (i->second.e.topicName != "" && e.topicName == i->second.e.topicName) {
				f = &i->second;
			}
		}
	}
	return f;
}
/**************
* Purpose: �̺�Ʈ �˻�(���ο�)
**************/
filterData *FilterInfo::checkEvent(listener_event le) { // view�� ã��

	filterData *f = NULL;
	unordered_map<int, filterData>::iterator i;

	for (i = currentFilterData.begin(); i != currentFilterData.end(); i++) {
		if (i->second.forView ) {
			if (i->second.le.state != "" && le.state == i->second.le.state) {
				if (i->second.le.status != "") {
					if (le.status == i->second.le.status) {
							*f = i->second;
					}
				}
				else {
					*f = i->second;
				}
			}
		}
	}
	return f;
}


/**************
* Purpose: �����ص� �Ǹ� 1 ���� �ȵǸ� 0 �ݳ� 
**************/
bool FilterInfo::checkSaveOption(int id)
{
	if (id) {
		entity *e = MainSystem::GetInstance()->findEntity(id);
		if (e != NULL) {
			filterData *f = checkEntity(*e, true); //flag 1�̸� �����ã��, 0�̸� view�� ã��
			if (f == NULL) //��ϵ� ���� ���ٴ� ��
				return true;
			else
				return false;

		}
		else return true; 
	}
	else
		return false;
}





/**************
* Purpose: �������� ���� guid �߰�
**************/
void FilterInfo::setSaveOption(entity e)
{
	//saveOption.push_back(id);
	filterData *f = checkEntity(e, true); //flag 1�̸� �����ã��, 0�̸� view�� ã��
	if (f == NULL)
	DataManager::GetInstance()->setFilterOption(NULL, &e, -1, true/*save��*/, false /*�߰�*/); //setSaveFileOption(e);//0�̸� ���� 1�̸� �߰�

}



/**************
* Purpose: guid�� �ش��ϴ� �� �������Ϳ��� ����
**************/
void FilterInfo::setSaveOptionDelete(entity e)
{
	//saveOption.remove(id);

	unordered_map<int, filterData>::iterator i;
	filterData *f = checkEntity(e, true); //flag 1�̸� �����ã��, 0�̸� view�� ã��

	if (f != NULL) {
		DataManager::GetInstance()->setFilterOption(NULL, NULL, f->id, true/*save��*/, true /*����*/);
		currentFilterData.erase(f->id);
		count_save--;
	}

}


