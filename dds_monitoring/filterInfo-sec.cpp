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
* Purpose: �̱��� �ν��Ͻ� ��ȯ�Լ�
**************/
FilterInfo* FilterInfo::GetInstance() {
	if (instance == NULL) instance = new FilterInfo();
	return instance;
}


/**************
* Purpose: ���� Ȯ�� �Լ� ��ȯ�� true �� ui �� �迭 ����
**************/
bool FilterInfo::checkFilter(listener_event le)
{
	return true; //���� true�� ��ȯ
}



/**************
* Purpose: UI ��ư���� ���� ���� ���� ����
**************/
bool FilterInfo::changeFilter(int btnId)
{
	return true;
}



/**************
* Purpose: ���� ��¥ ����
**************/
void FilterInfo::changeDate(string sDate_new, string eDate_new)
{

}



/**************
* Purpose: �⺻ ���� ����
**************/
void FilterInfo::setDefault()
{

}


