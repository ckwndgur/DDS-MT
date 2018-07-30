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
* Purpose: 싱글턴 인스턴스 반환함수
**************/
FilterInfo* FilterInfo::GetInstance() {
	if (instance == NULL) instance = new FilterInfo();
	return instance;
}


/**************
* Purpose: 필터 확인 함수 반환이 true 면 ui 와 배열 갱신
**************/
bool FilterInfo::checkFilter(listener_event le)
{
	return true; //현재 true만 반환
}



/**************
* Purpose: UI 버튼으로 부터 필터 정보 변경
**************/
bool FilterInfo::changeFilter(int btnId)
{
	return true;
}



/**************
* Purpose: 필터 날짜 변경
**************/
void FilterInfo::changeDate(string sDate_new, string eDate_new)
{

}



/**************
* Purpose: 기본 필터 적용
**************/
void FilterInfo::setDefault()
{

}


