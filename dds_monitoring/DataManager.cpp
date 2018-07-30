#include "stdafx.h"
#include "DataManager.h"



DataManager::DataManager()
{
}

DataManager::DataManager(const DataManager& sfo)
{
}

DataManager::~DataManager()
{
}
DataManager* DataManager::instance = nullptr;


/**************
* Purpose:  싱글턴 인스턴스 받아오기
**************/
DataManager* DataManager::GetInstance() {
	if (instance == NULL) instance = new DataManager();
	return instance;
}


/**************
* Purpose: 데이터 저장하기 (SPY로부터 온 데이터)
**************/
int DataManager::saveData(listener_event le) {

	string tmp = "";
	if (le.table_name == "DA") {
		tmp = "INSERT INTO `da`( `dr_id`, `dw_id`, `t1_sec`, `t1_nanosec`, `t2_sec`, `t2_nanosec`, `sample_state`, `view_state`, `instance_state`,";
		tmp += "`disposed_generation_count`, `no_writers_generation_count`, `sample_rank`, `generation_rank`, `absolute_generation_rank`, `valid_data`, `data`) VALUES";
		tmp += "('" + to_string(le.id) + "','" + to_string(le.wid) + "','" + le.datetime + "','" + le.data[1] + "','" + le.datetime2 + "','" + le.data[2] + "','" + le.data[3] + "','";
		tmp += le.data[4] + "','" + le.data[5] + "','" + le.data[6] + "','" + le.data[7] + "','" + le.data[8] + "','" + le.data[9] + "','" + le.data[10] + "','" + le.data[11] + "','" + le.data[12] + "')";
	}
	else if (le.table_name == "LC") {
		tmp = "INSERT INTO `lc`( `dr_id`, `st_id`, `dr_datetime`, `alive_count`, `not_alive_count`, `alive_count_change`, `not_alive_count_change`) VALUES ";
		tmp += "('" + to_string(le.id) + "','" + to_string(le.wid) + "','" + le.datetime + "','" + le.data[1] + "','" + le.data[2] + "','" + le.data[3] + "','" + le.data[4] + "')";
	}
	else if (le.table_name == "RDM") {
		tmp = "INSERT INTO `rdm`( `dr_id`, `st_id`, `dr_datetime`, `total_count`, `total_count_change`) VALUES  ";
		tmp += "('" + to_string(le.id) + "','" + to_string(le.wid) + "','" + le.datetime + "','" + le.data[1] + "','" + le.data[2] + "')";
	}
	else if (le.table_name == "RIQOS") {
		tmp = "INSERT INTO `riqos`( `dr_id`, `dr_datetime`, `total_count`, `last_policy_id`, `st_policies`) VALUES  ";
		tmp += "('" + to_string(le.id) + "','" + le.datetime + "','" + le.data[0] + "','" + le.data[1] + "','" + le.data[2] + "')";
	}
	else if (le.table_name == "SL") {
		tmp = "INSERT INTO `sl`( `dr_id`, `dr_datetime`, `total_count`, `total_count_change`) VALUES  ";
		tmp += "('" + to_string(le.id) + "','" + le.datetime + "','" + le.data[0] + "','" + le.data[1] + "')";
	}
	else if (le.table_name == "SM") {
		tmp = "INSERT INTO `sm`( `dr_id`, `st_id`, `dr_datetime`, `total_count`, `current_count`) VALUES   ";
		tmp += "('" + to_string(le.id) + "','" + to_string(le.wid) + "','" + le.datetime + "','" + le.data[1] + "','" + le.data[2] + "')";
	}
	else if (le.table_name == "SR") {
		tmp = "INSERT INTO `sr`( `dr_id`, `st_id`, `dr_datetime`, `total_count`, `total_count_change`, `last_reason`) VALUES ";
		tmp += "('" + to_string(le.id) + "','" + to_string(le.wid) + "','" + le.datetime + "','" + le.data[1] + "','" + le.data[2] + "','" + le.data[3] + "')";
	}
	//cout << tmp << endl;

	std::vector<char> writable(tmp.begin(), tmp.end());
	writable.push_back('\0');
	char* ptr = &writable[0];
	query(ptr);
	string r = "";


	if (le.table_name == "DA") {
		r = sql_fetch("select * from `da` order by no desc");
	}
	else if (le.table_name == "LC") {
		r = sql_fetch("select * from `lc` order by no desc");
	}
	else if (le.table_name == "RDM") {
		r = sql_fetch("select * from `rdm` order by no desc");
	}
	else if (le.table_name == "RIQOS") {
		r = sql_fetch("select * from `riqos` order by no desc");
	}
	else if (le.table_name == "SL") {
		r = sql_fetch("select * from `sl` order by no desc");
	}
	else if (le.table_name == "SM") {
		r = sql_fetch("select * from `sm` order by no desc");
	}
	else if (le.table_name == "SR") {
		r = sql_fetch("select * from `sr` order by no desc");
	}


	int intr = stoi(r);
	//insert 코드
	if (le.datetime == "")le.datetime = "NULL";
	else le.datetime = "'" + le.datetime + "'";
	if (le.datetime2 == "")le.datetime2 = "NULL";
	else le.datetime2 = "'" + le.datetime2 + "'";
	tmp = "INSERT INTO `totalmessage`( `dr_id`, `dw_id`, `dr_datetime`, `dw_datetime`, `dr_state`, `dr_status`, `message`, `table_name`, `table_id`) VALUES";
	tmp += "('" + to_string(le.id) + "','" + to_string(le.wid) + "'," + le.datetime + "," + le.datetime2 + ",'" + le.state + "','";
	tmp += le.status + "','" + le.value + "','" + le.table_name + "','" + to_string(intr) + "')";
	writable.clear();
	writable.assign(tmp.begin(), tmp.end());
	writable.push_back('\0');
	ptr = &writable[0];
	query(ptr);


	r = sql_fetch("select * from `totalmessage` order by id desc");

	return intr;
}





/**************
* Purpose: 문자열을 특수문자기준으로 잘라서 vector로 저장하기
**************/
std::vector<std::string> DataManager::explode(std::string const & s, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim); )
	{
		result.push_back(std::move(token));
	}

	return result;
}


/**************
* Purpose: 데이터 불러오기 (기본)
**************/
bool DataManager::getData(unordered_map<int, listener_event>& currentDataSet, unordered_map<int, entity>& currentEntitySet) {
	//수정_손민영_2018-02-26
	string tmp = "select * from entity where id > " + last_entity_no + " order by id desc";
	int flag = 0;
	std::vector<char> writable(tmp.begin(), tmp.end());
	writable.push_back('\0');
	char* ptr = &writable[0];

	MYSQL_RES *sql_result = query(ptr);
	MYSQL_ROW sql_row;
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{

		if (atoi(last_entity_no.c_str()) < atoi(((string)sql_row[0]).c_str()))last_entity_no = (string)sql_row[0];

		if ((string)sql_row[1] == "") {
			if (sql_row[8] == NULL) {
				if (sql_row[7] == ""|| sql_row[7] == " ") {//dp qos
					int find_id = atoi(((string)sql_row[0]).c_str());

					entity new_entity = { find_id, "", sql_row[2], sql_row[3], sql_row[4], sql_row[5],  "", "", "", "", "", "",  sql_row[11] };
					MainSystem::GetInstance()->qosEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));

				}
				else {//ub qos

					int find_id = atoi(((string)sql_row[0]).c_str());

					if ((string)sql_row[7] == "02" || (string)sql_row[7] == "03" || (string)sql_row[7] == "c2" || (string)sql_row[7] == "c3") { //writer 타입
						entity new_entity = { find_id, "", sql_row[2], sql_row[3], sql_row[4], sql_row[5], "", "", "",  sql_row[10], "", "",  sql_row[11] };
						MainSystem::GetInstance()->qosEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));
					}
					else if ((string)sql_row[7] == "04" || (string)sql_row[7] == "07" || (string)sql_row[7] == "c4" || (string)sql_row[7] == "c7") { //reader 타입
						entity new_entity = { find_id, "", sql_row[2], sql_row[3], sql_row[4], sql_row[5], "", "", sql_row[10], "", "", "",  sql_row[11] };
						MainSystem::GetInstance()->qosEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));
					}


				}
			}
			else { //topic_qos

				int find_id = atoi(((string)sql_row[0]).c_str());

				entity new_entity = { find_id, "", sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[8], sql_row[9], "", "", "", "",  sql_row[11] };
				MainSystem::GetInstance()->qosEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));

			}
		}else{
			if (FilterInfo::GetInstance()->checkSaveOption(atoi(sql_row[0]))) {// 저장 옵션 활성화 된 엔티티만
				if ((string)sql_row[7] == "02" || (string)sql_row[7] == "03" || (string)sql_row[7] == "c2" || (string)sql_row[7] == "c3") { //writer 타입
					string text = sql_row[11];
					entity new_entity = { atoi(((string)sql_row[0]).c_str()),sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[8], sql_row[9], "", sql_row[10], "", sql_row[6], sql_row[11] };
					currentEntitySet.insert(unordered_map<int, entity>::value_type(atoi(sql_row[0]), new_entity));
					flag = 1;
				}
				else if ((string)sql_row[7] == "04" || (string)sql_row[7] == "07" || (string)sql_row[7] == "c4" || (string)sql_row[7] == "c7") { //reader 타입
					entity new_entity = { atoi(((string)sql_row[0]).c_str()),sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[8], sql_row[9], sql_row[10], "", sql_row[6], "", sql_row[11] };
					currentEntitySet.insert(unordered_map<int, entity>::value_type(atoi(sql_row[0]), new_entity));
					flag = 1;
				}
			}
		}
	}
	
	tmp = "select * from totalmessage where id > " + last_message_no + " order by id desc";

	std::vector<char> writable2(tmp.begin(), tmp.end());
	writable2.push_back('\0');
	char* ptr2 = &writable2[0];

	sql_result = query(ptr2);
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		if (atoi(last_message_no.c_str()) < atoi(((string)sql_row[0]).c_str()))last_message_no = (string)sql_row[0];
		if (FilterInfo::GetInstance()->checkSaveOption(atoi(sql_row[1])) && FilterInfo::GetInstance()->checkSaveOption(atoi(sql_row[2]))) {// 저장 옵션 활성화 된 엔티티만
			listener_event le;
			if (sql_row[1] != "") le.id = atoi(sql_row[1]);
			if (sql_row[2] != "") le.wid = atoi(sql_row[2]);
			if (sql_row[3] != "") le.datetime = sql_row[3];
			if (sql_row[4] != ""&&sql_row[4] != NULL) le.datetime2 = sql_row[4];
			if (sql_row[5] != "") le.state = sql_row[5];
			if (sql_row[6] != "") le.status = sql_row[6];
			if (sql_row[7] != "") le.value = sql_row[7];
			if (sql_row[8] != "") le.table_name = sql_row[8];

			auto v = explode(sql_row[7], ';');
			int i;
			for (i = 0; i < v.size(); i++) {
				auto w = explode(v[i], ':');
				if (w.size()) {
					le.val[i] = w[0];
					le.data[i] = w[1];
				}
			}

			currentDataSet.insert(unordered_map<int, listener_event>::value_type(atoi(sql_row[0]), le));
			flag = 1;
		}
	}

	mysql_free_result(sql_result);
	return flag;
}





/**************
* Purpose: 필터저장하기
**************/
void DataManager::setFilterOption(listener_event *le, entity *e, int f, bool type, bool del) {//추가용

	if (del) { //삭제용
		if (f >0) {

			string tmp = " delete from `filter` where id = " + to_string(f);
			std::vector<char> writable(tmp.begin(), tmp.end());
			writable.push_back('\0');
			char* ptr = &writable[0];

			MYSQL_RES *sql_result = query(ptr);
		}
	}
	else { //추가용
		if (type) { //save용
			if (e != NULL) {

				string tmp = "INSERT INTO `filter`( `process`, `dp`, `up`, `entity`, `topic_name`, `forSave`, `forView`) VALUES ";
					tmp += "('" + (*e).process + "','" + (*e).domainParticipant + "','" + (*e).publisher + "','" + (*e).dataWriter + "','" + (*e).topicName + "',true,false )";
					std::vector<char> writable(tmp.begin(), tmp.end());
					writable.push_back('\0');
					char* ptr = &writable[0];
					query(ptr);


					string r = sql_fetch("select * from `filter` order by id desc");
					int intr = stoi(r);

					listener_event tmp_le;
					tmp_le.state = "";
					tmp_le.status = "";

					filterData f = { intr , *e, tmp_le, true, false };

					FilterInfo::GetInstance()->currentFilterData.insert(unordered_map<int, filterData>::value_type(intr, f));
					FilterInfo::GetInstance()->count_save++;
				}
				
		}
		else { //view용
			if (le != NULL) {


					string tmp = "INSERT INTO `filter`(  `state`, `status`, `forSave`, `forView`) VALUES ";
					tmp += "('" + (*le).state + "','" + (*le).status + "',false,true )";
					std::vector<char> writable(tmp.begin(), tmp.end());
					writable.push_back('\0');
					char* ptr = &writable[0];
					query(ptr);


					string r = sql_fetch("select * from `filter` order by id desc");
					int intr = stoi(r);

					entity tmp_e;
					tmp_e.process = "";
					tmp_e.domainParticipant = "";
					tmp_e.publisher = "";
					tmp_e.dataWriter = "";
					tmp_e.topicName = "";


					filterData f = { intr , tmp_e,*le, false , true };

					FilterInfo::GetInstance()->currentFilterData.insert(unordered_map<int, filterData>::value_type(intr, f));
					FilterInfo::GetInstance()->count_view_le++;


			}
			if (e != NULL) {


					string tmp = "INSERT INTO `filter`(  `process`, `dp`, `up`, `entity`, `topic_name`, `forSave`, `forView`) VALUES ";
					tmp += "('" + (*e).process + "','" + (*e).domainParticipant + "','" + (*e).publisher + "','" + (*e).dataWriter + "','" + (*e).topicName + "',false,true )";
					std::vector<char> writable(tmp.begin(), tmp.end());
					writable.push_back('\0');
					char* ptr = &writable[0];
					query(ptr);


					string r = sql_fetch("select * from `filter` order by id desc");
					int intr = stoi(r);

					listener_event tmp_le;
					tmp_le.state = "";
					tmp_le.status = "";

					filterData f = { intr , *e, tmp_le, false , true };

					FilterInfo::GetInstance()->currentFilterData.insert(unordered_map<int, filterData>::value_type(intr, f));
					FilterInfo::GetInstance()->count_view_e++;
			}
		}
	}
}


/**************
* Purpose: 필터 불러오기
**************/
void DataManager::getFilterOption() {
	string tmp = "select * from `filter` order by id desc";
	int flag = 0;
	std::vector<char> writable(tmp.begin(), tmp.end());
	writable.push_back('\0');
	char* ptr = &writable[0];

	MYSQL_RES *sql_result = query(ptr);
	MYSQL_ROW sql_row;
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		entity tmp_e;
		tmp_e.process =sql_row[1];
		tmp_e.domainParticipant =  sql_row[2];
		tmp_e.publisher = sql_row[3];
		tmp_e.dataWriter = sql_row[4];
		tmp_e.topicName = sql_row[5];

		listener_event tmp_le;
		tmp_le.state = sql_row[6];
		tmp_le.status = sql_row[7];

		bool tmp_save = false;
		bool tmp_view = false;
		if (sql_row[1] != "") {

			tmp_e.id = atoi(sql_row[10]);
			if (atoi(((string)sql_row[8]).c_str()) == 1) {
				tmp_save = true;
				FilterInfo::GetInstance()->count_save++;

			}
			else {
				tmp_view = true;
				FilterInfo::GetInstance()->count_view_e++;
			}
		}
		else {
			tmp_le.id = atoi(sql_row[10]);
			FilterInfo::GetInstance()->count_view_le++;

		}

		filterData f = { atoi(sql_row[0]) , tmp_e, tmp_le, tmp_save , tmp_view };

		FilterInfo::GetInstance()->currentFilterData.insert(unordered_map<int, filterData>::value_type(atoi(sql_row[0]), f));
		
	}


	mysql_free_result(sql_result);
}




/**************
* Purpose: 엔티티 찾아보고 없으면 저장
**************/
int DataManager::saveEntity(string guid, string domain_id, string qos, string ub_id, string topic_name, string topic_type, string ub_qos, string dp_qos, string topic_qos) {

	string tmp = "";

	tmp = "select * from `entity` where `guid` like '" + guid + "'";

	std::vector<char> writable(tmp.begin(), tmp.end());
	writable.push_back('\0');
	char* ptr = &writable[0];
	MYSQL_RES *sql_result = query(ptr);
	MYSQL_ROW sql_row;	
	string r = "";
	if ((sql_row = mysql_fetch_row(sql_result)) == NULL) {
		//insert 코드
		tmp = "INSERT INTO `entity`( `guid`, `domain`, `host`, `app`, `instance`, `entity`, `entity_kind`, `up_entity`, `qos`, `topic_name`, `topic_type`) VALUES ";
		tmp += "('" + guid + "','" + domain_id + "','" + guid.substr(0, 8) + "','" + guid.substr(8, 8) + "','" + guid.substr(16, 8) + "','";
		tmp += guid.substr(24, 6) + "','" + guid.substr(30, 2) + "','" + ub_id + "','" + qos + "','" + topic_name + "','" + topic_type + "')";
		writable.clear();
		writable.assign(tmp.begin(), tmp.end());
		writable.push_back('\0');
		ptr = &writable[0];
		query(ptr);

		if (ub_qos != "") {
			tmp = "select * from `entity` where `domain` like '" + domain_id + "' and  `host` like '" + guid.substr(0, 8) + "' and  `app` like '" + guid.substr(8, 8) + "' and  `instance` like '" + guid.substr(16, 8) + "' and  `up_entity` like '" + ub_id + "'  and  `entity_kind` like '" + guid.substr(30, 2) + "' and  `entity` like ''   and  `topic_name` is NULL ";
			writable.clear();
			writable.assign(tmp.begin(), tmp.end());
			writable.push_back('\0');
			char* ptr = &writable[0];
			MYSQL_RES *sql_result = query(ptr);
			MYSQL_ROW sql_row;
			if ((sql_row = mysql_fetch_row(sql_result)) == NULL) {
				tmp = "INSERT INTO `entity`( `guid`,`entity`, `domain`, `host`, `app`, `instance`,`entity_kind`, `up_entity`, `qos`) VALUES ";
				tmp += "('','','" + domain_id + "','" + guid.substr(0, 8) + "','" + guid.substr(8, 8) + "','" + guid.substr(16, 8) + "','";
				tmp += guid.substr(30, 2) + "','"+ub_id + "','" + ub_qos + "')";
				std::vector<char> writable2(tmp.begin(), tmp.end());
				writable2.push_back('\0');



				ptr = &writable2[0];
				query(ptr);

				string tmp2 = "select * from `entity` where `domain` like '" + domain_id + "' and  `host` like '" + guid.substr(0, 8) + "' and  `app` like '" + guid.substr(8, 8) + "' and  `instance` like '" + guid.substr(16, 8) + "' and  `up_entity` like '" + ub_id + "'  and  `entity_kind` like '" + guid.substr(30, 2) + "' and  `entity` like ''   and  `topic_name` is NULL ";


				std::vector<char> writable3(tmp2.begin(), tmp2.end());
				writable3.push_back('\0');



				ptr = &writable3[0];

				string r = sql_fetch(ptr);
				int find_id= stoi(r);

				if (guid.substr(30, 2) == "02" || guid.substr(30, 2) == "03" || guid.substr(30, 2) == "c2" || guid.substr(30, 2) == "c3") { //writer 타입
					entity new_entity = { find_id, "", domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8), "", "", "", ub_id, "", "", ub_qos };
					MainSystem::GetInstance()->qosEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));
				}
				else if (guid.substr(30, 2) == "04" || guid.substr(30, 2) == "07" || guid.substr(30, 2) == "c4" || guid.substr(30, 2) == "c7") { //reader 타입
					entity new_entity = { find_id, "", domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8), "", "", ub_id, "", "", "", ub_qos };
					MainSystem::GetInstance()->qosEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));
				}

			}
		}
		if (topic_qos != "") {

			tmp = "select * from `entity` where `domain` like '" + domain_id + "' and  `host` like '" + guid.substr(0, 8) + "' and  `app` like '" + guid.substr(8, 8) + "' and  `instance` like '" + guid.substr(16, 8) + "' and  `topic_name` like '" + topic_name + "' and  `entity` like ''   and  `entity_kind` like '' ";
			writable.clear();
			writable.assign(tmp.begin(), tmp.end());
			writable.push_back('\0');
			char* ptr = &writable[0];
			MYSQL_RES *sql_result = query(ptr);
			MYSQL_ROW sql_row;
			if ((sql_row = mysql_fetch_row(sql_result)) == NULL) {
				tmp = "INSERT INTO `entity`( `guid`,`entity`, `entity_kind`, `domain`, `host`, `app`, `instance`, `topic_name`, `topic_type`,  `qos`) VALUES ";
				tmp += "('','','','" + domain_id + "','" + guid.substr(0, 8) + "','" + guid.substr(8, 8) + "','" + guid.substr(16, 8) + "','";
				tmp +=  topic_name + "', '" + topic_type + "' ,'" + topic_qos + "')";

				std::vector<char> writable2(tmp.begin(), tmp.end());
				writable2.push_back('\0');



				ptr = &writable2[0];
				query(ptr);

				string tmp2 = "select * from `entity` where `domain` like '" + domain_id + "' and  `host` like '" + guid.substr(0, 8) + "' and  `app` like '" + guid.substr(8, 8) + "' and  `instance` like '" + guid.substr(16, 8) + "' and  `topic_name` like '" + topic_name + "' and  `entity` like ''   and  `entity_kind` like '' ";


				std::vector<char> writable3(tmp2.begin(), tmp2.end());
				writable3.push_back('\0');



				ptr = &writable3[0];

				string r = sql_fetch(ptr);
				int find_id = stoi(r);

					entity new_entity = { find_id, "", domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8), topic_name, topic_type, "", "", "", "", topic_qos };
					MainSystem::GetInstance()->qosEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));


			}
		}
		if (dp_qos != "") {
			tmp = "select * from `entity` where `domain` like '" + domain_id + "' and  `host` like '" + guid.substr(0, 8) + "' and  `app` like '" + guid.substr(8, 8) + "' and  `instance` like '" + guid.substr(16, 8) + "' and  `up_entity` is NULL  and  `entity_kind` like ''   and  `topic_name` is NULL ";
			writable.clear();
			writable.assign(tmp.begin(), tmp.end());
			writable.push_back('\0');
			char* ptr = &writable[0];
			MYSQL_RES *sql_result = query(ptr);
			MYSQL_ROW sql_row;
			if ((sql_row = mysql_fetch_row(sql_result)) == NULL) {
				tmp = "INSERT INTO `entity`(  `guid`, `entity`, `entity_kind`,`domain`, `host`, `app`, `instance`, `qos`) VALUES ";
				tmp += "('','','','" + domain_id + "','" + guid.substr(0, 8) + "','" + guid.substr(8, 8) + "','" + guid.substr(16, 8) + "','";
				tmp += dp_qos + "')";

				cout << tmp << endl << endl;

				std::vector<char> writable2(tmp.begin(), tmp.end());
				writable2.push_back('\0');



				ptr = &writable2[0];
				query(ptr);



				string tmp2 = "select * from `entity` where `domain` like '" + domain_id + "' and  `host` like '" + guid.substr(0, 8) + "' and  `app` like '" + guid.substr(8, 8) + "' and  `instance` like '" + guid.substr(16, 8) + "' and  `up_entity` is NULL  and  `entity_kind` like ''   and  `topic_name` is NULL ";


				std::vector<char> writable3(tmp2.begin(), tmp2.end());
				writable3.push_back('\0');



				ptr = &writable3[0];

				string r = sql_fetch(ptr);
				int find_id = stoi(r);





				entity new_entity = { find_id, "", domain_id, guid.substr(0, 8), guid.substr(8, 8), guid.substr(16, 8),  "", "", "", "", "", "", dp_qos };
				MainSystem::GetInstance()->qosEntitySet.insert(unordered_map<int, entity>::value_type(find_id, new_entity));

			}
		}

		tmp = "select * from `entity` where `guid` like '" + guid + "'";
		std::vector<char> writable3(tmp.begin(), tmp.end());
		writable3.push_back('\0');



		ptr = &writable3[0];

		r = sql_fetch(ptr);
		return stoi(r);

	}
	else {
		r = sql_row[0];
		return stoi(r);
		//update 코드
		if (domain_id != "" || qos != "" || ub_id != "") {
			tmp = "UPDATE `entity` SET ";
			tmp += " `guid`= '" + guid + "' ";
			if (domain_id != "") tmp += " , `domain`='" + domain_id + "'";
			if (qos != "") tmp += " , `qos`='" + qos + "'";
			if (ub_id != "") tmp += " , `up_entity`='" + ub_id + "'";
			if (topic_name != "") tmp += " , `topic_name`='" + topic_name + "'";
			if (topic_type != "") tmp += " , `topic_type`='" + topic_type + "'";
			tmp += " WHERE `guid` like '" + guid + "' ";

			writable.clear();
			writable.assign(tmp.begin(), tmp.end());
			writable.push_back('\0');
			char* ptr = &writable[0];
			query(ptr);
		}
	}
}
















/* DB 기본 도구 시작*/


/**************
* Purpose: 데이터베이스와 연결
**************/
int DataManager::connect()
{
	if (sock) //already connected to another database
		disconnect(); //disconnect from that one

					  //initilize the socket
	mysql_init(&conn);

	//something went wrong with the socket
	/*if (!sock)
	return ERR201;
	*/
	//try connecting to the database  

	sock = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	if (sock == NULL)
		return ERR202;

	//successfully connected to the database    
	return SUCCESS;
}

/**************
* Purpose: 데이터 베이스와 연결 해제
**************/
bool DataManager::disconnect()
{
	if (sock) //they have a socket open
		mysql_close(sock);

	//release result data
	free();

	//database disconnected
	return true;
}

/**************
* Purpose: 데이터베이스 쿼리로 부터 결과 지우기
**************/
bool DataManager::free()
{
	if (result)
	{
		mysql_free_result(result);
		return true;
	}

	return false;
}

/**************
* Purpose: DB 에러 미시지 처리
**************/
char *DataManager::dberror(int errorcode)
{

	//display the appropriate error message for this error    
	switch (errorcode)
	{
	case SUCCESS: return "SUCCESS";
		break;

	case ERR201: return "201 SOCKET ERROR: SOCKET FAILURE";
		break;

	case ERR202: return "202 CONNECTION ERROR: CANNOT ACCESS THE SERVER";
		break;

	case ERR203: return "203 DATABASE ERROR: QUERY FAILED";
		break;
	}

	return NULL; //no error, return null
}

/**************
* Purpose: 쿼리 결과
**************/
MYSQL_RES *DataManager::query(char *query)
{
	if (query_print) printf("%s\n", query);
	if (!sock) connect();
	//query the database
	int query_stat = mysql_query(sock, query);
	if (query_stat != 0) {
		printf("Mysql query Error : %s ", mysql_error(&conn));
		return NULL;
	}

	//store the results
	result = mysql_store_result(sock);
	return result;
}


/**************
* Purpose: 쿼리 실행
**************/
string DataManager::sql_fetch(char *query)
{
	if (query_print) printf("%s\n", query);
	if (!sock) connect();
	//query the database

	int query_stat = mysql_query(sock, query);

	if (query_stat != 0) {
		printf("Mysql query Error : %s ", mysql_error(&conn));
		return NULL;
	}
	//store the results
	result = mysql_store_result(sock);

	MYSQL_ROW row;
	if ((row = mysql_fetch_row(result))) {
		//fetch the row 
		//store the result & convert it to a number
		string stringResult = row[0];
		//free the results
		free();

		return stringResult;
	}
	else {
		return NULL; //no results
	}
}

