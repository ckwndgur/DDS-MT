#pragma once
#ifndef INCLUDEONCE_H
#define INCLUDEONCE_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#endif
#include <vector>
#include <sstream>
#include <utility>
using namespace std;

/**************
* Purpose: 데이터베이스 헤더파일
**************/
#ifndef DM_H
#define DM_H

struct entity {
	int id;
	string guid;
	string domainId;
	string host;
	string process;
	string domainParticipant;
	string topicName;
	string topicType;
	string subscriber;
	string publisher;
	string dataReader;
	string dataWriter;
	string qos;

};


struct listener_event {
	int id;
	int wid;
	string table_name;
	string datetime;
	string datetime2;
	string state;
	string status;
	string value;
	string val[20];
	string data[20];
	bool visible = false;
};
#ifndef _INDDS

struct position
{
	int index;
	entity entity;
	string name;
	CSize x;
	CSize y;
	bool bclick = false;
};
#endif



//#include "dataManager.h"
#include "filterInfo.h"
#include "mainSystem.h"

#include "uiManager.h"

#include <my_global.h>
#include <mysql.h>
#pragma comment(lib,"libmySQL.lib")

/**************
* Database Error Definitions
**************/
#define SUCCESS 200
#define ERR201 201
#define ERR202 202
#define ERR203 203

/**************
* 데이터베이스 접속정보
**************/
#define MAX_QUERY_LEN 500
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "1234"
#define DB_NAME "dds_ms"

class DataManager
{


private:
	DataManager(const DataManager& sfo);
	DataManager();
	~DataManager();

	bool query_print = false;
	MYSQL *sock;
	MYSQL conn;
	MYSQL_RES *result;
	static DataManager* instance;

	//수정_손민영_2018-02-26
	string last_entity_no = "0";
	string last_message_no = "0";

public:
	static DataManager* GetInstance();

	//connection & disconnect
	int       connect(); //connect to the database
	bool      disconnect(); //disconnect from the database

							//free the query results
	bool free();
	char *dberror(int errorcode); //return a string for this error message

								  //querying
	MYSQL_RES *query(char *query); //return result set
	string sql_fetch(char *query); //return string

								   /* DB 기본 도구 끝*/



								   // dds_모니터링 도구
	int saveData(listener_event le);

	bool getData(unordered_map<int, listener_event>& currentDataSet, unordered_map<int, entity>& currentEntitySet);
	void setFilterOption(listener_event *le, entity *e, int f, bool type, bool del);
	void getFilterOption();
	int saveEntity(string guid, string domain_id, string qos, string ub_id, string topic_name, string topic_type, string ub_qos = "", string dp_qos = "", string topic_pos = ""); // 엔티티 찾아보고 없으면 저장
	vector<string> explode(string const & s, char delim);
};


#endif