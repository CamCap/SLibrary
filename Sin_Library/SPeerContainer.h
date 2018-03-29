#pragma once

#include "ssocket.h"
#include "Container.h"
#include "IOCP.h"
#include "Log.h"

#define MAX_USER_COUNT 500


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

	void DisConnect(T* pUser) {
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
			it = this->m_mapCon.find(pUser->GetId());

			if (it == this->m_mapCon.end())
				return;

			this->m_mapCon.erase(it);

		}

		//closesocket((SOCKET)*pUser);
		//(SOCKET)*pUser = INVALID_SOCKET;
		pUser->ReleaseSocket();
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

		return NULL;
	}

protected:
	T m_value[MAX_USER_COUNT];

	static SPeerContainer* m_instance;
};


template<typename T = SPeer>
SPeerContainer<T>* SPeerContainer<T>::m_instance = NULL;