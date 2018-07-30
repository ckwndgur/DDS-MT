//Deigned by CJH
//Make relation of entities
#pragma once

#include <list>
#include <string>
#include "DataManager.h"
#include "mainSystem.h"

#define EntityDistance 20
#define EntityWidth 70
#define HOST 0
#define PROCESS 1
#define DOMAINPARTICIPANT 2
#define PUB 3
#define SUB 4
#define DATAWRITER 5
#define DATAREADER 6
#define TOPIC 7

using namespace std;



class RelationshipView
{
	/*Operation*/
public:
	RelationshipView();
	~RelationshipView();

	static RelationshipView* GetInstance();
	void Cal_AllPosition(string domain);
	void Clear_AllPosition();
	int Cal_RWTSize(string dwdrt);
	position Find_Position(int kind, entity e);
	void Edit_Position(int kind, position p);
	bool Comapare_Reduplication(list<string> a, string b);
	bool Comapare_Reduplication(list<position> a, position p);

private:
	//sort dataset
	//list<string> Del_Reduplication(list<string> lstString);
	list<entity> Get_EntitySet();
	list<string> Save_SortedHost(string d);
	list<string> Save_SortedProcess(string d, string h);
	list<string> Save_SortedDP(string d, string h, string p);
	list<string> Save_SortedPub(string d, string h, string p, string dp);
	list<string> Save_SortedSub(string d, string h, string p, string dp);
	list<string> Save_SortedDW(string d, string h, string p, string dp, string pub);
	list<string> Save_SortedDR(string d, string h, string p, string dp, string sub);
	list<string> Save_SortedTopic(string d, string h, string p, string dp);
	
	//needs to calculate position of entity
	int Cnt_HinD(string d);
	int Cnt_PinH(string d, string h);
	int Cnt_DPinP(string d, string h, string p);
	int Cnt_TinDP(string d, string h, string p, string dp);
	int Cnt_PSinDP(string d, string h, string p, string dp);
	int Cnt_PinDP(string d, string h, string p, string dp);
	int Cnt_SinDP(string d, string h, string p, string dp);
	int Cnt_DRinSub(string d, string h, string p, string dp, string sub);
	int Cnt_DWinPub(string d, string h, string p, string dp, string pub);
	int Cnt_RWinDP(string d, string h, string p, string dp);
	int Cnt_RWinP(string d, string h, string p);
	int Cnt_PSinP(string d, string h, string p);
	int Cnt_TinP(string d, string h, string p);
	int Cnt_TotalProcess(string d, string host);
	int Cnt_TotalDP(string d, string host);
	int Cnt_TotalPub(string d, string host);
	int Cnt_TotalSub(string d, string host);
	int Cnt_TotalDW(string d, string host);
	int Cnt_TotalDR(string d, string host);
	int Cnt_TotalTopic(string d, string host);
	int Cal_EmptySpace(int kind, entity entity);
	int Cal_TotalTopicSize(string host);
	

	//calcuate position of entity
	//void Cal_DWPosition(int n, entity entity);
	void Cal_HostPosition(int nhost, entity entity);
	void Cal_ProcessPosition(int nhost, int nprocess, entity entity);
	void Cal_DPPosition(int nprocess, int ndp, entity entity);
	void Cal_PubPosition(int nprocess, int ndp, int npub, entity entity);
	void Cal_SubPosition(int nprocess, int ndp, int nsub, entity entity);
	void Cal_DWPosition(int nprocess, int ndp, int npub, int ndw, entity entity);
	void Cal_DRPosition(int nprocess, int ndp, int nsub, int ndr, entity entity);
	void Cal_TopicPosition(int nprocess, int ndp, int nt, entity entity);

	/*attribute*/
public:
	list<position> lstDRPosition;
	list<position> lstDWPosition;
	list<position> lstPubPosition;
	list<position> lstSubPosition;
	list<position> lstDPPosition;
	list<position> lstProcessPosition;
	list<position> lstHostPosition;
	list<position> lstTopicPosition;


private:
	static RelationshipView* instance;
	list<entity> lstEntitySet;
};

