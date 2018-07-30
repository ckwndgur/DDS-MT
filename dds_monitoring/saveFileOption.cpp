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
* Purpose: �̱��� ���ν��Ͻ�
**************/
SaveFileOption* SaveFileOption::GetInstance() {
	if (instance == NULL) instance = new SaveFileOption();
	return instance;
}



/**************
* Purpose: guid�� �ش��ϴ� �� ����
**************/
void SaveFileOption::optionFree(int id)
{
	saveOption.remove(id);

	DataManager::GetInstance()->setSaveFileOption(id, 0);//0�̸� ���� 1�̸� �߰�
}





/**************
* Purpose: �����ص� �Ǹ� 1 ���� �ȵǸ� 0 �ݳ� ... input guid
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
* Purpose: �������� ���� guid �߰�
**************/
void SaveFileOption::setSaveOption(int id)
{
	saveOption.push_back(id);

	DataManager::GetInstance()->setSaveFileOption(id, 1);//0�̸� ���� 1�̸� �߰�

}




/**************
* Purpose: �ʱ�ȭ
**************/
void SaveFileOption::initial() {

	DataManager::GetInstance()->getSaveFileOption(saveOption);
}