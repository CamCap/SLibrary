#pragma once
#include "ssocket.h"
#include "Container.h"
#include <functional>

#define SERVER_SIZE 10

template <typename T, typename type>
class SServerContainer
	:public Container<T*>
{
	using VEC_CONTANINER = typename std::vector<T*>;
	using MAP_CONTANINER = typename std::map<int, T*>;

public:
	static SServerContainer* GetInstance();


	void ServerCheckPing(DWORD tickcount, std::function<void(DWORD)> checkping) {
		if (tick - m_tickPing < PING_CHECK_TIME)
			return;
			
		m_tickPing = tick;
	
		MAP_CONTANINER::iterator it = m_mapCon.begin();
	
		T* _server = NULL;
	
		for (it; it != m_mapCon.end(); it++)
		{
			_server = it->second;
			if (_server == NULL)
				continue;
	
			checkping(tickcount);
		}
	}

	//bool IsCurrentServer(type _type) {
	//	T* _server = NULL;
	//	
	//	CSLOCK(m_cs)
	//	{
	//		MAP_CONTANINER::iterator it = std::find_if(m_mapCon.begin(), m_mapCon.end(), \
	//			[&](MAP_CONTANINER::value_type itr)->bool {return itr.second->GetType() == _type; }\
	//		);

	//		if (it != m_mapCon.end())
	//			return true;
	//		else
	//			return false;
	//	}
	//	
	//	return false;
	//}

	constexpr T* FindServer(int id){	
		if (server != NULL) return server;
	
		CSLOCK(m_cs)
		{
			////현재 연결된 유저 목록에서 지우고...
			//V::iterator it;
			VEC_CONTANINER::iterator it = \
				find_if(m_vecRogueServer.begin(), m_vecRogueServer.end(), \
				[&](T* server)->bool { return id == server->GetId(); } \
			);

			if (it != m_vecRogueServer.end())
				server = *it;
			else
			{
				MAP_CONTANINER::iterator it = \
					find_if(this->m_mapCon.begin(), this->m_mapCon.end(), \
					[&])
			}

		}

		return server;
	}

	void RemoveAuthServer(int key) { this->MapPopBack(key); }
	void PushServer(T* server) { this->VecPushBack(server); }
	void PushRogueServer(T* server) {
		if (server == NULL) return;
		
		CSLOCK(m_cs)
		{
			VEC_CONTANINER::iterator it = std::find(m_vecRogueServer.begin(), m_vecRogueServer.end(), server);
		
			if (it != m_vecRogueServer.end())
				return;
		
			m_vecRogueServer.push_back(server);
		}
	}
	void AddAuthServer(int id, T* server) { this->MapPushBack(id, server); }
	T* PopServer() { return (T*)this->VecPopBack(); }
	void PopRogueServer() {
		if (m_vecRogueServer.size() == 0)
		{
			return;
		}

		SServer* value = NULL;

		CSLOCK(m_cs)
		{
			VEC_CONTANINER::iterator it = m_vecRogueServer.begin();

			value = *it;
			m_vecRogueServer.erase(it);
		}
	}

public:
	SServerContainer() {
		for (int i = 0; i < SERVER_SIZE; i++)
		{
			PushServer(m_value[i]);
		}
	}
	~SServerContainer() {
	}

private:
	VEC_CONTANINER m_vecRogueServer; // 비인증 서버 목록

	T m_value[SERVER_SIZE];
	DWORD m_tickPing;

	static SServerContainer* m_instance;
};

