#pragma once
#include "ssocket.h"
#include "Container.h"
#include <functional>

#define SERVER_SIZE 10
#define PING_CHECK_TIME 10000

template <class T = SServer>
class SServerContainer
{
private:
	using VEC_CONTAINER = std::vector<T*>;
	using MAP_CONTAINER = std::map<int, T*>;

public:
	void PushServer(T* _ty) { m_vecServer.push(_ty); }
	void PushAuthServer(T* _ty) { m_vecAuth.push(_ty); }
	T* PopServer() { return m_vecServer.pop(); }
	T* PopAuthServer(T* _ty) { return m_mapServer.pop(); }
	
	void DisConnectServer(T* _ty) {
		T* pElement = FindServer(_ty->GetID());
		
		if (pElement == NULL) return;

		pElement->ReleaseSocket();
		this->PushServer(pElement);
	}

	//using find_function = bool (*_find_function)(T*);
	//typedef bool(*find_function)(T*);

	T* FindServer(int id) {

		T* pElement = m_vecAuth.find([&](T* ty)->bool {\
			return id == ty->GetID(); \
		});

		return pElement;
	}
	
	void PingCheck(DWORD tick) {
		if (tick - m_tickPing < PING_CHECK_TIME)
			return;

		m_tickPing = tick;

		m_vecAuth.process([&](T* server)->void { dynamic_cast<SServer*>(server)->OnPingCheck(tick); });
	}

	template <typename process_function, typename... Args>
	void process(process_function pf, Args... arg)
	{
		m_vecAuth.process(pf, arg...);
	}

public:
	SServerContainer() : m_vecServer(0), m_vecAuth(0) {}
	~SServerContainer() {}

private:
	VecContainer<T> m_vecServer; // 미사용 서버
	VecContainer<T> m_vecAuth; // 사용중 서버

	DWORD m_tickPing;
};

