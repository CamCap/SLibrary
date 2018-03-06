#include "stdafx.h"
#include "SUserContainer.h"
#include "ssocket.h"
#include "IOCP.h"
#include "Log.h"

UserContainer* UserContainer::m_instance = NULL;

UserContainer* UserContainer::GetInstance()
{
	if (m_instance == NULL) m_instance = new UserContainer();

	return m_instance;
}

UserContainer::UserContainer()
	:m_cs()
{
	SPeer* pUser;
	for (int i = 0; i < MAX_USER_COUNT; i++)
	{
		pUser = &m_user[i];
		//		m_vecEmptyUser[i] = pUser;
		m_vecEmptyUser.push_back(pUser);
	}

}


UserContainer::~UserContainer()
{
}


void UserContainer::Push_EmptyUser(SPeer* puser)
{
	if (puser == NULL)
		return;

	CSLOCK(m_cs)
	{
		VECTOR_USER::iterator it = std::find(m_vecEmptyUser.begin(), m_vecEmptyUser.end(), puser);

		if (it != m_vecEmptyUser.end())
			return;

		m_vecEmptyUser.push_back(puser);
	}
	//UnLock();

	return;
}


void UserContainer::Add_CurUser(int userid, SPeer* puser)
{
	if (puser == NULL)
		return; \

		CSLOCK(m_cs)
	{
		m_mapConnectUser.insert(std::pair<int, SPeer*>(userid, puser));
	}

	return;
}

SPeer* UserContainer::Pop_EmptyUser()
{
	//SUser* pUser = NULL;

	if (m_vecEmptyUser.size() == 0)
	{
		//UnLock();
		return NULL;
	}

	//	this->m_vecEmptyUser.erase

	SPeer* puser = NULL;

	CSLOCK(m_cs)
	{
		VECTOR_USER::iterator ituser = m_vecEmptyUser.begin();

		puser = *ituser;
		m_vecEmptyUser.erase(ituser);
	}
	//UnLock();
	//SUser* puser = (SUser*)ituser._Ptr;
	return puser;
}

void UserContainer::Remove_CurUser(SOCKET sock)
{
	MAP_USER::iterator it = m_mapConnectUser.find(sock);
	if (it != m_mapConnectUser.end())
	{
		m_mapConnectUser.erase(it);
	}
}

void UserContainer::DisConnect(SOCKET_CONTEXT* pUser)
{
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

	CSLOCK(m_cs)
	{
		//현재 연결된 유저 목록에서 지우고...
		MAP_USER::iterator it = m_mapConnectUser.find(pUser->m_socket);

		if (it == m_mapConnectUser.end())
			return;

		m_mapConnectUser.erase(it);
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
