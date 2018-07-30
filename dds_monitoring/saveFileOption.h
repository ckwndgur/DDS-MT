#pragma once

#ifndef INCLUDEONCE_H
#define INCLUDEONCE_H
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <unordered_map>
#endif
using namespace std;

/**************
* Purpose: 파일저장정보 헤더파일
**************/

#ifndef SFO_H
#define SFO_H


#include "DataManager.h"
//#include "saveFileOption.h"
#include "filterInfo.h"
#include "mainSystem.h"

#include "uiManager.h"


class SaveFileOption
{
private:
	list<int> saveOption;
	SaveFileOption();
	SaveFileOption(const SaveFileOption& sfo);
	~SaveFileOption();
	static SaveFileOption* instance;

public:
	static SaveFileOption* GetInstance();
	bool checkSaveOption(int id);
	void setSaveOption(int id);
	void optionFree(int id);
	void initial();

};

#endif

