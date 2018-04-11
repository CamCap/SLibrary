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
	void PushAuthServer(T* _ty) { m_mapServer.push(_ty); }
	T* PopServer() { return m_vecServer.pop(); }
	T* PopAuthServer(T* _ty) { return m_mapServer.pop(); }
	
	void DisConnectServer(T* _ty) {
		T* pElement = FindServer(_ty->GetID());
		
		if (pElement == NULL) return;

		pElement->ReleaseSocket();
		this->PushServer(pElement);
	}

	T* FindServer(int id) {
		T* pElement = m_mapServer.find([&](T* ty)->bool {\
			return id == ty->GetID(); \
		});

		return pElement;
	}
	
	void PingCheck(DWORD tick) {
		if (tick - m_tickPing < PING_CHECK_TIME)
			return;

		m_tickPing = tick;

		m_mapServer.process < std::function<void(DWORD)>, DWORD> (std::bind(&SServer::OnPingCheck), tick);
	}

public:
	SServerContainer() {}
	~SServerContainer() {}

private:
	VecContainer<T> m_vecServer; // 미사용 서버
	MapContainer<T> m_mapServer; // 사용중 서버

	DWORD m_tickPing;
};

