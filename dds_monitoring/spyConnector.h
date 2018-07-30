
#ifndef INCLUDEONCE_H
#define INCLUDEONCE_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#endif
using namespace std;

/**************
* Purpose: 스파이인터페이스 헤더파일
**************/

#ifndef SPY_H
#define SPY_H



#include "dataManager.h"
#include "mainSystem.h"
#include "filterInfo.h"
//#include "spyConnector.h"
#include "uiManager.h"

// Sample states to support reads
typedef unsigned long SampleStateKind;
const SampleStateKind READ_SAMPLE_STATE = 0x0001 << 0;
const SampleStateKind NOTREADSAMPLESTATE = 0x0001 << 1;

// View states to support reads
typedef unsigned long ViewStateKind;
const ViewStateKind NEW_VIEW_STATE = 0x0001 << 0;
const ViewStateKind NOT_NEW_VIEW_STATE = 0x0001 << 1;

// Instance states to support reads
typedef unsigned long InstanceStateKind;
const InstanceStateKind ALIVE_INSTANCE_STATE = 0x0001 << 0;
const InstanceStateKind NOT_ALIVE_DISPOSED_INSTANCE_STATE = 0x0001 << 1;
const InstanceStateKind NOT_ALIVE_NO_WRITERS_INSTANCE_STATE = 0x0001 << 2;


class SPYConnector
{
private:

public:
	SPYConnector();

	void onRDMfromSPY(
		/* [string][in] */ string dr_guid,
		/* [string][in] */ string st_guid,
		/* [in] */ long total_count,
		/* [in] */ long total_count_change,
		/* [string][in] */ long t);// interface #1

	void onSRfromSPY(
		/* [string][in] */ string dr_guid,
		/* [string][in] */ string st_guid,
		/* [in] */ long total_count,
		/* [in] */ long total_count_change,
		/* [in] */ int last_reason,
		/* [string][in] */ long t);

	void onLCfromSPY(
		/* [string][in] */ string dr_guid,
		/* [string][in] */ string st_guid,
		/* [in] */ long alive_count,
		/* [in] */ long not_alive_count,
		/* [in] */ long alive_count_change,
		/* [in] */ long not_alive_count_change,
		/* [string][in] */ long t);

	void onSMfromSPY(
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
		/* [string][in] */ long t);

	void onRIQOSfromSPY(
		/* [string][in] */ string dr_guid,
		/* [in] */ long total_count,
		/* [in] */ long last_policy_id,
		/* [string][in] */ string st_policies,
		/* [string][in] */ long t);

	void onSLfromSPY(
		/* [string][in] */ string dr_guid,
		/* [in] */ long total_count,
		/* [in] */ long total_count_change,
		/* [string][in] */ long t);

	void onDAfromSPY(
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
		/* [in] */ int valid_data);
};

#endif