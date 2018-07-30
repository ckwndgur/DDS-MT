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
* Purpose:  싱글턴 인스턴스 받아오기
**************/
FilterInfo* FilterInfo::GetInstance() {
	if (instance == NULL) instance = new FilterInfo();
	return instance;
}


/**************
* Purpose: 현재 시간을 string으로 반환
**************/
const std::string currentDateTime(int type = 0) {
	time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
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

	strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 스트링


	return buf;
}


/**************
* Purpose: 저장된 날짜구간 받아오기
**************/
string FilterInfo::getDate()
{
	string  tmp = this->sDate + "~" + this->eDate;
	return tmp;
}


/**************
* Purpose: 시작날짜 받아오기
**************/
string FilterInfo::getSdate()
{
	return this->sDate;
}

/**************
* Purpose:종료 날짜 받아오기
**************/
string FilterInfo::getEdate()
{
	return this->eDate;
}



/**************
* Purpose: 필터 날짜 변경
**************/
void FilterInfo::changeDate(string sDate_new, string eDate_new)
{
	//수정_손민영_02_28
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
* Purpose: 기본 필터 적용
**************/
void FilterInfo::setDefault()
{
	sDate= currentDateTime(1); //한달전
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
* Purpose: 이벤트의 보기 필터 저장
**************/
void FilterInfo::setViewOption(listener_event le)
{
	filterData *f = checkEvent(le);
	if (f == NULL)
	DataManager::GetInstance()->setFilterOption(&le, NULL, -1, false/*view용*/, false /*추가*/); 
}


/**************
* Purpose: 엔티티의 보기 필터 저장
**************/
void FilterInfo::setViewOption(entity e)
{
	filterData *f = checkEntity(e, 0); //flag 1이면 저장용찾기, 0이면 view용 찾기
	if (f == NULL) 
	DataManager::GetInstance()->setFilterOption(NULL, &e, -1, false/*view용*/, false /*추가*/);
}


/**************
* Purpose: 이벤트의 보기필터 삭제 
**************/
void FilterInfo::setViewOptionDelete(listener_event le)
{
	unordered_map<int, filterData>::iterator i;
	filterData *f = checkEvent(le);
	
	if (f != NULL) {
		DataManager::GetInstance()->setFilterOption(NULL, NULL, f->id, false/*view용*/, true /*삭제*/);
		currentFilterData.erase(f->id);
		count_view_le--;
	}
}


/**************
* Purpose: 엔티티의 보기필터 삭제
**************/
void FilterInfo::setViewOptionDelete(entity e)
{
	unordered_map<int, filterData>::iterator i;
	filterData *f = checkEntity(e, 0); //flag 1이면 저장용찾기, 0이면 view용 찾기

	if (f != NULL) {
		int tt = f->id;
		currentFilterData.erase(tt);
		DataManager::GetInstance()->setFilterOption(NULL, NULL, tt, false/*view용*/, true /*삭제*/);
		count_view_e--;
	}

}

/**************
* Purpose: 보기 필터 확인 함수 반환이 true 면 ui 와 배열 갱신
**************/
bool FilterInfo::checkViewOption(listener_event le)
{
	//수정_손민영_02_28
	bool saveflag = 0;
	bool entity_flag = 1;
	bool dateflag = 1;
	if (this->checkSaveOption(le.id)&&this->checkSaveOption(le.wid)) {
		saveflag = 1;
	}

	entity *e = MainSystem::GetInstance()->findEntity(le.id);

	if ((MainSystem::GetInstance()->sel_domainid).Compare((CString)(e->domainId.c_str())) == 0) {
		filterData *f = checkEntity(*e, false); //flag 1이면 저장용찾기, 0이면 view용 찾기
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
			if (e != NULL)	f = checkEntity(*e, false); //flag 1이면 저장용찾기, 0이면 view용 찾기
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


	return dateflag&&entity_flag&&saveflag; //true면 보여주기
}


/**************
* Purpose: 엔티티 검사(내부용)
**************/
filterData *FilterInfo::checkEntity(entity e, bool flag) { //flag 1이면 저장용찾기, 0이면 view용 찾기

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
			//토픽검사
			if (i->second.e.topicName != "" && e.topicName == i->second.e.topicName) {
				f = &i->second;
			}
		}
	}
	return f;
}
/**************
* Purpose: 이벤트 검사(내부용)
**************/
filterData *FilterInfo::checkEvent(listener_event le) { // view용 찾기

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
* Purpose: 저장해도 되면 1 저장 안되면 0 반납 
**************/
bool FilterInfo::checkSaveOption(int id)
{
	if (id) {
		entity *e = MainSystem::GetInstance()->findEntity(id);
		if (e != NULL) {
			filterData *f = checkEntity(*e, true); //flag 1이면 저장용찾기, 0이면 view용 찾기
			if (f == NULL) //등록된 것이 없다는 뜻
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
* Purpose: 저장하지 않을 guid 추가
**************/
void FilterInfo::setSaveOption(entity e)
{
	//saveOption.push_back(id);
	filterData *f = checkEntity(e, true); //flag 1이면 저장용찾기, 0이면 view용 찾기
	if (f == NULL)
	DataManager::GetInstance()->setFilterOption(NULL, &e, -1, true/*save용*/, false /*추가*/); //setSaveFileOption(e);//0이면 삭제 1이면 추가

}



/**************
* Purpose: guid에 해당하는 값 저장필터에서 삭제
**************/
void FilterInfo::setSaveOptionDelete(entity e)
{
	//saveOption.remove(id);

	unordered_map<int, filterData>::iterator i;
	filterData *f = checkEntity(e, true); //flag 1이면 저장용찾기, 0이면 view용 찾기

	if (f != NULL) {
		DataManager::GetInstance()->setFilterOption(NULL, NULL, f->id, true/*save용*/, true /*삭제*/);
		currentFilterData.erase(f->id);
		count_save--;
	}

}


