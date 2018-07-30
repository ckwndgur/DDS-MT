#include "stdafx.h"
#include "spyConnector.h"

/**************
* Purpose: ������
**************/
SPYConnector::SPYConnector()
{
}


/**************
* Purpose: requested_deadline_missed ������ ������ο� Ư�� �ν��Ͻ��� �ؼ����� ������ ��
**************/
void SPYConnector::onRDMfromSPY(
	/* [string][in] */ string dr_guid,
	/* [string][in] */ string st_guid,
	/* [in] */ long total_count,
	/* [in] */ long total_count_change,
	/* [string][in] */ long t)// interface #1
{
	listener_event le;

	le.id = MainSystem::GetInstance()->getEntity(dr_guid); //dr_entity
	le.wid = MainSystem::GetInstance()->getEntity(st_guid); //st_entity

	bool ch1 = FilterInfo::GetInstance()->checkSaveOption(le.id);
	bool ch2 = FilterInfo::GetInstance()->checkSaveOption(le.wid);


	time_t now_time = t;
	struct tm tm;
	char str[256];

	//time(&now_time); //���� �� ���� �ð��� ����
	localtime_s(&tm, &now_time); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
								 //localtime(&now_time);

	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm);

	//ctime_s(str, sizeof(str), &now_time);//����ð��� ���ڿ��� ��ȯ


	if (ch1&&ch2) {


		le.table_name = "RDM";
		le.datetime = str;
		le.state = "Warning";
		le.status = "Requested_Deadline_Missed";
		int i = 0;
		le.val[i] = "St_GUID";
		le.data[i++] = st_guid;
		le.val[i] = "Total_Count";
		le.data[i++] = to_string(total_count);
		le.val[i] = "Total_Count_Change";
		le.data[i++] = to_string(total_count_change);
		le.value = "";
		for (int j = 0; j < i; j++) {
			le.value += le.val[j] + ":" + le.data[j] + ";";
		}

		DataManager::GetInstance()->saveData(le);


	}


}




/**************
* Purpose: sample_rejected ������ ���������� ��
**************/

void SPYConnector::onSRfromSPY(
	/* [string][in] */ string dr_guid,
	/* [string][in] */ string st_guid,
	/* [in] */ long total_count,
	/* [in] */ long total_count_change,
	/* [in] */ int last_reason,
	/* [string][in] */ long t)
{

	listener_event le;





	le.id = MainSystem::GetInstance()->getEntity(dr_guid); //dr_entity
	le.wid = MainSystem::GetInstance()->getEntity(st_guid); //st_entity

	bool ch1 = FilterInfo::GetInstance()->checkSaveOption(le.id);
	bool ch2 = FilterInfo::GetInstance()->checkSaveOption(le.wid);



	time_t now_time = t;
	struct tm tm;
	char str[256];

	//time(&now_time); //���� �� ���� �ð��� ����
	localtime_s(&tm, &now_time); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
								 //localtime(&now_time);

	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm);

	//ctime_s(str, sizeof(str), &now_time);//����ð��� ���ڿ��� ��ȯ




	if (ch1&&ch2) {
		le.table_name = "SR";
		le.datetime = str;
		le.state = "Warning";
		le.status = "Sample_Rejected";
		int i = 0;
		le.val[i] = "St_GUID";
		le.data[i++] = st_guid;
		le.val[i] = "Total_Count";
		le.data[i++] = to_string(total_count);
		le.val[i] = "Total_Count_Change";
		le.data[i++] = to_string(total_count_change);
		le.val[i] = "Last_Reason";
		le.data[i++] = to_string(last_reason);
		le.value = "";
		for (int j = 0; j < i; j++) {
			le.value += le.val[j] + ":" + le.data[j] + ";";
		}

		DataManager::GetInstance()->saveData(le);
	}


}




/**************
* Purpose: liveliness_changed ���� �ν��Ͻ��� �а� �ִ� ������ ��ȭ�� ���� �� (��Ŀ����?)
**************/

void SPYConnector::onLCfromSPY(
	/* [string][in] */ string dr_guid,
	/* [string][in] */ string st_guid,
	/* [in] */ long alive_count,
	/* [in] */ long not_alive_count,
	/* [in] */ long alive_count_change,
	/* [in] */ long not_alive_count_change,
	/* [string][in] */ long t)
{
	listener_event le;





	le.id = MainSystem::GetInstance()->getEntity(dr_guid); //dr_entity
	le.wid = MainSystem::GetInstance()->getEntity(st_guid); //st_entity


	bool ch1 = FilterInfo::GetInstance()->checkSaveOption(le.id);
	bool ch2 = FilterInfo::GetInstance()->checkSaveOption(le.wid);




	time_t now_time = t;
	struct tm tm;
	char str[256];

	//time(&now_time); //���� �� ���� �ð��� ����
	localtime_s(&tm, &now_time); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
								 //localtime(&now_time);

	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm);

	//ctime_s(str, sizeof(str), &now_time);//����ð��� ���ڿ��� ��ȯ




	if (ch1&&ch2) {

		le.table_name = "LC";
		le.datetime = str;
		le.state = "Event";
		le.status = "Liveliness_Changed";
		int i = 0;
		le.val[i] = "St_GUID";
		le.data[i++] = st_guid;
		le.val[i] = "Alive_Count";
		le.data[i++] = to_string(alive_count);
		le.val[i] = "Not_Alive_Count";
		le.data[i++] = to_string(not_alive_count);
		le.val[i] = "Alive_Count_Change";
		le.data[i++] = to_string(alive_count_change);
		le.val[i] = "Not_Alive_Count_Change";
		le.data[i++] = to_string(not_alive_count_change);
		le.value = "";
		for (int j = 0; j < i; j++) {
			le.value += le.val[j] + ":" + le.data[j] + ";";
		}
		DataManager::GetInstance()->saveData(le);

	}
}




/**************
* Purpose: subscription_matched  ���Ȱ� QOS�� ���� DW�� ��Ī�Ǿ��� ��
**************/

void SPYConnector::onSMfromSPY(
	/* [string][in] */ string dr_guid,
	/* [string][in] */ string st_guid,
	/* [string][in] */ string sub_id,
	/* [string][in] */ string ub_id,
	/* [in] */ long domain_id,
	/* [string][in] */ string dr_qos,
	/* [string][in] */ string st_qos,
	/* [string][in] */ string drp_qos,
	/* [string][in] */ string dwp_qos,
	/* [string][in] */ string topic_qos,
	/* [string][in] */ string sub_qos,
	/* [string][in] */ string pub_qos,
	/* [string][in] */ string topic_info,
	/* [in] */ long total_count,
	/* [in] */ long current_count,
	/* [string][in] */ long t)
{


	listener_event le;





	std::stringstream stream;

	stream << std::hex << domain_id;

	string topic_name = "";
	string topic_type = "";
	if (topic_info != "") {
		topic_name = topic_info.substr(0, (topic_info.find(":")));
		topic_type = topic_info.substr(topic_info.find(":") + 1);
		topic_type = topic_type.substr(0, topic_type.length() - 1);
	}

	cout << topic_info << endl
		<< topic_name << endl
		<< topic_type << endl;

	le.id = MainSystem::GetInstance()->getEntity(dr_guid, stream.str(), dr_qos, sub_id, topic_name, topic_type, sub_qos, drp_qos, topic_qos); //dr_entity
	le.wid = MainSystem::GetInstance()->getEntity(st_guid, stream.str(), st_qos, ub_id, topic_name, topic_type, pub_qos, dwp_qos, topic_qos); //st_entity

	bool ch1 = FilterInfo::GetInstance()->checkSaveOption(le.id);
	bool ch2 = FilterInfo::GetInstance()->checkSaveOption(le.wid);




	time_t now_time = t;
	struct tm tm;
	char str[256];

	//time(&now_time); //���� �� ���� �ð��� ����
	localtime_s(&tm, &now_time); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
								 //localtime(&now_time);

	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm);

	//ctime_s(str, sizeof(str), &now_time);//����ð��� ���ڿ��� ��ȯ



	if (ch1&&ch2) {

		le.table_name = "SM";
		le.datetime = str;
		le.state = "Event";
		le.status = "Subscription_Matched";
		int i = 0;
		le.val[i] = "St_GUID";
		le.data[i++] = st_guid;
		le.val[i] = "Total_Count";
		le.data[i++] = to_string(total_count);
		le.val[i] = "Current_Count";
		le.data[i++] = to_string(current_count);
		le.value = "";
		for (int j = 0; j < i; j++) {
			le.value += le.val[j] + ":" + le.data[j] + ";";
		}
		DataManager::GetInstance()->saveData(le);

	}
}




/**************
* Purpose: requested_incompatible_qos ȣȯ���� ���� QOS�� ���� ��
**************/

void SPYConnector::onRIQOSfromSPY(
	/* [string][in] */ string dr_guid,
	/* [in] */ long total_count,
	/* [in] */ long last_policy_id,
	/* [string][in] */ string st_policies,
	/* [string][in] */ long t)
{

	listener_event le;



	le.id = MainSystem::GetInstance()->getEntity(dr_guid); //dr_entity


	bool ch1 = FilterInfo::GetInstance()->checkSaveOption(le.id);



	time_t now_time = t;
	struct tm tm;
	char str[256];

	//time(&now_time); //���� �� ���� �ð��� ����
	localtime_s(&tm, &now_time); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
								 //localtime(&now_time);

	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm);

	//ctime_s(str, sizeof(str), &now_time);//����ð��� ���ڿ��� ��ȯ



	if (ch1) {

		le.table_name = "RIQOS";
		le.datetime = str;
		le.state = "warning";
		le.status = "Requested_Incompatible_QOS";
		int i = 0;
		le.val[i] = "Total_Count";
		le.data[i++] = to_string(total_count);
		le.val[i] = "Last_Policy_Id";
		le.data[i++] = to_string(last_policy_id);
		le.val[i] = "Dt_Policies";
		le.data[i++] = st_policies;
		le.value = "";
		for (int j = 0; j < i; j++) {
			le.value += le.val[j] + ":" + le.data[j] + ";";
		}
		DataManager::GetInstance()->saveData(le);

	}
}




/**************
* Purpose: sample_lost ������ �������� ���� ��
**************/

void SPYConnector::onSLfromSPY(
	/* [string][in] */ string dr_guid,
	/* [in] */ long total_count,
	/* [in] */ long total_count_change,
	/* [string][in] */ long t)
{
	listener_event le;

	le.id = MainSystem::GetInstance()->getEntity(dr_guid); //dr_entity

	bool ch1 = FilterInfo::GetInstance()->checkSaveOption(le.id);



	time_t now_time = t;
	struct tm tm;
	char str[256];

	//time(&now_time); //���� �� ���� �ð��� ����
	localtime_s(&tm, &now_time); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
								 //localtime(&now_time);

	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm);

	//ctime_s(str, sizeof(str), &now_time);//����ð��� ���ڿ��� ��ȯ



	if (ch1) {


		le.table_name = "SL";
		le.datetime = str;
		le.state = "warning";
		le.status = "Sample_Lost";
		int i = 0;
		le.val[i] = "Total_Count";
		le.data[i++] = to_string(total_count);
		le.val[i] = "Total_Count_Change";
		le.data[i++] = to_string(total_count_change);
		le.value = "";
		for (int j = 0; j < i; j++) {
			le.value += le.val[j] + ":" + le.data[j] + ";";
		}
		DataManager::GetInstance()->saveData(le);

	}

}




/**************
* Purpose: data_available �����Ͱ� ��ȿ�� ��
**************/

void SPYConnector::onDAfromSPY(
	/* [string][in] */ string dr_guid,
	/* [string][in] */ string dw_guid,
	/* [string][in] */ long t1_sec,
	/* [in] */ unsigned long t1_nanosec,
	/* [string][in] */ long t2_sec,
	/* [in] */ unsigned long t2_nanosec,
	/* [in] */ unsigned long sample_state,
	/* [in] */ unsigned long view_state,
	/* [in] */ unsigned long instance_state,
	/* [in] */ long disposed_generation_count,
	/* [in] */ long no_writers_generation_count,
	long sample_rank,
	/* [in] */ long generation_rank,
	/* [in] */ long absolute_generation_rank,
	/* [in] */ int valid_data)
{
	listener_event le;


	le.id = MainSystem::GetInstance()->getEntity(dr_guid); //dr_entity
	le.wid = MainSystem::GetInstance()->getEntity(dw_guid); //st_entity

	bool ch1 = FilterInfo::GetInstance()->checkSaveOption(le.id);
	bool ch2 = FilterInfo::GetInstance()->checkSaveOption(le.wid);




	time_t now_time = t1_sec;
	struct tm tm;
	char str[256];

	//time(&now_time); //���� �� ���� �ð��� ����
	localtime_s(&tm, &now_time); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
								 //localtime(&now_time);

	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm);

	//ctime_s(str, sizeof(str), &now_time);//����ð��� ���ڿ��� ��ȯ



	time_t now_time2 = t2_sec;
	struct tm tm2;
	char str2[256];

	//time(&now_time); //���� �� ���� �ð��� ����
	localtime_s(&tm2, &now_time2); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
								   //localtime(&now_time);

	strftime(str2, sizeof(str2), "%Y-%m-%d %H:%M:%S", &tm2);

	//ctime_s(str, sizeof(str), &now_time);//����ð��� ���ڿ��� ��ȯ




	if (ch1&&ch2) {




		le.table_name = "DA";
		le.datetime = str;
		le.datetime2 = str2;
		le.state = "normal";
		le.status = "data_available";
		int i = 0;
		le.val[i] = "dw_guid";
		le.data[i++] = dw_guid;
		le.val[i] = "t1_nanosec";
		le.data[i++] = to_string(t1_nanosec);
		le.val[i] = "t2_nanosec";
		le.data[i++] = to_string(t2_nanosec);
		le.val[i] = "sample_state";
		le.data[i++] = to_string(sample_state);
		le.val[i] = "view_state";
		le.data[i++] = to_string(view_state);
		le.val[i] = "instance_state";
		le.data[i++] = to_string(instance_state);
		le.val[i] = "disposed_generation_count";
		le.data[i++] = to_string(disposed_generation_count);
		le.val[i] = "no_writers_generation_count";
		le.data[i++] = to_string(no_writers_generation_count);
		le.val[i] = "sample_rank";
		le.data[i++] = to_string(sample_rank);
		le.val[i] = "generation_rank";
		le.data[i++] = to_string(generation_rank);
		le.val[i] = "absolute_generation_rank";
		le.data[i++] = to_string(absolute_generation_rank);
		le.val[i] = "valid_data";
		le.data[i++] = to_string(valid_data);
		le.value = "";
		for (int j = 0; j < i; j++) {
			le.value += le.val[j] + ":" + le.data[j] + ";";
		}
		DataManager::GetInstance()->saveData(le);

	}
}



