#pragma once

using namespace std;

class MessageData
{
public:
	MessageData();
	~MessageData();

	//Attributes
public:
	int Id;
	int WriterId;
	CString csState;
	CString csStatus;
	CString csValue;

};

