#include "stdafx.h"
#include "RelationshipView.h"

RelationshipView* RelationshipView::instance = nullptr;

RelationshipView::RelationshipView()
{
}


RelationshipView::~RelationshipView()
{
}


/**************
* Purpose: initialize position data
**************/
void RelationshipView::Clear_AllPosition()
{
	lstDRPosition.clear();
	lstDWPosition.clear();
	lstPubPosition.clear();
	lstSubPosition.clear();
	lstDPPosition.clear();
	lstProcessPosition.clear();
	lstHostPosition.clear();
	lstTopicPosition.clear();
}



/**************
* Purpose: Draw_Realationshipview in dds_monitoringview
**************/
void RelationshipView::Cal_AllPosition(string domain)
{
	Clear_AllPosition();
	lstEntitySet = Get_EntitySet();
	entity entity;
	
	int nHost = 0;
	entity.domainId = domain;
	list<string> lstHost = Save_SortedHost(domain);
	for (list<string>::iterator i = lstHost.begin(); i != lstHost.end(); i++)
	{
		entity.host = *i;
		Cal_HostPosition(nHost, entity);
		nHost++;

		int nProcess = 0;
		list<string> lstProcess = Save_SortedProcess(domain, *i);
		for (list<string>::iterator j = lstProcess.begin(); j != lstProcess.end(); j++)
		{
			entity.process = *j;
			Cal_ProcessPosition(nHost, nProcess, entity);
			nProcess++;

			int nDP = 0;
			list<string> lstDP = Save_SortedDP(domain, *i, *j);
			for (list<string>::iterator k = lstDP.begin(); k != lstDP.end(); k++)
			{
				entity.domainParticipant = *k;
				Cal_DPPosition(nProcess, nDP, entity);
				nDP++;

				int nPub = 0;
				list<string> lstPub = Save_SortedPub(domain, *i, *j, *k);
				for (list<string>::iterator l = lstPub.begin(); l != lstPub.end(); l++)
				{
					entity.publisher = *l;
					Cal_PubPosition(nProcess, nDP, nPub, entity);
					nPub++;

					int nDW = 0;
					list<string> lstDW = Save_SortedDW(domain, *i, *j, *k, *l);
					for (list<string>::iterator m = lstDW.begin(); m != lstDW.end(); m++)
					{
						entity.dataWriter = *m;
						Cal_DWPosition(nProcess, nDP, nPub, nDW, entity);
						nDW++;
					}
				}

				int nSub = 0;
				list<string> lstSub = Save_SortedSub(domain, *i, *j, *k);
				for (list<string>::iterator l = lstSub.begin(); l != lstSub.end(); l++)
				{
					entity.subscriber = *l;
					Cal_SubPosition(nProcess, nDP, nSub, entity);
					nSub++;

					int nDR = 0;
					list<string> lstDR = Save_SortedDR(domain, *i, *j, *k, *l);
					for (list<string>::iterator m = lstDR.begin(); m != lstDR.end(); m++)
					{
						entity.dataReader = *m;
						Cal_DRPosition(nProcess, nDP, nSub, nDR, entity);
						nDR++;
					}
				}

				int nT = 0;
				list<string> lstTopic = Save_SortedTopic(domain, *i, *j, *k);
				for (list<string>::iterator m = lstTopic.begin(); m != lstTopic.end(); m++)
				{
					entity.topicName = *m;
					Cal_TopicPosition(nDP, nProcess, nT, entity);
					nT++;
				}
			}
		}
	}
}

/**************Â÷ÁßÇõ 
* Purpose: Calculate position of one dp according order
**************/
void RelationshipView::Cal_TopicPosition(int nprocess, int ndp, int nt, entity entity)
{
	position p;
	CSize size;
	int width_Topic = EntityWidth;

	if (nt == 0)
	{
		list<position>::iterator dp_iter;
		dp_iter = --lstDPPosition.end();

		size.cx = dp_iter->y.cx - EntityDistance - EntityWidth;
		size.cy = dp_iter->x.cy + EntityDistance + 10;
		p.x = size;

		size.cx = p.x.cx + width_Topic;
		size.cy = dp_iter->y.cy - EntityDistance - 10;
		p.y = size;
		p.name = entity.topicName;
		p.entity = entity;
		p.index = lstTopicPosition.size();

		if (Comapare_Reduplication(lstTopicPosition, p))
		{
			lstTopicPosition.push_back(p);
		}
	}
	else
	{
		list<position>::iterator topic_iter;
		topic_iter = --lstTopicPosition.end();

		size.cx = topic_iter->x.cx - EntityDistance - EntityWidth;
		size.cy = topic_iter->x.cy;
		p.x = size;

		size.cx = topic_iter->x.cx - EntityDistance;
		size.cy = topic_iter->y.cy;
		p.y = size;
		p.name = entity.topicName;
		p.entity = entity;
		p.index = lstTopicPosition.size();

		if (Comapare_Reduplication(lstTopicPosition, p))
		{
			lstTopicPosition.push_back(p);
		}
	}
}

/**************
* Purpose: Calculate DR position in one sub
**************/
void RelationshipView::Cal_DRPosition(int nprocess, int ndp, int nsub, int ndr, entity entity)
{
	position p;
	CSize size;
	int width_DR = EntityWidth;
	int nProgression = 4 * nprocess + 2 * ndp + nsub - 6; //a linear equation (nprocess, ndp, npub) -> npub

	if (ndr == 0)
	{
		list<position>::iterator sub_iter;
		sub_iter = --lstSubPosition.end();

		size.cx = sub_iter->x.cx + EntityDistance;
		size.cy = sub_iter->x.cy + EntityDistance;
		p.x = size;

		size.cx = size.cx + width_DR;
		size.cy = sub_iter->y.cy - EntityDistance;
		p.y = size;
		p.name = entity.dataReader;
		p.entity = entity;
		p.index = lstDRPosition.size();

		if (Comapare_Reduplication(lstDRPosition, p))
			lstDRPosition.push_back(p);
	}
	else
	{
		list<position>::iterator dr_iter;
		dr_iter = --lstDRPosition.end();

		size.cx = dr_iter->y.cx + EntityDistance;
		size.cy = dr_iter->x.cy;
		p.x = size;

		size.cx = size.cx + width_DR;
		size.cy = dr_iter->y.cy;
		p.y = size;
		p.name = entity.dataReader;
		p.entity = entity;
		p.index = lstDRPosition.size();

		if (Comapare_Reduplication(lstDRPosition, p))
			lstDRPosition.push_back(p);
	}
}

/**************
* Purpose: Calculate DW position in one pub
**************/
void RelationshipView::Cal_DWPosition(int nprocess, int ndp, int npub, int ndw, entity entity)
{
	position p;
	CSize size;
	int width_DW = EntityWidth;
	int nProgression = 4 * nprocess + 2 * ndp + npub - 6; //a linear equation (nprocess, ndp, npub) -> npub

	if (ndw == 0)
	{
		list<position>::iterator pub_iter;
		pub_iter = --lstPubPosition.end();

		size.cx = pub_iter->x.cx + EntityDistance;
		size.cy = pub_iter->x.cy + EntityDistance;
		p.x = size;

		size.cx = size.cx + width_DW;
		size.cy = pub_iter->y.cy - EntityDistance;
		p.y = size;
		p.name = entity.dataWriter;
		p.entity = entity;
		p.index = lstDWPosition.size();

		if (Comapare_Reduplication(lstDWPosition, p))
			lstDWPosition.push_back(p);
	}
	else
	{
		list<position>::iterator dw_iter;
		dw_iter = --lstDWPosition.end();

		size.cx = dw_iter->y.cx + EntityDistance;
		size.cy = dw_iter->x.cy;
		p.x = size;

		size.cx = size.cx + width_DW;
		size.cy = dw_iter->y.cy;
		p.y = size;
		p.name = entity.dataWriter;
		p.entity = entity;
		p.index = lstDWPosition.size();

		if (Comapare_Reduplication(lstDWPosition, p))
			lstDWPosition.push_back(p);
	}
}

/**************
* Purpose: Calculate Sub position in one DP
**************/
void RelationshipView::Cal_SubPosition(int nprocess, int ndp, int nsub, entity entity)
{
	position p;
	CSize size;
	int nDR = Cnt_DRinSub(entity.domainId, entity.host, entity.process, entity.domainParticipant, entity.subscriber);
	int width_DR = nDR * EntityWidth;
	int nProgression = 2 * nprocess + ndp - 2; //a linear equation (nprocess, ndp) -> ndp
	
	//int nPubinSameDP = Cnt_PinDP(entity.domainParticipant); -> entity º¯¼ö »« ÈÄ ÁøÇà

	if (nsub == 0)
	{
		list<position>::iterator dp_iter = --lstDPPosition.end();

		size.cx = dp_iter->x.cx + EntityDistance;
		size.cy = dp_iter->x.cy + EntityDistance;
		p.x = size;

		size.cx = size.cx + width_DR + Cal_EmptySpace(SUB, entity) * EntityDistance;
		size.cy = dp_iter->y.cy - EntityDistance;
		p.y = size;
		p.name = entity.subscriber;
		p.entity = entity;
		p.index = lstSubPosition.size();

		if (Comapare_Reduplication(lstSubPosition, p))
			lstSubPosition.push_back(p);
	}
	else
	{
		list<position>::iterator sub_iter = --lstSubPosition.end();

		size.cx = sub_iter->y.cx + EntityDistance;
		size.cy = sub_iter->x.cy;
		p.x = size;

		size.cx = size.cx + width_DR + Cal_EmptySpace(SUB, entity) * EntityDistance;
		size.cy = sub_iter->y.cy;
		p.y = size;
		p.name = entity.subscriber;
		p.entity = entity;
		p.index = lstSubPosition.size();

		if (Comapare_Reduplication(lstSubPosition, p))
			lstSubPosition.push_back(p);
	}
}

/**************
* Purpose: Calculate Pub position in one DP
**************/
void RelationshipView::Cal_PubPosition(int nprocess, int ndp, int npub, entity entity)
{
	position p;
	CSize size;
	int nDW = Cnt_DWinPub(entity.domainId, entity.host, entity.process, entity.domainParticipant, entity.publisher) ;
	int width_DW = nDW * EntityWidth;
	int nProgression = 2 * nprocess + ndp - 2; //a linear equation (nprocess, ndp) -> npub

	if (npub == 0)
	{
		list<position>::iterator dp_iter;
		dp_iter = --lstDPPosition.end();

		size.cx = dp_iter->x.cx + EntityDistance;
		size.cy = dp_iter->x.cy + EntityDistance;
		p.x = size;

		size.cx = size.cx + width_DW + Cal_EmptySpace(PUB, entity) * EntityDistance;
		size.cy = dp_iter->y.cy - EntityDistance;
		p.y = size;
		p.name = entity.publisher;
		p.entity = entity;
		p.index = lstPubPosition.size();
		
		if (Comapare_Reduplication(lstPubPosition, p))
			lstPubPosition.push_back(p);
	}
	else
	{
		list<position>::iterator pub_iter;
		pub_iter = --lstPubPosition.end();

		size.cx = pub_iter->y.cx + EntityDistance;
		size.cy = pub_iter->x.cy;
		p.x = size;

		size.cx = size.cx + width_DW + Cal_EmptySpace(PUB, entity) * EntityDistance;
		size.cy = pub_iter->y.cy;
		p.y = size;
		p.name = entity.publisher;
		p.entity = entity;
		p.index = lstPubPosition.size();

		if (Comapare_Reduplication(lstPubPosition, p))
			lstPubPosition.push_back(p);
	}
}

/**************
* Purpose: Calculate DP position in one Process
**************/
void RelationshipView::Cal_DPPosition(int nprocess, int ndp, entity entity)
{
	position p;
	CSize size;
	int nRW = Cnt_RWinDP(entity.domainId, entity.host, entity.process, entity.domainParticipant);
	int width_RW = nRW * EntityWidth;
	int nT = Cnt_TinDP(entity.domainId, entity.host, entity.process, entity.domainParticipant);
	int nEmpty = Cal_EmptySpace(DOMAINPARTICIPANT, entity);
	int width_topic = nT * EntityWidth;
	

	if (ndp == 0)
	{
		list<position>::iterator process_iter;
		process_iter = --lstProcessPosition.end();

		size.cx = process_iter->x.cx + EntityDistance;
		size.cy = process_iter->x.cy + EntityDistance;
		p.x = size;

		size.cx = size.cx + width_RW + nEmpty * EntityDistance + width_topic;
		size.cy = process_iter->y.cy - EntityDistance;
		p.y = size;
		p.name = entity.domainParticipant;
		p.entity = entity;
		p.index = lstDPPosition.size();

		if (Comapare_Reduplication(lstDPPosition, p))
			lstDPPosition.push_back(p);
	}
	else
	{
		list<position>::iterator dp_iter;
		dp_iter = --lstDPPosition.end();

		size.cx = dp_iter->y.cx + EntityDistance;
		size.cy = dp_iter->x.cy;
		p.x = size;

		size.cx = size.cx + width_RW + Cal_EmptySpace(DOMAINPARTICIPANT, entity) * EntityDistance + width_topic;
		size.cy = dp_iter->y.cy;
		p.y = size;
		p.name = entity.domainParticipant;
		p.entity = entity;
		p.index = lstDPPosition.size();

		if (Comapare_Reduplication(lstDPPosition, p))
			lstDPPosition.push_back(p);
	}
}

/**************
* Purpose: Calculate Process position in one Host
**************/
void RelationshipView::Cal_ProcessPosition(int nhost, int nprocess, entity entity)
{
	position p;
	CSize size;
	int nRW = Cnt_RWinP(entity.domainId, entity.host, entity.process);
	int nT = Cnt_TinP(entity.domainId, entity.host, entity.process);
	int width_topic = nT * EntityWidth;
	int nPS = Cnt_PSinP(entity.domainId, entity.host, entity.process);
	int nDP = Cnt_DPinP(entity.domainId, entity.host, entity.process);

	if (nprocess == 0)
	{
		list<position>::iterator host_iter;
		host_iter = --lstHostPosition.end();

		size.cx = host_iter->x.cx + EntityDistance;
		size.cy = host_iter->x.cy + EntityDistance;
		p.x = size;

		size.cx = size.cx + nRW * EntityWidth + Cal_EmptySpace(PROCESS, entity) * EntityDistance + width_topic;
		size.cy = host_iter->y.cy - EntityDistance;
		p.y = size;
		p.name = entity.process;
		p.entity = entity;
		p.index = lstProcessPosition.size();

		if (Comapare_Reduplication(lstProcessPosition, p))
			lstProcessPosition.push_back(p);
	}
	else
	{
		list<position>::iterator process_iter;
		process_iter = --lstProcessPosition.end();

		size.cx = process_iter->y.cx + EntityDistance;
		size.cy = process_iter->x.cy;
		p.x = size;

		size.cx = size.cx + nRW * EntityWidth + Cal_EmptySpace(PROCESS, entity) * EntityDistance + width_topic;
		size.cy = process_iter->y.cy;
		p.y = size;
		p.name = entity.process;
		p.entity = entity;
		p.index = lstProcessPosition.size();

		if (Comapare_Reduplication(lstProcessPosition, p))
			lstProcessPosition.push_back(p);
	}
}

/**************
* Purpose: Calculate Host position in one Domain
**************/
void RelationshipView::Cal_HostPosition(int nhost, entity entity)
{
	position p;
	CSize size;
	int nRW = Cnt_TotalDR(entity.domainId, entity.host) + Cnt_TotalDW(entity.domainId, entity.host);
	int nPS = Cnt_TotalPub(entity.domainId, entity.host) + Cnt_TotalSub(entity.domainId, entity.host);
	int nP = Cnt_TotalProcess(entity.domainId, entity.host);
	int nT = Cnt_TotalTopic(entity.domainId, entity.host);
	int nDP = Cnt_TotalDP(entity.domainId, entity.host);
	int totaltopicsize = nT*EntityWidth;

	if (nhost == 0)
	{
		size.cx = EntityDistance;
		size.cy = EntityDistance;
		p.x = size;

		size.cx = nRW * EntityWidth + Cal_EmptySpace(HOST, entity) * EntityDistance + totaltopicsize;
		size.cy = EntityDistance + 10 * EntityDistance;
		p.y = size;
		p.name = entity.host;
		p.entity = entity;
		p.index = lstHostPosition.size();
		
		if (Comapare_Reduplication(lstHostPosition, p))
		{
			lstHostPosition.push_back(p);
		}
	}
	else
	{
		list<position>::iterator iter;
		iter = --lstHostPosition.end();

		size.cx = iter->x.cx;
		size.cy = iter->y.cy + 40;
		p.x = size;
		
		size.cx = nRW * EntityWidth + Cal_EmptySpace(HOST, entity) * EntityDistance + totaltopicsize;
		size.cy = iter->y.cy + 40 + 10 * EntityDistance;
		p.y = size;
		p.name = entity.host;
		p.entity = entity;
		p.index = lstHostPosition.size();

		if (Comapare_Reduplication(lstHostPosition, p))
		{
			lstHostPosition.push_back(p);
		}
	}
}

/**************
* Purpose: Calculate emptyspace(size) in one entity
**************/
int RelationshipView::Cal_EmptySpace(int kind, entity entity)
{
	switch (kind)
	{
	case HOST:
		return 2 + 2*Cnt_TotalProcess(entity.domainId, entity.host) + Cnt_TotalDP(entity.domainId, entity.host)*2 + 2*(Cnt_TotalPub(entity.domainId, entity.host) + Cnt_TotalSub(entity.domainId, entity.host)) + Cnt_TotalTopic(entity.domainId, entity.host) + Cnt_TotalDW(entity.domainId, entity.host) + Cnt_TotalDR(entity.domainId, entity.host);
		break;
	case PROCESS:
		return 1 + Cnt_DPinP(entity.domainId, entity.host, entity.process)*2 +Cnt_PSinP(entity.domainId, entity.host, entity.process)*2 + Cnt_RWinP(entity.domainId, entity.host, entity.process) + Cnt_TinP(entity.domainId, entity.host, entity.process);
		break;
	case DOMAINPARTICIPANT:
		return 1 + Cnt_PSinDP(entity.domainId, entity.host, entity.process, entity.domainParticipant) * 2 + Cnt_RWinDP(entity.domainId, entity.host, entity.process, entity.domainParticipant) + Cnt_TinDP(entity.domainId, entity.host, entity.process, entity.domainParticipant);
		break;
	case PUB:
		return 1 + Cnt_DWinPub(entity.domainId, entity.host, entity.process, entity.domainParticipant, entity.publisher);
		break;
	case SUB:
		return 1 + Cnt_DRinSub(entity.domainId, entity.host, entity.process, entity.domainParticipant, entity.subscriber);
		break;
	default:
		break;
	}
}

/**************
* Purpose: Calculate total size of Topic in one Host
**************/
int RelationshipView::Cal_TotalTopicSize(string host)
{
	int ntt = 0;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->host.compare(host) == 0)
		{
			if (i->topicName.length() > 0)
				/*ntt = ntt + Cal_RWTSize(i->topicName);*/
				ntt++;
		}
	}
	return ntt * EntityWidth;
}

/**************
* Purpose: Count the number of Topic in one Host
**************/
int RelationshipView::Cnt_TotalTopic(string d, string h)
{
	list<string> ts;
	list<string> temp;
	list<string> ps = Save_SortedProcess(d, h);
	bool updated = false;
	for (list<string>::iterator j = ps.begin(); j != ps.end(); j++)
	{
		list<string> dps = Save_SortedDP(d, h, *j);
		for (list<string>::iterator k = dps.begin(); k != dps.end(); k++)
		{
			for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
			{
				if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(*j) == 0 && i->domainParticipant.compare(*k) == 0)
				{
					if (i->topicName.length() > 0)
					{
						if (Comapare_Reduplication(temp, i->topicName))
						{
							temp.push_back(i->topicName);
							updated = true;
						}
					}
				}
			}
			if (updated)
			{
				if (temp.size() > 0)
				{
					ts.sort();
					temp.sort();
					ts.merge(temp);
					updated = false;
					temp.clear();
				}
			}
		}
	}

	return ts.size();
}


/**************
* Purpose: Count the number of DR in one Host
**************/
int RelationshipView::Cnt_TotalDR(string d, string h)
{
	list<string> temp;
	list<string> drs;
	list<string> ps = Save_SortedProcess(d, h);
	bool updated = false;
	for (list<string>::iterator j = ps.begin(); j != ps.end(); j++)
	{
		list<string> dps = Save_SortedDP(d, h, *j);
		for (list<string>::iterator k = dps.begin(); k != dps.end(); k++)
		{
			list<string> subs = Save_SortedSub(d, h, *j, *k);
			for (list<string>::iterator l = subs.begin(); l != subs.end(); l++)
			{
				for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
				{
					if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(*j) == 0 && i->domainParticipant.compare(*k) == 0 && i->subscriber.compare(*l) == 0)
					{
						if (i->dataReader.length() > 0)
						{
							if (Comapare_Reduplication(temp, i->dataReader))
							{
								temp.push_back(i->dataReader);
								updated = true;
							}
						}
					}
				}
				if (updated)
				{
					if (temp.size() > 0)
					{
						drs.sort();
						temp.sort();
						drs.merge(temp);
						updated = false;
						temp.clear();
					}
				}
			}
		}
	}

	return drs.size();
}

/**************
* Purpose: Count the number of DW in one Host
**************/
int RelationshipView::Cnt_TotalDW(string d, string h)
{
	list<string> temp;
	list<string> dws;
	list<string> ps = Save_SortedProcess(d, h);
	bool updated = false;
	for (list<string>::iterator j = ps.begin(); j != ps.end(); j++)
	{
		list<string> dps = Save_SortedDP(d, h, *j);
		for (list<string>::iterator k = dps.begin(); k != dps.end(); k++)
		{
			list<string> pubs = Save_SortedPub(d, h, *j, *k);
			for (list<string>::iterator l = pubs.begin(); l != pubs.end(); l++)
			{
				for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
				{
					if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(*j) == 0 && i->domainParticipant.compare(*k) == 0 && i->publisher.compare(*l) == 0)
					{
						if (i->dataWriter.length() > 0)
						{
							if (Comapare_Reduplication(temp, i->dataWriter))
							{
								temp.push_back(i->dataWriter);
								updated = true;
							}
						}
					}
				}
				if (updated)
				{
					if (temp.size() > 0)
					{
						dws.sort();
						temp.sort();
						dws.merge(temp);
						updated = false;
						temp.clear();
					}
				}
			}
		}
	}

	return dws.size();
}

/**************
* Purpose: Count the number of Sub in one Host
**************/
int RelationshipView::Cnt_TotalSub(string d, string h)
{
	list<string> temp;
	list<string> subs;
	list<string> ps = Save_SortedProcess(d, h);
	bool updated = false;
	for (list<string>::iterator j = ps.begin(); j != ps.end(); j++)
	{
		list<string> dps = Save_SortedDP(d, h, *j);
		for (list<string>::iterator k = dps.begin(); k != dps.end(); k++)
		{
			for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
			{
				if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(*j) == 0 && i->domainParticipant.compare(*k) == 0)
				{
					if (i->subscriber.length() > 0)
					{
						if (Comapare_Reduplication(temp, i->subscriber))
						{
							temp.push_back(i->subscriber);
							updated = true;
						}
					}
				}
			}
			if (updated)
			{
				if (temp.size() > 0)
				{
					subs.sort();
					temp.sort();
					subs.merge(temp);
					updated = false;
					temp.clear();
				}
			}
		}
	}

	return subs.size();
}

/**************
* Purpose: Count the number of Pub in one Host
**************/
int RelationshipView::Cnt_TotalPub(string d, string h)
{
	list<string> temp;
	list<string> pubs;
	list<string> ps = Save_SortedProcess(d, h);
	bool updated = false;
	for (list<string>::iterator j = ps.begin(); j != ps.end(); j++)
	{
		list<string> dps = Save_SortedDP(d, h, *j);
		for (list<string>::iterator k = dps.begin(); k != dps.end(); k++)
		{
			for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
			{
				if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(*j) == 0 && i->domainParticipant.compare(*k) == 0)
				{
					if (i->publisher.length() > 0)
					{
						if (Comapare_Reduplication(temp, i->publisher))
						{
							temp.push_back(i->publisher);
							updated = true;
						}
					}
				}
			}
			if (updated)
			{
				if (temp.size() > 0)
				{
					pubs.sort();
					temp.sort();
					pubs.merge(temp);
					updated = false;
					temp.clear();
				}
			}
		}
	}

	return pubs.size();
}

/**************
* Purpose: Count the number of DP in one Host
**************/
int RelationshipView::Cnt_TotalDP(string d, string h)
{
	list<string> temp;
	list<string> dps;
	list<string> ps = Save_SortedProcess(d, h);
	bool updated = false;
	for (list<string>::iterator j = ps.begin(); j != ps.end(); j++)
	{
		for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
		{
			if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(*j) == 0)
			{
				if (i->domainParticipant.length() > 0)
				{
					if (Comapare_Reduplication(temp, i->domainParticipant))
					{
						temp.push_back(i->domainParticipant);
						updated = true;
					}
				}
			}
		}
		if (updated)
		{
			if (temp.size()>0)
			{
				dps.sort();
				temp.sort();
				dps.merge(temp);
				updated = false;
				temp.clear();
			}
			
		}
	}

	return dps.size();
}

/**************
* Purpose: Count the number of Process in one Host
**************/
int RelationshipView::Cnt_TotalProcess(string d, string h)
{
	list<string> ps;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0)
			if (i->process.length() > 0)
				if(Comapare_Reduplication(ps,i->process))
					ps.push_back(i->process);
	}

	return ps.size();
}

/**************
* Purpose: Count the number of Topic in one Process
**************/
int RelationshipView::Cnt_TinP(string d, string h, string p)
{
	list<string> temp;
	list<string> ts;
	list<string> dps = Save_SortedDP(d, h, p);
	bool updated = false;
	for (list<string>::iterator k = dps.begin(); k != dps.end(); k++)
	{
		for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
		{
			if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(*k) == 0)
			{
				if (i->topicName.length() > 0)
				{
					if (Comapare_Reduplication(temp, i->topicName))
					{
						temp.push_back(i->topicName);
						updated = true;
					}
				}
			}
		}
		if (updated)
		{
			if (temp.size() > 0) 
			{
				ts.sort();
				temp.sort();
				ts.merge(temp);
				updated = false;
				temp.clear();
			}
		}
	}

	return ts.size();
}

/**************
* Purpose: Count the number of Topic in one DP
**************/
int RelationshipView::Cnt_TinDP(string d, string h, string p, string dp)
{
	list<string> ts;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0)
		{
			if (i->topicName.length() > 0)
			{
				if (Comapare_Reduplication(ts, i->topicName))
				{
					ts.push_back(i->topicName);
				}
			}
		}
	}

	return ts.size();
}

/**************
* Purpose: Count the number of RW in one Process
**************/
int RelationshipView::Cnt_RWinP(string d, string h, string p)
{
	list<string> temp;
	list<string> temp1;
	list<string> dws;
	list<string> drs;
	bool updated = false;
	bool updated1 = false;
	list<string> dps = Save_SortedDP(d, h, p);
	for (list<string>::iterator k = dps.begin(); k != dps.end(); k++)
	{
		list<string> pubs = Save_SortedPub(d, h, p, *k);
		for (list<string>::iterator l = pubs.begin(); l != pubs.end(); l++)
		{
			for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
			{
				if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(*k) == 0 && i->publisher.compare(*l) == 0)
				{
					if (i->dataWriter.length() > 0)
					{
						if (Comapare_Reduplication(temp1, i->dataWriter))
						{
							temp1.push_back(i->dataWriter);
							updated1 = true;
						}
					}
				}
			}
			if (updated1)
			{
				if (temp1.size() > 0)
				{
					dws.sort();
					temp1.sort();
					dws.merge(temp1);
					updated1 = false;
					temp1.clear();
				}
			}
		}

		list<string> subs = Save_SortedSub(d, h, p, *k);
		for (list<string>::iterator l = subs.begin(); l != subs.end(); l++)
		{
			for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
			{
				if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(*k) == 0 && i->subscriber.compare(*l) == 0)
				{
					if (i->dataReader.length() > 0)
					{
						if (Comapare_Reduplication(temp, i->dataReader))
						{
							temp.push_back(i->dataReader);
							updated = true;
						}
					}
				}
			}
			if (updated)
			{
				if (temp.size() > 0)
				{
					drs.sort();
					temp.sort();
					drs.merge(temp);
					updated = false;
					temp.clear();
				}
			}
		}
	}
	return drs.size() + dws.size();
}

/**************
* Purpose: Count the number of DR in one Sub
**************/
int RelationshipView::Cnt_DRinSub(string d, string h, string p, string dp, string sub)
{
	int cnt = 0;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0 && i->subscriber.compare(sub) == 0)
		{
			if (i->dataReader.length() > 0)
			{
				cnt++;
			}
		}

	}
	return cnt;
}

/**************
* Purpose: Count the number of DW in one Pub
**************/
int RelationshipView::Cnt_DWinPub(string d, string h, string p, string dp, string pub)
{
	int cnt = 0;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0 && i->publisher.compare(pub) == 0)
		{
			if (i->dataWriter.length()>0)
			{
				cnt++;
			}
		}

	}
	return cnt;
}

/**************
* Purpose: Count the number of RW in one DP
**************/
int RelationshipView::Cnt_RWinDP(string d, string h, string p, string dp)
{
	list<string> temp;
	list<string> temp1;
	list<string> dws;
	list<string> drs;
	bool updated = false;
	bool updated1 = false;
	
	list<string> pubs = Save_SortedPub(d, h, p, dp);
	for (list<string>::iterator l = pubs.begin(); l != pubs.end(); l++)
	{
		for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
		{
			if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0 && i->publisher.compare(*l) == 0)
			{
				if (i->dataWriter.length() > 0)
				{
					if (Comapare_Reduplication(temp1, i->dataWriter))
					{
						temp1.push_back(i->dataWriter);
						updated1 = true;
					}
				}
			}
		}
		if (updated1)
		{
			if (temp1.size() > 0)
			{
				dws.sort();
				temp1.sort();
				dws.merge(temp1);
				updated1 = false;
				temp1.clear();
			}
		}
	}

	list<string> subs = Save_SortedSub(d, h, p, dp);
	for (list<string>::iterator l = subs.begin(); l != subs.end(); l++)
	{
		for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
		{
			if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0 && i->subscriber.compare(*l) == 0)
			{
				if (i->dataReader.length() > 0)
				{
					if (Comapare_Reduplication(temp, i->dataReader))
					{
						temp.push_back(i->dataReader);
						updated = true;
					}
				}
			}
		}
		if (updated)
		{
			if (temp.size() > 0)
			{
				drs.sort();
				temp.sort();
				drs.merge(temp);
				updated = false;
				temp.clear();
			}
		}
	}
	return drs.size() + dws.size();
}

/**************
* Purpose: Count the number of PS in one Process
**************/
int RelationshipView::Cnt_PSinP(string d, string h, string p)
{
	list<string> temp;
	list<string> temp1;
	list<string> pubs;
	list<string> subs;
	bool updated = false;
	bool updated1 = false;

	list<string> dps = Save_SortedDP(d, h, p);
	for (list<string>::iterator k = dps.begin(); k != dps.end(); k++)
	{
		for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
		{
			if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(*k) == 0)
			{
				if (i->publisher.length() > 0)
				{
					if (Comapare_Reduplication(temp, i->publisher))
					{
						temp.push_back(i->publisher);
						updated = true;
					}
				}
				else
				{
					if (Comapare_Reduplication(temp1, i->subscriber))
					{
						temp1.push_back(i->subscriber);
						updated1 = true;
					}
				}
			}
		}
		if (updated)
		{
			if (temp.size() > 0)
			{
				pubs.sort();
				temp.sort();
				pubs.merge(temp);
				updated = false;
				temp.clear();
			}
		}
		if (updated1)
		{
			if (temp1.size() > 0)
			{
				subs.sort();
				temp1.sort();
				subs.merge(temp1);
				updated1 = false;
				temp1.clear();
			}
		}
	}
	return pubs.size() + subs.size();
}

/**************
* Purpose: Count the number of PS in one DP
**************/
int RelationshipView::Cnt_PSinDP(string d, string h, string p, string dp)
{
	return Cnt_PinDP(d, h, p, dp) + Cnt_SinDP(d, h, p, dp);
}

int RelationshipView::Cnt_PinDP(string d, string h, string p, string dp)
{
	list<string> pubs;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0)
		{
			if (i->publisher.length() > 0)
				if(Comapare_Reduplication(pubs, i->publisher))
					pubs.push_back(i->publisher);
		}
	}
	return pubs.size();
}

/**************
* Purpose: Count the number of Sub in one DP
**************/
int RelationshipView::Cnt_SinDP(string d, string h, string p, string dp)
{
	list<string> subs;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0)
		{
			if (i->subscriber.length() > 0)
				if (Comapare_Reduplication(subs, i->subscriber))
					subs.push_back(i->subscriber);
		}
	}
	return subs.size();
}

/**************
* Purpose: Count the number of DP in one Process
**************/
int RelationshipView::Cnt_DPinP(string d, string h, string p)
{
	return Save_SortedDP(d, h, p).size();
}

/**************
* Purpose: Count the number of Process in one Host
**************/
int RelationshipView::Cnt_PinH(string d, string h)
{
	return Save_SortedProcess(d, h).size();
}

/**************
* Purpose: Count the number of Host in one Domain
**************/
int RelationshipView::Cnt_HinD(string d)
{
	return Save_SortedHost(d).size();
}


/**************
* Purpose: Save entityset from mainsystem in RV
**************/
list<entity> RelationshipView::Get_EntitySet()
{
	list<entity> lstEntities;

	MainSystem * m = MainSystem::GetInstance();

	unordered_map<int, entity>::iterator i;

	for (i = m->currentEntitySet.begin(); i != m->currentEntitySet.end(); i++) {
		lstEntities.push_back(i->second);
	}
	return lstEntities;
}

/**************
* Purpose: Save Host list in one Domain
**************/
list<string> RelationshipView::Save_SortedHost(string d)
{
	list<string> hs;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0)
		{
			if (i->host.length() > 0)
			{
				if (Comapare_Reduplication(hs, i->host))
				{
					hs.push_back(i->host);
				}
			}
		}
	}

	return hs;
}


/**************
* Purpose: Save Process list in one Host
**************/
list<string> RelationshipView::Save_SortedProcess(string d, string h)
{
	list<string> ps;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0)
		{
			if (i->host.compare(h) == 0)
			{
				if (i->process.length() > 0)
				{
					if (Comapare_Reduplication(ps, i->process))
					{
						ps.push_back(i->process);
					}
				}
			}
		}
	}
	//ps = Del_Reduplication(ps);

	return ps;
}

/**************
* Purpose: Check whether same string is or not
**************/
bool RelationshipView::Comapare_Reduplication(list<string> a, string b)
{
	bool bIS = true;
	for (list<string>::iterator j = a.begin(); j != a.end(); j++)
	{
		if (b.compare(*j) == 0)
		{
			return false;
		}
	}
	return bIS;
}

/**************
* Purpose: Check whether same position is or not
**************/
bool RelationshipView::Comapare_Reduplication(list<position> a, position p)
{
	bool bIS = true;
	for (list<position>::iterator j = a.begin(); j != a.end(); j++)
	{
		if (p.x == j->x)
		{
			if (p.y == j->y)
			{
				return false;
			}
		}
	}
	return bIS;
}


/**************
* Purpose: Save DP list in one Process
**************/
list<string> RelationshipView::Save_SortedDP(string d, string h, string p)
{
	list<string> dps;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0)
		{
			if (i->domainParticipant.length() > 0)
			{
				if (Comapare_Reduplication(dps, i->domainParticipant))
				{
					dps.push_back(i->domainParticipant);
				}
			}
		}
	}

	return dps;
}

/**************
* Purpose: Save Topic list in one DP
**************/
list<string> RelationshipView::Save_SortedTopic(string d, string h, string p, string dp)
{
	list<string> topic;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0)
		{
			if (i->topicName.length() > 0)
			{
				if (Comapare_Reduplication(topic, i->topicName))
				{
					topic.push_back(i->topicName);
				}
			}
		}
	}

	return topic;
}

/**************
* Purpose: Save Pub list in one DP
**************/
list<string> RelationshipView::Save_SortedPub(string d, string h, string p, string dp)
{
	list<string> pub;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0)
		{
			if (i->publisher.length() > 0)
			{
				if (Comapare_Reduplication(pub, i->publisher))
				{
					pub.push_back(i->publisher);
				}
			}
		}
	}

	return pub;
}

/**************
* Purpose: Save Sub list in one DP
**************/
list<string> RelationshipView::Save_SortedSub(string d, string h, string p, string dp)
{
	list<string> sub;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0)
		{
			if (i->subscriber.length() > 0)
			{
				if (Comapare_Reduplication(sub, i->subscriber))
				{
					sub.push_back(i->subscriber);
				}
			}
		}
	}

	return sub;
}

/**************
* Purpose: Save DW list in one Pub
**************/
list<string> RelationshipView::Save_SortedDW(string d, string h, string p, string dp, string pub)
{
	list<string> dw;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0 && i->publisher.compare(pub) == 0)
		{
			if (i->dataWriter.length() > 0)
			{
				if (Comapare_Reduplication(dw, i->dataWriter))
				{
					dw.push_back(i->dataWriter);
				}
			}
		}
	}

	return dw;
}

/**************
* Purpose: Save DW list in one Pub
**************/
list<string> RelationshipView::Save_SortedDR(string d, string h, string p, string dp, string sub)
{
	list<string> dr;
	for (list<entity>::iterator i = lstEntitySet.begin(); i != lstEntitySet.end(); i++)
	{
		if (i->domainId.compare(d) == 0 && i->host.compare(h) == 0 && i->process.compare(p) == 0 && i->domainParticipant.compare(dp) == 0 && i->subscriber.compare(sub) == 0)
		{
			if (i->dataReader.length() > 0)
			{
				if (Comapare_Reduplication(dr, i->dataReader))
				{
					dr.push_back(i->dataReader);
				}
			}
		}
	}

	return dr;
}


/**************
* Purpose: edit position content
**************/
void RelationshipView::Edit_Position(int kind, position p)
{
	list<position>::iterator i;

	switch (kind)
	{
	case HOST:
		for (i = lstHostPosition.begin(); i != lstHostPosition.end(); i++)
		{
			if (i->index == p.index)
			{
				i->bclick = p.bclick;
				i->entity = p.entity;
				i->index = p.index;
				i->x = p.x;
				i->y = p.y;
				i->name = p.name;
			}
		}
		break;
	case PROCESS:
		for (i = lstProcessPosition.begin(); i != lstProcessPosition.end(); i++)
		{
			if (i->index == p.index)
			{
				i->bclick = p.bclick;
				i->entity = p.entity;
				i->index = p.index;
				i->x = p.x;
				i->y = p.y;
				i->name = p.name;
			}
		}
		break;
	case DOMAINPARTICIPANT:
		for (i = lstDPPosition.begin(); i != lstDPPosition.end(); i++)
		{
			if (i->index == p.index)
			{
				i->bclick = p.bclick;
				i->entity = p.entity;
				i->index = p.index;
				i->x = p.x;
				i->y = p.y;
				i->name = p.name;
			}
		}
		break;
	case PUB:
		for (i = lstPubPosition.begin(); i != lstPubPosition.end(); i++)
		{
			if (i->index == p.index)
			{
				i->bclick = p.bclick;
				i->entity = p.entity;
				i->index = p.index;
				i->x = p.x;
				i->y = p.y;
				i->name = p.name;
			}
		}
		break;
	case SUB:
		for (i = lstSubPosition.begin(); i != lstSubPosition.end(); i++)
		{
			if (i->index == p.index)
			{
				i->bclick = p.bclick;
				i->entity = p.entity;
				i->index = p.index;
				i->x = p.x;
				i->y = p.y;
				i->name = p.name;
			}
		}
		break;
	case DATAWRITER:
		for (i = lstDWPosition.begin(); i != lstDWPosition.end(); i++)
		{
			if (i->index == p.index)
			{
				i->bclick = p.bclick;
				i->entity = p.entity;
				i->index = p.index;
				i->x = p.x;
				i->y = p.y;
				i->name = p.name;
			}
		}
		break;
	case DATAREADER:
		for (i = lstDRPosition.begin(); i != lstDRPosition.end(); i++)
		{
			if (i->index == p.index)
			{
				i->bclick = p.bclick;
				i->entity = p.entity;
				i->index = p.index;
				i->x = p.x;
				i->y = p.y;
				i->name = p.name;
			}
		}
		break;
	case TOPIC:
		for (i = lstTopicPosition.begin(); i != lstTopicPosition.end(); i++)
		{
			if (i->index == p.index)
			{
				i->bclick = p.bclick;
				i->entity = p.entity;
				i->index = p.index;
				i->x = p.x;
				i->y = p.y;
				i->name = p.name;
			}
		}
	default:
		break;
	}
}


/**************
* Purpose: find which position it is, when u wanna re-draw one entity
**************/
position RelationshipView::Find_Position(int kind, entity e)
{
	list<position>::iterator i;
	switch (kind)
	{
	case HOST:
		for (i = lstHostPosition.begin(); i != lstHostPosition.end(); i++)
		{
			if (e.host.compare(i->entity.host) == 0)
			{
				return *i;
			}
		}
		break;
	case PROCESS:
		for (i = lstProcessPosition.begin(); i != lstProcessPosition.end(); i++)
		{
			if (e.host.compare(i->entity.host) == 0 && e.process.compare(i->entity.process) == 0)
			{
				return *i;
			}
		}
		break;
	case DOMAINPARTICIPANT:
		for (i = lstDPPosition.begin(); i != lstDPPosition.end(); i++)
		{
			if (e.host.compare(i->entity.host) == 0 && e.process.compare(i->entity.process) == 0 && e.domainParticipant.compare(i->entity.domainParticipant) == 0)
			{
				return *i;
			}
		}
		break;
	case PUB:
		for (i = lstPubPosition.begin(); i != lstPubPosition.end(); i++)
		{
			if (e.host.compare(i->entity.host) == 0 && e.process.compare(i->entity.process) == 0 && e.domainParticipant.compare(i->entity.domainParticipant) == 0 && e.publisher.compare(i->entity.publisher) == 0)
			{
				return *i;
			}
		}
		break;
	case SUB:
		for (i = lstSubPosition.begin(); i != lstSubPosition.end(); i++)
		{
			if (e.host.compare(i->entity.host) == 0 && e.process.compare(i->entity.process) == 0 && e.domainParticipant.compare(i->entity.domainParticipant) == 0 && e.subscriber.compare(i->entity.subscriber) == 0)
			{
				return *i;
			}
		}
		break;
	case DATAWRITER:
		for (i = lstDWPosition.begin(); i != lstDWPosition.end(); i++)
		{
			if (e.host.compare(i->entity.host) == 0 && e.process.compare(i->entity.process) == 0 && e.domainParticipant.compare(i->entity.domainParticipant) == 0 && e.publisher.compare(i->entity.publisher) == 0 && e.dataWriter.compare(i->entity.dataWriter) == 0)
			{
				return *i;
			}
		}
		break;
	case DATAREADER:
		for (i = lstDRPosition.begin(); i != lstDRPosition.end(); i++)
		{
			if (e.host.compare(i->entity.host) == 0 && e.process.compare(i->entity.process) == 0 && e.domainParticipant.compare(i->entity.domainParticipant) == 0 && e.subscriber.compare(i->entity.subscriber) == 0 && e.dataReader.compare(i->entity.dataReader) == 0)
			{
				return *i;
			}
		}
	case TOPIC:
		for (i = lstTopicPosition.begin(); i != lstTopicPosition.end(); i++)
		{
			if (e.host.compare(i->entity.host) == 0 && e.process.compare(i->entity.process) == 0 && e.domainParticipant.compare(i->entity.domainParticipant) == 0 && e.topicName.compare(i->entity.topicName) == 0)
			{
				return *i;
			}
		}
	default:
		break;
	}


}

/**************
* Purpose: Calculate size of Topic, RW depend on string length
**************/
int RelationshipView::Cal_RWTSize(string dwdrt)
{
	int size;
	size = dwdrt.length() * 3;

	return size;
}

/**************
* Purpose: Get instance this class
**************/
RelationshipView* RelationshipView::GetInstance()
{
	if (instance == NULL) instance = new RelationshipView();
	return instance;
}
