#pragma once
#include "ssocket.h"
#include "Container.h"
#include <functional>

#define SERVER_SIZE 10
#define PING_CHECK_TIME 10000

template <class T = SServer>
class SPeerContainer
{
private:
	using VEC_CONTAINER = std::vector<T*>;
	using MAP_CONTAINER = std::map<int, T*>;

public:
	void PushWaitPeer(T* _ty) { m_vecWaitPeer.push(_ty); }
	void PushActive(T* _ty) { m_vecActivePeer.push(_ty); }
	T* PopWaitPeer() { return m_vecWaitPeer.pop(); }
	T* PopActivePeer() { return m_vecActivePeer.pop(); }
	void PopActivePeer(T* _ty) { m_vecActivePeer.Erase(_ty); }

	/*void DisConnectPeer(T* _ty) {
		T* pElement = Find(_ty->GetID());
		
		if (pElement == NULL) return;

		pElement->ReleaseSocket();
		m_vecActivePeer.Erase(_ty);
		m_vecWaitPeer.push(_ty);
	}*/

	//using find_function = bool (*_find_function)(T*);
	//typedef bool(*find_function)(T*);

	T* Find(int id) {

		T* pElement = m_vecActivePeer.find([&](T* ty)->bool {\
			return id == ty->GetID(); \
		});

		return pElement;
	}
	
	template<class find_function>
	T* Find(find_function f){
		return m_vecActivePeer.find(f);
	}

	void PingCheck(DWORD tick) {
		if (tick - m_tickPing < PING_CHECK_TIME)
			return;

		m_tickPing = tick;

		m_vecActivePeer.process([&](T* peer)->void { reinterpret_cast<SServer*>(peer)->OnPingCheck(tick); });
	}


	template <typename process_function, typename... Args>
	void process(process_function pf, Args... arg)
	{
		m_vecActivePeer.process(pf, arg...);
	}

	int WaitPeerNum() { return m_vecWaitPeer.size(); }
	int ActivePeerNum() { return m_vecActivePeer.size(); }
public:
	SPeerContainer() : m_vecWaitPeer(0), m_vecActivePeer(0) {}
	~SPeerContainer() {}

private:
	VecContainer<T> m_vecWaitPeer; // 미사용 
	VecContainer<T> m_vecActivePeer; // 사용중 

	DWORD m_tickPing;
};

