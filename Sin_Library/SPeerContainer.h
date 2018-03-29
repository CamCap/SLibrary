#pragma once

#include "ssocket.h"
#include "Container.h"
#include "IOCP.h"
#include "Log.h"

#define MAX_USER_COUNT 500

struct SOCKET_CONTEXT;

template<typename T = SPeer>
class SPeerContainer
	:public Container<T>
{
protected:
	using Container<T>::MapPopBack;
	using Container<T>::VecPopBack;
	using Container<T>::MapPushBack;
	using Container<T>::VecPushBack;

public:
	void Remove_CurPeer(int key) { MapPopBack(key); }
	T* Pop_EmptyPeer() { return  VecPopBack(); }
	void Add_CurPeer(int key, T* value) { MapPushBack(key, value); }
	void Push_EmptyPeer(T* value) { VecPushBack(value); }
		
public:
	static SPeerContainer* GetInstance()
	{
		if (m_instance == NULL)
			m_instance = new SPeerContainer();

		return m_instance;
	}

public:
	SPeerContainer()
	{
		T* pValue;
		for (int i = 0; i < MAX_USER_COUNT; i++)
		{
			pValue = &m_value[i];
			//		m_vecEmptyUser[i] = pUser;
			VecPushBack(pValue);
		}
	}
	virtual ~SPeerContainer()
	{

	}

	void DisConnect(SOCKET_CONTEXT* pUser) {
		if (pUser == NULL)
		{
#ifdef _DEBUG
			OutputDebugStringA("[ERROR] NULL인 유저를 지운다. [UserContainer::DisConnect]\n");
#endif
			return;
		}

#ifdef _DEBUG
		OutputDebugStringA("[ERROR] NULL인 유저를 지운다. [UserContainer::DisConnect]\n");
#endif

		CSLOCK(this->m_cs)
		{
			////현재 연결된 유저 목록에서 지우고...
			typename std::map<int, T*>::iterator it;
			it = this->m_mapCon.find(pUser->m_puser->GetId());

			if (it == this->m_mapCon.end())
				return;

			this->m_mapCon.erase(it);

		}
		//소켓 강제 종료
		///다음의 기능은 http://egloos.zum.com/mirine35/v/5057014 참조
		///우아한 종료로 TIMEOUT이 발생할 수 있음. 시나리오는 위를 참조.
		///이 TIMEOUT이 무한 발생(실제로는 240초까지만)하면, 소켓을 사용할 수 없다.
		///이를 위해 closesocket이 대기하는 시간을 조절

		LINGER LingerStruct;
		LingerStruct.l_onoff = 1;
		LingerStruct.l_linger = 0;

		if (setsockopt(pUser->m_socket, SOL_SOCKET, SO_LINGER, (char*)&LingerStruct, sizeof(LingerStruct) == SOCKET_ERROR))
		{
			//return;
			ERROR_LOG("Set Socket LINGER Error ");
		}

		//closesocket((SOCKET)*pUser);
		//(SOCKET)*pUser = INVALID_SOCKET;
		pUser->m_puser->ReleaseSocket();
	}

	T* FindPeer(int id)
	{
		CSLOCK(this->m_cs)
		{
			typename std::map<int, T*>::iterator it;
			it = this->m_mapCon.find(id);

			if (it == this->m_mapCon.end())
				return NULL;

			return it->second;
		}
	}

protected:
	T m_value[MAX_USER_COUNT];

	static SPeerContainer* m_instance;
};


template<typename T = SPeer>
SPeerContainer<T>* SPeerContainer<T>::m_instance = NULL;