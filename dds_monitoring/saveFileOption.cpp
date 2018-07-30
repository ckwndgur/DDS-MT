#include "stdafx.h"
#include "saveFileOption.h"

SaveFileOption::SaveFileOption()
{
}

SaveFileOption::SaveFileOption(const SaveFileOption& sfo)
{
}

SaveFileOption::~SaveFileOption()
{
}



SaveFileOption* SaveFileOption::instance = nullptr;



/**************
* Purpose: 싱글턴 겟인스턴스
**************/
SaveFileOption* SaveFileOption::GetInstance() {
	if (instance == NULL) instance = new SaveFileOption();
	return instance;
}



/**************
* Purpose: guid에 해당하는 값 삭제
**************/
void SaveFileOption::optionFree(int id)
{
	saveOption.remove(id);

	DataManager::GetInstance()->setSaveFileOption(id, 0);//0이면 삭제 1이면 추가
}





/**************
* Purpose: 저장해도 되면 1 저장 안되면 0 반납 ... input guid
**************/
bool SaveFileOption::checkSaveOption(int id)
{
	list<int>::iterator  i;
	bool find = true;
	for (i = saveOption.begin(); i != saveOption.end(); i++) {
		if (*i == id) find = false;
	}
	return find;
}





/**************
* Purpose: 저장하지 않을 guid 추가
**************/
void SaveFileOption::setSaveOption(int id)
{
	saveOption.push_back(id);

	DataManager::GetInstance()->setSaveFileOption(id, 1);//0이면 삭제 1이면 추가

}




/**************
* Purpose: 초기화
**************/
void SaveFileOption::initial() {

	DataManager::GetInstance()->getSaveFileOption(saveOption);
}