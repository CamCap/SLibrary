#include "stdafx.h"
#include "SServerContainer.h"
#include <algorithm>

//
//SServerContainer* SServerContainer::m_instance = NULL;
//
//SServerContainer::SServerContainer()
//	:SPeerContainer<SServer*>()
//{
//}
//
//
//SServerContainer::~SServerContainer()
//{
//}
//
//SServerContainer* SServerContainer::GetInstance()
//{
//	if (m_instance == NULL)
//		m_instance = new SServerContainer();
//
//	return m_instance;
//}
//
//void SServerContainer::ServerCheckPing(DWORD tick)
//{
//	if (tick - m_tickPing < PING_CHECK_TIME)
//		return;
//
//	m_tickPing = tick;
//
//	MAP_CONTANINER::iterator it = m_mapCon.begin();
//
//	SServer* _server = NULL;
//
//	for (it; it != m_mapCon.end(); it++)
//	{
//		_server = it->second;
//		if (_server == NULL)
//			continue;
//
//
//		_server->OnPingCheck(m_tickPing);
//	}
//}
//
//template<typename type>
//inline bool SServerContainer::IsCurrentServer(type _type)
//{
//	SServer* _server = NULL;
//
//	CSLOCK(m_cs)
//	{
//		MAP_CONTANINER::iterator it = std::find_if(m_mapCon.begin(), m_mapCon.end(), \
//			[&](MAP_CONTANINER::value_type itr)->bool {return itr.second->GetType() == _type; }\
//		);
//
//		if (it != m_mapCon.end())
//			return true;
//		else
//			return false;
//	}
//	return false;
//}
//
////
////void SServerContainer::AuthServer(SServer * server)
////{
////	if (IsCurrentServer(server->GetType()) == false)
////	{
////		AddAuthServer(server->GetId(), server);
////	}
////}
//
//SServer * SServerContainer::FindServer(int id)
//{
//	SServer * server = FindServer(id);
//
//	if (server != NULL) return server;
//
//	CSLOCK(m_cs)
//	{
//		////���� ����� ���� ��Ͽ��� �����...
//		//V::iterator it;
//		VEC_CONTANINER::iterator it = \
//			find_if(m_vecRogueServer.begin(), m_vecRogueServer.end(), \
//				[&](SServer* server)->bool { return id == server->GetId(); } \
//			);
//
//		if (it != m_vecRogueServer.end())
//			server = *it;
//		else
//			server = NULL;
//	}
//
//	return server;
//}
//
//void SServerContainer::PushRogueServer(SServer * server)
//{
//	if (server == NULL) return;
//
//	CSLOCK(m_cs)
//	{
//		VEC_CONTANINER::iterator it = std::find(m_vecRogueServer.begin(), m_vecRogueServer.end(), server);
//
//		if (it != m_vecRogueServer.end())
//			return;
//
//		m_vecRogueServer.push_back(server);
//	}
//}
//
//void SServerContainer::PopRogueServer()
//{
//	if (m_vecRogueServer.size() == 0)
//	{
//		//UnLock();
//		return;
//	}
//
//	SServer* value = NULL;
//
//	CSLOCK(m_cs)
//	{
//		VEC_CONTANINER::iterator it = m_vecRogueServer.begin();
//
//		value = *it;
//		m_vecRogueServer.erase(it);
//	}
//}
//
//
//
